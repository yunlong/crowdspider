#ifndef __SHA_H__
#define __SHA_H__

namespace matrix { 

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
  unsigned long H[5];
  unsigned long W[80];
  int lenW;
  unsigned long sizeHi,sizeLo;
} SHA_CTX;


void shaInit(SHA_CTX *ctx);
void shaUpdate(SHA_CTX *ctx, unsigned char *dataIn, int len);
void shaFinal(SHA_CTX *ctx, unsigned char hashout[20]);
void shaBlock(unsigned char *dataIn, int len, unsigned char hashout[20]);

#ifdef __cplusplus
}
#endif

}

#endif
