#include "bp_io.h"
#include "bp_utils.h"
#include <string.h>

int bp_io_readint32 (FILE *file, unsigned int *input)
{
	unsigned char ch [4];
	int res = fscanf (file, "%c%c%c%c", ch, ch + 1, ch + 2, ch + 3);

	*input = ((int)ch [0] << 24) | ((int)ch [1] << 16) | ((int)ch [2] << 8) | ((int)ch[3]);
	return res != EOF;
}

int bp_io_readfloat (FILE *file, float *input) {
	unsigned int *inputalias = (unsigned int *)input;

	return bp_io_readint32 (file, inputalias);
}


void bp_io_writeint32 (FILE *file, unsigned int input)
{
	fprintf (file, "%c%c%c%c", (input >> 24), ((input >> 16) & 0xFF),
		 ((input >> 8) & 0xFF), (input & 0xFF));
}

void bp_io_writefloat (FILE *file, float finput)
{
	int input = *(int *)(void *)&finput;

	bp_io_writeint32 (file, input);
}

FILE *bp_file_open (scene_t *scene, const char *filename, const char *mode) {
	char **library_path;
	char *path;
	int len;
	FILE *file;

	int flen = strlen (filename);

	for (library_path = scene->library_path; *library_path; library_path++) {
		len = strlen (*library_path);
#ifdef G_IS_DIR_SEPARATOR
		if (G_IS_DIR_SEPARATOR ((*library_path) [len - 1])) {
#else
		if ((*library_path) [len -1] == G_DIR_SEPARATOR) {
#endif
			path = NEW_0 (char, len + flen + 1);
			sprintf (path, "%s%s", *library_path, filename);
		} else {
			path = NEW_0 (char, len + flen + 2);
			sprintf (path, "%s" G_DIR_SEPARATOR_S "%s", 
				 *library_path, filename);
		}

		file = fopen (path, mode);
		free (path);
		if (file)
			return file;
	}
		
	return NULL;
}

#ifndef WIN32
static char * path_canonicalize_unix (const char *path);
#endif

void bp_library_path_add1 (scene_t *scene, char const *path)
{
	char **paths = scene->library_path;
	int i = 0;
#ifndef WIN32
	char *canonical_path = path_canonicalize_unix (path);
#endif
	for (; *paths; i++, paths++) {
		assert (i < MAX_LIBRARY_PATH);
#ifdef WIN32
		if (!strcmp (*paths, path))
#else
		if (!strcmp (*paths, canonical_path))
#endif
			return;
	}
	*paths = strdup (path);
}

static char *getnextpos (char *start)
{
	char *next = start;
	while (*next) {
		if (*next == G_SEARCHPATH_SEPARATOR)
			return next;
		next++;
	}
	return next;
}

void bp_library_path_add (scene_t *scene, char *paths)
{
	char *library_path;
	char *path, *next;

	if (!paths)
		return;
	
	path = paths;
	do {
		next = getnextpos (path);
		library_path = (char *)malloc (next - path + 1);
		snprintf (library_path, next-path+1, "%s", path);
		bp_library_path_add1 (scene, library_path);
		free (library_path);
		path = next + 1;
	} while (*next);
}

#define CHAR_COPY(dest, src)  do {\
  *(dest)=*(src); \
   (dest)++; \
   (src)++; \
} while (0)

enum states {
	CHARACTER = 1,
	ONE_DOT = 2,
	TWO_DOT = 4,
       SLASH = 8
};

#ifndef WIN32
static char * path_canonicalize_unix (const char *path)
{
	char *canonical_path, *cdot_pos;
	char *cbegin_pos, *ccurr_pos;
	const char *ocurr_pos;
	int state;

	state = SLASH;
	if (path [0] != '/') {
		char pwd [256];
		int pwdlen;

		if (getcwd (pwd, sizeof (pwd))) {
			pwdlen = strlen (pwd);
			canonical_path = NEW_0 (char, strlen (path) + pwdlen +1);
			sprintf (canonical_path, "%s/", pwd);

			ccurr_pos = canonical_path + pwdlen;
			ocurr_pos = (char *)path;
		}
		else {
			bp_report (BP_LOG_ERROR, "getcwd failed");
			exit (1);
		}
	} else {
		canonical_path = NEW_0 (char, strlen (path) + 1);
		canonical_path [0] = '/';
		ccurr_pos = canonical_path  + 1;
		ocurr_pos = (char *)path;
	}
	cbegin_pos = canonical_path + 1;

	do {
		switch (*ocurr_pos) {
		case '\0':
			*ccurr_pos = '\0';
			if (state == CHARACTER)
				return canonical_path;
			if (state & (ONE_DOT | SLASH)) {
				/* try to remove any trailing slashes */
				while (ccurr_pos != cbegin_pos && *(--ccurr_pos) == '/')
					*ccurr_pos = '\0';
				return canonical_path;
			}
			if (state & (TWO_DOT)) {
				if (cdot_pos != cbegin_pos) {
					--cdot_pos;
					if (cdot_pos [0] == '/' && cdot_pos > cbegin_pos) {
						/*    /etc/..
                                                         ^
						*/
						while (*(-- cdot_pos) != '/')
							;
						if (ccurr_pos > cdot_pos)
							*cdot_pos = '\0';
						else
							*ccurr_pos = '\0';
					}
					ccurr_pos ++;
				}
				ocurr_pos ++;
				return canonical_path;
			}
			return NULL;
		case '.':
			if (state == CHARACTER) {
				CHAR_COPY (ccurr_pos, ocurr_pos);
			}
			else if (state == SLASH) {
				state = ONE_DOT;
				cdot_pos = ccurr_pos;
				ocurr_pos ++;
			}
			else if (state == ONE_DOT) {
				state = TWO_DOT;
				ocurr_pos ++;
			}
			else {
/*                              state = CHARACTER; */
/*                              CHAR_COPY (ccurr_pos, ocurr_pos); */
			}
			break;
		case '/':
			if (state == CHARACTER) {
				state = SLASH;
				CHAR_COPY (ccurr_pos, ocurr_pos);
			}
			else if (state == SLASH) {
				ocurr_pos ++;
			}
			else if (state == ONE_DOT) {
				state = SLASH;
				ocurr_pos ++;
			}
			else if (state == TWO_DOT) {
				state = SLASH;
				if (cdot_pos != cbegin_pos) {
					--cdot_pos;
					if (cdot_pos [0] == '/') {
						/*    /etc/../
                                                         ^
						*/
						while (*(-- cdot_pos) != '/' && cdot_pos > cbegin_pos)
							;
						if (cdot_pos == cbegin_pos)
							ccurr_pos = cbegin_pos;
						else if (ccurr_pos > cdot_pos)
							ccurr_pos = cdot_pos + 1;
						else
							ccurr_pos ++;
					}
					else
						ccurr_pos ++;
				}
				ocurr_pos ++;
			}
			break;
		default:
			if (state & (CHARACTER | SLASH))
				;
			else if (state & ONE_DOT) {
				*ccurr_pos = '.';
				ccurr_pos ++;
			}
			else if (state & TWO_DOT) {
				*ccurr_pos = '.';
				ccurr_pos ++;
				*ccurr_pos = '.';
				ccurr_pos ++;
			}
			CHAR_COPY (ccurr_pos, ocurr_pos);
			state = CHARACTER;
			break;
		}
	} while (1);
}
#endif
