#include <ui.h>
#include <config.h>

#ifdef PLATFORM_WIN32

void update_immediate(const unsigned int keyup, const unsigned int key) {

	const int keydown = !keyup;
	const float value = keydown ? 1.f : 0.f;
	switch (key) {
		/* axis */
	case VK_LEFT:get_user_input ()->axis[1] = -value; break;
	case VK_RIGHT:get_user_input ()->axis[1] = +value; break;
	case VK_DOWN:get_user_input ()->axis[0] = -value; break;
	case VK_UP:get_user_input ()->axis[0] = +value; break;
	case VK_DELETE:get_user_input ()->axis[2] = -value; break;
	case VK_NEXT:get_user_input ()->axis[2] = +value; break;

	case VK_INSERT:get_user_input ()->axis[3] = -value; break;
	case VK_PRIOR:get_user_input ()->axis[3] = +value; break;

	case VK_HOME:get_user_input ()->axis[3] = keyup ? 0.f : -1.f; break;
	case VK_END:get_user_input ()->axis[3] = keyup ? 0.f : +1.f; break;

	case VK_SHIFT:get_user_input ()->modifiers.shift = keydown; break;
	case VK_CONTROL:get_user_input ()->modifiers.control = keydown; break;
	case VK_MENU:get_user_input ()->modifiers.menu = keydown; break;

	default: ;
	}
}

void update_queue_click(const int button, const int x, const int y) 
{
/*
	inputs_t::event_t e;
	e.click.type = inputs_t::type_click;
	e.click.button = button;
	e.click.x = x;
	e.click.y = y;
	get_user_input ()->queue.push_back(e);
*/
}

void update_queue_key(const unsigned int key) {
	switch (key) {
	case 27:
	case 'q':
		get_user_input ()->exit = 1;
		break;
	default:
		/* we should put this on a stack */
		get_user_input ()->keys [0] = key;
		break;
	}
}

static LRESULT CALLBACK 
WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch(uMsg) {
		/* Recieved when the user hits the X box or alt-F4 */
	case WM_CLOSE:
		/* Tell the program to end, and dispath WM_DESTROY */
		get_user_input ()->exit = 1;
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		/* End the program. */
		get_user_input ()->exit = 1;
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP: update_immediate(uMsg == WM_KEYDOWN ? 0 : 1, (unsigned int)wParam); return 0;
	case WM_CHAR: update_queue_key(wParam); return 0;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN: update_queue_click(uMsg-WM_LBUTTONDOWN, LOWORD(lParam), HIWORD(lParam)); return 0;
	default:
		/* DefWindowProc processes anything we don't. */
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

static const char *ClassName = "MyWindow";

int
window_create (ui_t *ui, framebuffer_t *fb)
{
	const HINSTANCE hInstance = 0;
	/* The window class struct. */
	WNDCLASSEX wc;
	PIXELFORMATDESCRIPTOR pfd;
	int format;

	/* The size of the structure */
	memset(&wc, 0, sizeof(wc));
	wc.style = CS_OWNDC;	
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;

	wc.cbSize = sizeof(WNDCLASSEX);

	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
/* 	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION); */
	wc.hIcon = LoadIcon(hInstance, IDI_EXCLAMATION);
	wc.lpszClassName = ClassName;


	/* Register the class to be used with CreateWindow. */
	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Could not register window.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	
	ui->wnd = CreateWindowEx (WS_EX_WINDOWEDGE, ClassName, PACKAGE_STRING, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 
				  CW_USEDEFAULT, bp_framebuffer_get_horizontal_resolution (fb),
				  bp_framebuffer_get_vertical_resolution (fb), NULL, NULL, hInstance, NULL);

	ui->dc = GetDC(ui->wnd);
        pfd.nSize = sizeof( pfd );
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |	PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 24;
        pfd.cDepthBits = 16;
        pfd.iLayerType = PFD_MAIN_PLANE;
        
	format = ChoosePixelFormat(ui->dc, &pfd );
        SetPixelFormat(ui->dc, format, &pfd );

        /*  create the render context (RC)  */
        ui->glrc = wglCreateContext(ui->dc);

        /*  make it the current render context */
        wglMakeCurrent(ui->dc, ui->glrc );

	/* Set the windows show state, to show it. */
 	ShowWindow(ui->wnd, SW_SHOW);

	return 1;
}

void
window_update (ui_t *ui)
{
	int rc;
	MSG msg;


	/* Draw the window. */
	UpdateWindow(ui->wnd);

	/* Retrieve messages from the message queue. */
	while ((rc = GetMessage(&msg, NULL, 0, 0) > 0)) {

		if (get_user_input ()->exit)
			return;

		/* Convert virtual key messages into a char */
		TranslateMessage(&msg);
		/* Send the message to our WndProc function */
		DispatchMessage(&msg);

		break;
	}

}

void
window_swap_buffers (ui_t *ui)
{
 	SwapBuffers(ui->dc); 
}

void
window_destroy (ui_t *ui)
{
	if (!get_user_input ()->exit) {
		ReleaseDC(ui->wnd, ui->dc );
		wglMakeCurrent(0,0);
		wglDeleteContext (ui->glrc);
		UnregisterClass(ClassName, 0 /* histance */);

	}
}
#endif
