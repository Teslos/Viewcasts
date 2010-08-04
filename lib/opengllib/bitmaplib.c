#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "paulslib.h"
#include "bitmaplib.h"

/*
   Create a bitmap structure
*/
BITMAP *CreateBitmap(int nx,int ny)
{
   return(malloc(nx*ny*sizeof(BITMAP)));
}

/*
   Destroy the bitmap structure
*/
void DestroyBitmap(BITMAP *bm)
{
   free(bm);
}

/*
   Write a bitmap to a file
	The format is as follows
	A negative format indicates a image vertical flip
   1 == tga file
   2 == ppm file
	3 == rgb file
	4 == raw grey scale
   5 == tiff file
	6 == EPS colour (Encapsulated PostScript)
	7 == EPS black and white
	8 == raw file
   9 == BMP file
*/
void WriteBitmap(char *file_name, FILE *fptr, BITMAP *bm, int nx, int ny, int format)
{
	int i,j,jstart,jstop,jstep,index,offset;
	int linelength = 0,size;
	char buffer[1024];

	/* Write the header */
	switch (ABS(format)) {
		case 1:
			BM_WriteHexString(fptr,"000002");        	/* uncompressed RGB	*/
			BM_WriteHexString(fptr,"0000000000");
			BM_WriteHexString(fptr,"00000000");		/* X and Y origin 	*/
			putc((nx & 0x00ff),fptr);			/* X width		*/
			putc((nx & 0xff00) / 256,fptr);
			putc((ny & 0x00ff),fptr);	/* Y width				*/
			putc((ny & 0xff00) / 256,fptr);
			BM_WriteHexString(fptr,"1800");  /* 24 bit bitmap 		*/
			break;
		case 2:
			fprintf(fptr,"P6\n%d %d\n255\n",nx,ny);
			break;
		case 3:
			BM_WriteHexString(fptr,"01da00010003");
			putc((nx & 0xFF00) / 256,fptr);
			putc((nx & 0x00FF),fptr);
			putc((ny & 0xFF00) / 256,fptr);
			putc((ny & 0x00FF),fptr);
			BM_WriteHexString(fptr,"000300000000000000ff00000000");
			fprintf(fptr,"WriteBitmap, pdb");
			BM_WriteHexString(fptr,"0000000000000000"); 
			break;
		case 4:
			break;
		case 5:
			BM_WriteHexString(fptr,"4d4d002a");	/* Little endian & TIFF identifier */
			offset = nx * ny * 3 + 8;
			BM_WriteLongInt(fptr,buffer,offset);
			break;
		case 6:
			fprintf(fptr,"%%!PS-Adobe-3.0 EPSF-3.0\n");
			fprintf(fptr,"%%%%Creator: Created from bitmaplib by Paul Bourke\n");
			fprintf(fptr,"%%%%BoundingBox: %d %d %d %d\n",0,0,nx,ny);
			fprintf(fptr,"%%%%LanguageLevel: 2\n");
			fprintf(fptr,"%%%%Pages: 1\n");
			fprintf(fptr,"%%%%DocumentData: Clean7Bit\n");
			fprintf(fptr,"%d %d scale\n",nx,ny);
			fprintf(fptr,"%d %d 8 [%d 0 0 -%d 0 %d]\n",nx,ny,nx,ny,ny);
			fprintf(fptr,"{currentfile 3 %d mul string readhexstring pop} bind\n",nx);
			fprintf(fptr,"false 3 colorimage\n");
			break;
		case 7:
			fprintf(fptr,"%%!PS-Adobe-3.0 EPSF-3.0\n");
			fprintf(fptr,"%%%%Creator: Created from bitmaplib by Paul Bourke\n");
			fprintf(fptr,"%%%%BoundingBox: %d %d %d %d\n",0,0,nx,ny);
			fprintf(fptr,"%%%%LanguageLevel: 2\n");
			fprintf(fptr,"%%%%Pages: 1\n");
			fprintf(fptr,"%%%%DocumentData: Clean7Bit\n");
			fprintf(fptr,"%d %d scale\n",nx,ny);
			fprintf(fptr,"%d %d 8 [%d 0 0 -%d 0 %d]\n",nx,ny,nx,ny,ny);
			fprintf(fptr,"{currentfile %d string readhexstring pop} bind\n",nx);
			fprintf(fptr,"false 1 colorimage\n");
			break;
		case 8:
			break;
		case 9:
			/* Header 10 bytes */
			putc('B',fptr);
			putc('M',fptr);
			size = nx * ny * 3 + 14 + 40;
			putc((size) % 256,fptr);
			putc((size / 256) % 256,fptr);
			putc((size / 65536) % 256,fptr);
			putc((size / 16777216),fptr);
			putc(0,fptr); putc(0,fptr); 
			putc(0,fptr); putc(0,fptr);
			/* Offset to image data */
			putc(14+40,fptr); putc(0,fptr); putc(0,fptr); putc(0,fptr); 
			/* Information header 40 bytes */
			putc(0,fptr); putc(0,fptr); putc(0,fptr); putc(0,fptr); 
			putc((nx) % 256,fptr);
			putc((nx / 256) % 256,fptr);
			putc((nx / 65536) % 256,fptr);
			putc((nx / 16777216),fptr);
			putc((ny) % 256,fptr);
			putc((ny / 256) % 256,fptr);
			putc((ny / 65536) % 256,fptr);
			putc((ny / 16777216),fptr);
			putc(1,fptr); putc(0,fptr); /* One plane */
			putc(24,fptr); putc(0,fptr); /* 24 bits */
			/* Compression type == 0 */
			putc(0,fptr); putc(0,fptr); putc(0,fptr); putc(0,fptr); 
			size = nx * ny * 3;
			putc((size) % 256,fptr);
			putc((size / 256) % 256,fptr);
			putc((size / 65536) % 256,fptr);
			putc((size / 16777216),fptr);
			putc(1,fptr); putc(0,fptr); putc(0,fptr); putc(0,fptr); 
			putc(1,fptr); putc(0,fptr); putc(0,fptr); putc(0,fptr); 
			putc(0,fptr); putc(0,fptr); putc(0,fptr); putc(0,fptr); /* No palette */
			putc(0,fptr); putc(0,fptr); putc(0,fptr); putc(0,fptr); 
			break;
	}

	/* Write the binary data */
	for (j=0;j<ny;j++) {
		for (i=0;i<nx;i++) {
			if (format > 0)
				index = j * nx + i;
			else
				index = (ny - 1 - j) * nx + i;
			switch (ABS(format)) {
				case 1:
					fputc(bm[index].b,fptr);
					fputc(bm[index].g,fptr);
					fputc(bm[index].r,fptr);
					break;
				case 2:
				case 3:
				case 5:
				case 8:
				case 9:
					fputc(bm[index].r,fptr);
					fputc(bm[index].g,fptr);
					fputc(bm[index].b,fptr);
					break;
				case 4:
					fputc((bm[index].r+bm[index].g+bm[index].b)/3,fptr);
					break;
				case 6:
					fprintf(fptr,"%02x%02x%02x",bm[index].r,bm[index].g,bm[index].b);
					linelength += 6;
					if (linelength >= 72 || linelength >= nx) {
						fprintf(fptr,"\n");
						linelength = 0;
					}	
					break;
				case 7:
					fprintf(fptr,"%02x",(bm[index].r+bm[index].g+bm[index].b)/3);
					linelength += 2;
					if (linelength >= 72 || linelength >= nx) {
						fprintf(fptr,"\n");
						linelength = 0;
					} 
					break;
			}
		}
	}

	/* Write the footer */
	switch (ABS(format)) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 8:
		case 9:
			break;
		case 5:
			BM_WriteHexString(fptr,"000e");/* The number of directory entries (14) */

			/* Width tag, short int */
			BM_WriteHexString(fptr,"0100000300000001");
			fputc((nx & 0xff00) / 256,fptr);		/* Image width */
			fputc((nx & 0x00ff),fptr);
			BM_WriteHexString(fptr,"0000");

			/* Height tag, short int */
			BM_WriteHexString(fptr,"0101000300000001");
			fputc((ny & 0xff00) / 256,fptr);    /* Image height */
			fputc((ny & 0x00ff),fptr);
			BM_WriteHexString(fptr,"0000");

			/* bits per sample tag, short int */
			BM_WriteHexString(fptr,"0102000300000003");
			offset = nx * ny * 3 + 182;
			BM_WriteLongInt(fptr,buffer,offset);

			/* Compression flag, short int */
			BM_WriteHexString(fptr,"010300030000000100010000");

			/* Photometric interpolation tag, short int */
			BM_WriteHexString(fptr,"010600030000000100020000");

			/* Strip offset tag, long int */
			BM_WriteHexString(fptr,"011100040000000100000008");

			/* Orientation flag, short int */
			BM_WriteHexString(fptr,"011200030000000100010000");

			/* Sample per pixel tag, short int */
			BM_WriteHexString(fptr,"011500030000000100030000");

			/* Rows per strip tag, short int */
			BM_WriteHexString(fptr,"0116000300000001");
			fputc((ny & 0xff00) / 256,fptr); 
			fputc((ny & 0x00ff),fptr);
			BM_WriteHexString(fptr,"0000");

			/* Strip byte count flag, long int */
			BM_WriteHexString(fptr,"0117000400000001");
			offset = nx * ny * 3;
			BM_WriteLongInt(fptr,buffer,offset);

			/* Minimum sample value flag, short int */
			BM_WriteHexString(fptr,"0118000300000003");
			offset = nx * ny * 3 + 188;
			BM_WriteLongInt(fptr,buffer,offset);

			/* Maximum sample value tag, short int */
			BM_WriteHexString(fptr,"0119000300000003");
			offset = nx * ny * 3 + 194;
			BM_WriteLongInt(fptr,buffer,offset);

			/* Planar configuration tag, short int */
			BM_WriteHexString(fptr,"011c00030000000100010000");

			/* Sample format tag, short int */
			BM_WriteHexString(fptr,"0153000300000003");
			offset = nx * ny * 3 + 200;
			BM_WriteLongInt(fptr,buffer,offset);

			/* End of the directory entry */
			BM_WriteHexString(fptr,"00000000");

			/* Bits for each colour channel */
			BM_WriteHexString(fptr,"000800080008");

			/* Minimum value for each component */
			BM_WriteHexString(fptr,"000000000000");

			/* Maximum value per channel */
			BM_WriteHexString(fptr,"00ff00ff00ff");

			/* Samples per pixel for each channel */
			BM_WriteHexString(fptr,"000100010001");

			break;
		case 6:
		case 7:
			fprintf(fptr,"\n%%%%EOF\n");
			break;
	}
}

