#define SUB(res, op1, op2) do {\
          (res)[0] = (op1)[0] - (op2)[0]; \
          (res)[1] = (op1)[1] - (op2)[1]; \
          (res)[2] = (op1)[2] - (op2)[2]; \
          } while (0)

#define ADD(res, op1, op2) do {\
          (res)[0] = (op1)[0] + (op2)[0]; \
          (res)[1] = (op1)[1] + (op2)[1]; \
          (res)[2] = (op1)[2] + (op2)[2]; \
          } while (0)

#define MUL(res, op1, op2) do {\
          (res)[0] = (op1)[0] * (op2)[0]; \
          (res)[1] = (op1)[1] * (op2)[1]; \
          (res)[2] = (op1)[2] * (op2)[2]; \
          } while (0)

#define DIV(res, op1, op2) do { \
          (res)[0] = (op1)[0] / (op2)[0]; \
          (res)[1] = (op1)[1] / (op2)[1]; \
          (res)[2] = (op1)[2] / (op2)[2]; \
          } while (0)

#define NEG(res, op) do {\
          (res)[0] = -(op)[0]; \
          (res)[1] = -(op)[1]; \
          (res)[2] = -(op)[2]; \
          } while (0)

#define SMUL(res, op1, op2) do {\
          float f  = (op1);\
          (res)[0] = (op2) [0] * f; \
          (res)[1] = (op2) [1] * f; \
          (res)[2] = (op2) [2] * f; \
          } while (0)


#define DOT(op1, op2) ((op1)[0]*(op2)[0] + (op1)[1]*(op2)[1] + (op1)[2]*(op2)[2])

#define MAG(v) (sqrt((double)((v)[0]*(v)[0]+(v)[1]*(v)[1]+(v)[2]*(v)[2])))

#define ASSIGN(dest, src) do {\
        (dest)[0] = (src)[0]; (dest)[1] = (src)[1]; (dest)[2] = (src)[2];\
} while (0)

#define ASSIGN4(dest, src) do {\
         (dest)[0] = (src)[0]; (dest)[1] = (src)[1]; \
         (dest)[2] = (src)[2]; (dest)[3] = (src)[3]; \
} while (0)

#define ASSIGN5(dest, src) do {\
         (dest)[0] = (src)[0]; (dest)[1] = (src)[1]; (dest)[2] = (src)[2];\
         (dest)[3] = (src)[3]; (dest)[4] = (src)[4];\
} while (0)

#define COPY(src, dest) do {		\
	  (dest)[0] = (src)[0]; (dest)[1] = (src)[1]; (dest)[2] = (src)[2]; \
} while (0)

#define VRESIZE(v, size) do { SMUL (v, size/MAG(v), v);} while (0)

#define VSET_SIZE(res, v, size) do {SMUL (res, size/MAG (v), v);} while (0)


#define CROSS(c,a,b) do {\
          (c)[0]=(a)[1]*(b)[2]-(a)[2]*(b)[1]; \
          (c)[1]=(a)[2]*(b)[0]-(a)[0]*(b)[2]; \
          (c)[2]=(a)[0]*(b)[1]-(a)[1]*(b)[0]; \
          } while (0)

#define COPY3(dest, src) do {\
         (dest)[0] = (src)[0]; (dest)[1] = (src)[1]; (dest)[2] = (src)[2];\
          } while (0)

#define COPY4(src, dest) do {\
         (dest)[0] = (src)[0]; (dest)[1] = (src)[1]; (dest)[2] = (src)[2]; \
         (dest)[3] = (src)[3]; \
          } while (0)

#define COPY5(src, dest) do { \
         (dest)[0] = (src)[0]; (dest)[1] = (src)[1]; (dest)[2] = (src)[2];\
         (dest)[3] = (src)[3]; (dest)[5] = (src)[5]; \
          } while (0)

#define LIN_COMB3(res, t1, v1, t2, v2) do { \
             (res) [0] = (t1) * (v1) [0] + (t2) * (v2) [0]; \
             (res) [1] = (t1) * (v1) [1] + (t2) * (v2) [1]; \
             (res) [2] = (t1) * (v1) [2] + (t2) * (v2) [2]; \
        } while (0)

#define SCALE_ADD3(res, k, v1, v2) do { \
	     (res) [0] = (k) * (v1) [0] + (v2) [0]; \
	     (res) [1] = (k) * (v1) [1] + (v2) [1]; \
	     (res) [2] = (k) * (v1) [2] + (v2) [2]; \
        } while (0)

#define SCALE_SUB3(res, k, v1, v2) do { \
	     (res) [0] = (k) * (v1) [0] - (v2) [0]; \
	     (res) [1] = (k) * (v1) [1] - (v2) [1]; \
	     (res) [2] = (k) * (v1) [2] - (v2) [2]; \
        } while (0)

#define VSCALE_ADD3(res, v1, v2, v3) do {\
	     (res) [0] = (v2) [0] * (v1) [0] + (v3) [0]; \
	     (res) [1] = (v2) [1] * (v1) [1] + (v3) [1]; \
	     (res) [2] = (v2) [2] * (v1) [2] + (v3) [2]; \
        } while (0)

#define vector3_inverse(res, v) do {\
             (res) [0] = 1 / (v) [0]; \
             (res) [1] = 1 / (v) [1]; \
             (res) [2] = 1 / (v) [2]; \
        } while ( 0)

#define VSET_ALL(v, val) {v [0] = val; v [1] = val; v [2] = val;}
#define ZERO4(v) {v [0] = 0.0; v [1] = 0.0; v [2] = 0.0; v [3] = 0.0;}
