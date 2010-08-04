#ifndef BITMAPLIB_H
#define BITMAPLIB_H

typedef struct {
   unsigned char r,g,b;
} BITMAP;

BITMAP *CreateBitmap(int,int);
void DestroyBitmap(BITMAP *);
void WriteBitmap(char*,FILE *,BITMAP *,int,int,int);
void WriteHexString(FILE *,char *);
void EraseBitmap(BITMAP *,int,int,BITMAP);
void ReSample(BITMAP *,int,int,BITMAP *,int,int,double);
int DrawPixel(BITMAP *,int,int,int,int,BITMAP);
BITMAP GetPixel(BITMAP *,int,int,int,int);
void DrawLine(BITMAP *,int,int,int,int,int,int,BITMAP);
void writergb(char *, int,int,int, char*);
void BM_WriteLongInt(FILE *,char *,long);
void BM_WriteHexString(FILE *,char *);

#endif /* BITMAPLIB_H */