void BM_WriteLongInt(FILE *fptr,char *s,long n)
{
	int i;

	s[0] = (n & 0xff000000) / 16777216;
	s[1] = (n & 0x00ff0000) / 65536;
	s[2] = (n & 0x0000ff00) / 256;
	s[3] = (n & 0x000000ff);

	for (i=0;i<4;i++)
		putc(s[i],fptr);
}

void BM_WriteHexString(FILE *fptr,char *s)
{
	int i,c;
	char hex[3];

	for (i=0;i<strlen(s);i+=2) {
      hex[0] = s[i];
      hex[1] = s[i+1];
      hex[2] = '\0';
      sscanf(hex,"%X",&c);
		fputc(c,fptr);
	}
}

/*
   Clear the bitmap to a particular colour
*/
void EraseBitmap(BITMAP *bm, int nx, int ny, BITMAP col)
{
   int i,j,index;

   for (i=0;i<nx;i++) {
      for (j=0;j<ny;j++) {
         index = j * nx + i;
         bm[index] = col;
      }
   }
}

/*
	Resample a bitmap
	Apply a gaussian radial average if r > 0
	r is in units of the input image
*/
void ReSample(BITMAP *bm_in,int nx,int ny,BITMAP *bm_out,int nnx,int nny,double r)
{
	int i,j,ii,jj,index,ci,cj;
	double x,y,cx,cy,red,green,blue,dist2,r2,weight,sum;
	BITMAP col,black = {0,0,0};

	r2 = r*r;

	for (i=0;i<nnx;i++) {
		for (j=0;j<nny;j++) {
			col = black;
			if (r2 <= 0) {
         	ci = (i * nx) / nnx;
         	cj = (j * ny) / nny;
				index = cj * nx + ci;
				col = bm_in[index];
			} else {
            cx = i * nx / (double)nnx;
            cy = j * ny / (double)nny;
				red   = 0;
				green = 0;
				blue  = 0;
				sum = 0;
				for (x=cx-4*r;x<=cx+4*r+0.01;x++) {
					for (y=cy-4*r;y<=cy+4*r+0.01;y++) {
						ii = x;
						jj = y;
						if (ii < 0)
							ii = 0;
						if (ii >= nx)
							ii = nx-1;;
						if (jj < 0)
							jj = 0;
						if (jj >= ny) 
							jj = ny-1;
						dist2 = (cx-x)*(cx-x) + (cy-y)*(cy-y);
						weight = exp(-0.5*dist2/r2) / (r2*TWOPI);
						index = jj * nx + ii;
						red   += weight * bm_in[index].r;
						green += weight * bm_in[index].g;
						blue  += weight * bm_in[index].b;
						sum += weight;
					}
				}
				col.r = red;
            col.g = green;
            col.b = blue;
			}
			bm_out[j * nnx + i] = col;
		}
	}
}

/*
   Turn on a pixel of a bitmap
*/
int DrawPixel(BITMAP *bm, int nx, int ny, int x, int y, BITMAP col)
{
   int index;

   if (x < 0 || y < 0 || x >= nx || y >= ny)
      return(FALSE);
   index = y * nx + x;
   bm[index] = col;
	return(TRUE);
}

/*
   Return the value of a pixel
*/
BITMAP GetPixel(BITMAP *bm, int nx, int ny, int x, int y)
{
   int index;
	BITMAP black = {0,0,0};

	if (x < 0 || y < 0 || x >= nx || y >= ny)
		return(black);
   index = y * nx + x;
   return(bm[index]);
}

/*
   Draw a line from (x1,y1) to (x2,y2)
   Use colour col
*/
void DrawLine(BITMAP *bm,int nx,int ny,int x1,int y1,int x2,int y2,BITMAP col)
{
   int i,j,index;
   double mu,dx,dy,dxy;

   dx = x2 - x1;
   dy = y2 - y1;
   dxy = sqrt(dx*dx + dy*dy);
   for (mu=0;mu<=dxy;mu++) {
      i = x1 + mu * dx / dxy;
      j = y1 + mu * dy / dxy;
		if (i < 0 || j < 0 || i >= nx || j >= ny)
			continue;
      index = j * nx + i;
      bm[index] = col;
   }
}

