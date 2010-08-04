 /**************************************************************************
  *                                                                        *
  * Copyright (c) 1996-98 Silicon Graphics, Inc.                           *
  *                                                                        *
  * Permission to use, copy, modify, distribute, and sell this software    *
  * and its documentation for any purpose is hereby granted without fee,   *
  * provided that the name of Silicon Graphics may not be used in any      *
  * advertising or publicity relating to the software without the          *
  * specific, prior written permission of Silicon Graphics.                *
  *                                                                        *
  * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,     *
  * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY       *
  * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.       *
  *                                                                        *
  * IN NO EVENT SHALL SILICON GRAPHICS BE LIABLE FOR ANY SPECIAL,          *
  * INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY      *
  * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,        *
  * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY *
  * OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR          *
  * PERFORMANCE OF THIS SOFTWARE.                                          *
  *                                                                        *
  **************************************************************************/

/*
 *	sgiobjgl -
 *		A simple object format - graphics support.
 *
 *			Paul Haeberli - 1990
 */

#include <stdio.h>
#include <GL/gl.h>
#include "sgiobj.h"

sgiobj *newquadobj(int nquads)
{
    sgiobj *obj;

    obj = (sgiobj *)malloc(sizeof(sgiobj));
    obj->next = 0;
    obj->objtype = OBJ_QUADLIST;
    obj->nlongs = PNTLONGS*4*nquads;
    obj->data = (long *)malloc(obj->nlongs*sizeof(long));
    obj->xnlongs = 0;
    obj->xdata = 0;
    return obj;
}

sgiobj *newtriobj(int ntri)
{
    sgiobj *obj;

    obj = (sgiobj *)malloc(sizeof(sgiobj));
    obj->next = 0;
    obj->objtype = OBJ_TRILIST;
    obj->nlongs = PNTLONGS*3*ntri;
    obj->data = (long *)malloc(obj->nlongs*sizeof(long));
    obj->xnlongs = 0;
    obj->xdata = 0;
    return obj;
}

sgiobj *newtmeshobj(int nlongs)
{
    sgiobj *obj;

    obj = (sgiobj *)malloc(sizeof(sgiobj));
    obj->next = 0;
    obj->objtype = OBJ_TRIMESH;
    obj->nlongs = nlongs;
    obj->data = (long *)malloc(obj->nlongs*sizeof(long));
    obj->xnlongs = 0;
    obj->xdata = 0;
    return obj;
}

sgiobj *readsgiobj(char *name)
{
    FILE *inf;
    sgiobj *obj, *head, *tail;
    int npoints, colors;
    long objtype, plongs, nlongs;
    long magic;
    int i, ntri, nquads;

    inf = fopen(name,"r");
    if(!inf) {
        fprintf(stderr,"readsgiobj: can't open input file %s\n",name);
        exit(1);
    }
    fread(&magic,sizeof(long),1,inf);
    if(magic == FASTMAGIC) {
	fread(&npoints,sizeof(long),1,inf);
	fread(&colors,sizeof(long),1,inf);
	nquads = npoints/4; 
	if(colors) {
	    fprintf(stderr,"readsgiobj: can't read nonormal spin objects\n");
	    exit(1);
	} else
	    obj = newquadobj(nquads);
	bzero(obj->data,obj->nlongs*sizeof(long));
	for(i=0; i<npoints; i++) {
	    fread(obj->data+(PNTLONGS*i)+OFFSET_NORMAL,3*sizeof(long),1,inf);
	    fread(obj->data+(PNTLONGS*i)+OFFSET_POINT,3*sizeof(long),1,inf);
	    bcopy(obj->data+(PNTLONGS*i)+OFFSET_POINT,
		  obj->data+(PNTLONGS*i)+OFFSET_UVS,3*sizeof(long),1,inf);
	}
	fclose(inf);
	return obj;
    } 
    if(magic == SOMAGIC) {
	head = 0;
	while(1) {
	    fread(&objtype,sizeof(long),1,inf);
	    if(objtype == OBJ_END)
		break;
	    fread(&nlongs,sizeof(long),1,inf);
	    switch(objtype) {
		case OBJ_QUADLIST:
		    nquads = (nlongs/PNTLONGS)/4;
		    obj  = newquadobj(nquads);
		    fread(obj->data,nlongs*sizeof(long),1,inf);
		    break;
		case OBJ_TRILIST:
		    ntri = (nlongs/PNTLONGS)/3;
		    obj  = newtriobj(ntri);
		    fread(obj->data,nlongs*sizeof(long),1,inf);
		    break;
		case OBJ_TRIMESH:
		    obj = newtmeshobj(nlongs);
		    fread(obj->data,nlongs*sizeof(long),1,inf);
		    break;
		default:
		    fprintf(stderr,"readsgiobj: bad obj type %d\n",objtype);
		    exit(1);
		    break;
	    }
	    if(head == 0) {
		head = tail = obj;
	    } else {
		tail->next = obj;
		tail = obj;
	    }
	}
	fclose(inf);
	return head;
    }
    fprintf(stderr,"readsgiobj: bad magic %d in object file\n",magic);
    return NULL;
}


#define BYTEOFF(v)	(sizeof(long)*(v))

void drawsgiobj(sgiobj *obj,int how)
{
    long npolys;
    long *data;
    char *vertdata, *avert;
    int vertlongs, nverts;
	float *lastv, *last_lastv;

    data = obj->data;
    if(obj->objtype == OBJ_QUADLIST) {
	npolys = (obj->nlongs/PNTLONGS)/4;
	switch(how) {
	    case DRAW_POINTS:
		while(npolys--) {
		    glBegin( GL_TRIANGLE_STRIP );
		    glVertex3fv((float*)&data[(PNTLONGS*0)+OFFSET_POINT]);
		    glVertex3fv((float*)&data[(PNTLONGS*1)+OFFSET_POINT]);
		    glVertex3fv((float*)&data[(PNTLONGS*3)+OFFSET_POINT]);
		    glVertex3fv((float*)&data[(PNTLONGS*2)+OFFSET_POINT]);
		    glEnd();
		    data += PNTLONGS*4;
		}
		break;
	    case DRAW_POINTS|DRAW_NORMALS:
		while(npolys--) {
		    glBegin( GL_TRIANGLE_STRIP );
		    glNormal3fv((float*)&data[(PNTLONGS*0)+OFFSET_NORMAL]);
		    glVertex3fv((float*)&data[(PNTLONGS*0)+OFFSET_POINT]);
		    glNormal3fv((float*)&data[(PNTLONGS*1)+OFFSET_NORMAL]);
		    glVertex3fv((float*)&data[(PNTLONGS*1)+OFFSET_POINT]);
		    glNormal3fv((float*)&data[(PNTLONGS*3)+OFFSET_NORMAL]);
		    glVertex3fv((float*)&data[(PNTLONGS*3)+OFFSET_POINT]);
		    glNormal3fv((float*)&data[(PNTLONGS*2)+OFFSET_NORMAL]);
		    glVertex3fv((float*)&data[(PNTLONGS*2)+OFFSET_POINT]);
		    glEnd();
		    data += PNTLONGS*4;
		}
		break;
	    case DRAW_POINTS|DRAW_UVS:
		while(npolys--) {
		    glBegin( GL_TRIANGLE_STRIP );
		    glTexCoord2fv((float*)&data[(PNTLONGS*0)+OFFSET_UVS]);
		    glVertex3fv((float*)&data[(PNTLONGS*0)+OFFSET_POINT]);
		    glTexCoord2fv((float*)&data[(PNTLONGS*1)+OFFSET_UVS]);
		    glVertex3fv((float*)&data[(PNTLONGS*1)+OFFSET_POINT]);
		    glTexCoord2fv((float*)&data[(PNTLONGS*3)+OFFSET_UVS]);
		    glVertex3fv((float*)&data[(PNTLONGS*3)+OFFSET_POINT]);
		    glTexCoord2fv((float*)&data[(PNTLONGS*2)+OFFSET_UVS]);
		    glVertex3fv((float*)&data[(PNTLONGS*2)+OFFSET_POINT]);
		    glEnd();
		    data += PNTLONGS*4;
		}
		break;
	    case DRAW_POINTS|DRAW_UVS|DRAW_NORMALS:
		while(npolys--) {
		    glBegin( GL_TRIANGLE_STRIP );
		    glTexCoord2fv((float*)&data[(PNTLONGS*0)+OFFSET_UVS]);
		    glNormal3fv((float*)&data[(PNTLONGS*0)+OFFSET_NORMAL]);
		    glVertex3fv((float*)&data[(PNTLONGS*0)+OFFSET_POINT]);
		    glTexCoord2fv((float*)&data[(PNTLONGS*1)+OFFSET_UVS]);
		    glNormal3fv((float*)&data[(PNTLONGS*1)+OFFSET_NORMAL]);
		    glVertex3fv((float*)&data[(PNTLONGS*1)+OFFSET_POINT]);
		    glTexCoord2fv((float*)&data[(PNTLONGS*3)+OFFSET_UVS]);
		    glNormal3fv((float*)&data[(PNTLONGS*3)+OFFSET_NORMAL]);
		    glVertex3fv((float*)&data[(PNTLONGS*3)+OFFSET_POINT]);
		    glTexCoord2fv((float*)&data[(PNTLONGS*2)+OFFSET_UVS]);
		    glNormal3fv((float*)&data[(PNTLONGS*2)+OFFSET_NORMAL]);
		    glVertex3fv((float*)&data[(PNTLONGS*2)+OFFSET_POINT]);
		    glEnd();
		    data += PNTLONGS*4;
		}
		break;
	    case DRAW_POINTS|DRAW_COLORS:
		while(npolys--) {
		    glBegin( GL_TRIANGLE_STRIP );
		    glColor3fv((float*)&data[(PNTLONGS*0)+OFFSET_COLOR]);
		    glVertex3fv((float*)&data[(PNTLONGS*0)+OFFSET_POINT]);
		    glColor3fv((float*)&data[(PNTLONGS*1)+OFFSET_COLOR]);
		    glVertex3fv((float*)&data[(PNTLONGS*1)+OFFSET_POINT]);
		    glColor3fv((float*)&data[(PNTLONGS*3)+OFFSET_COLOR]);
		    glVertex3fv((float*)&data[(PNTLONGS*3)+OFFSET_POINT]);
		    glColor3fv((float*)&data[(PNTLONGS*2)+OFFSET_COLOR]);
		    glVertex3fv((float*)&data[(PNTLONGS*2)+OFFSET_POINT]);
		    glEnd();
		    data += PNTLONGS*4;
		}
		break;
	    case DRAW_POINTS|DRAW_COLORS|DRAW_NORMALS:
		while(npolys--) {
		    glBegin( GL_TRIANGLE_STRIP );
		    glColor3fv((float*)&data[(PNTLONGS*0)+OFFSET_COLOR]);
		    glNormal3fv((float*)&data[(PNTLONGS*0)+OFFSET_NORMAL]);
		    glVertex3fv((float*)&data[(PNTLONGS*0)+OFFSET_POINT]);
		    glColor3fv((float*)&data[(PNTLONGS*1)+OFFSET_COLOR]);
		    glNormal3fv((float*)&data[(PNTLONGS*1)+OFFSET_NORMAL]);
		    glVertex3fv((float*)&data[(PNTLONGS*1)+OFFSET_POINT]);
		    glColor3fv((float*)&data[(PNTLONGS*3)+OFFSET_COLOR]);
		    glNormal3fv((float*)&data[(PNTLONGS*3)+OFFSET_NORMAL]);
		    glVertex3fv((float*)&data[(PNTLONGS*3)+OFFSET_POINT]);
		    glColor3fv((float*)&data[(PNTLONGS*2)+OFFSET_COLOR]);
		    glNormal3fv((float*)&data[(PNTLONGS*2)+OFFSET_NORMAL]);
		    glVertex3fv((float*)&data[(PNTLONGS*2)+OFFSET_POINT]);
		    glEnd();
		    data += PNTLONGS*4;
		}
		break;
	}
    } else if(obj->objtype == OBJ_TRILIST) {
	npolys = (obj->nlongs/PNTLONGS)/3;
	switch(how) {
	    case DRAW_POINTS:
		while(npolys--) {
		    glBegin( GL_TRIANGLE_STRIP );
		    glVertex3fv((float*)&data[(PNTLONGS*0)+OFFSET_POINT]);
		    glVertex3fv((float*)&data[(PNTLONGS*1)+OFFSET_POINT]);
		    glVertex3fv((float*)&data[(PNTLONGS*2)+OFFSET_POINT]);
		    glEnd();
		    data += PNTLONGS*3;
		}
		break;
	    case DRAW_POINTS|DRAW_NORMALS:
		while(npolys--) {
		    glBegin( GL_TRIANGLE_STRIP );
		    glNormal3fv((float*)&data[(PNTLONGS*0)+OFFSET_NORMAL]);
		    glVertex3fv((float*)&data[(PNTLONGS*0)+OFFSET_POINT]);
		    glNormal3fv((float*)&data[(PNTLONGS*1)+OFFSET_NORMAL]);
		    glVertex3fv((float*)&data[(PNTLONGS*1)+OFFSET_POINT]);
		    glNormal3fv((float*)&data[(PNTLONGS*2)+OFFSET_NORMAL]);
		    glVertex3fv((float*)&data[(PNTLONGS*2)+OFFSET_POINT]);
		    glEnd();
		    data += PNTLONGS*3;
		}
		break;
	    case DRAW_POINTS|DRAW_UVS:
		while(npolys--) {
		    glBegin( GL_TRIANGLE_STRIP );
		    glTexCoord2fv((float*)&data[(PNTLONGS*0)+OFFSET_UVS]);
		    glVertex3fv((float*)&data[(PNTLONGS*0)+OFFSET_POINT]);
		    glTexCoord2fv((float*)&data[(PNTLONGS*1)+OFFSET_UVS]);
		    glVertex3fv((float*)&data[(PNTLONGS*1)+OFFSET_POINT]);
		    glTexCoord2fv((float*)&data[(PNTLONGS*2)+OFFSET_UVS]);
		    glVertex3fv((float*)&data[(PNTLONGS*2)+OFFSET_POINT]);
		    glEnd();
		    data += PNTLONGS*3;
		}
		break;
	    case DRAW_POINTS|DRAW_UVS|DRAW_NORMALS:
		while(npolys--) {
		    glBegin( GL_TRIANGLE_STRIP );
		    glNormal3fv((float*)&data[(PNTLONGS*0)+OFFSET_NORMAL]);
		    glTexCoord2fv((float*)&data[(PNTLONGS*0)+OFFSET_UVS]);
		    glVertex3fv((float*)&data[(PNTLONGS*0)+OFFSET_POINT]);
		    glNormal3fv((float*)&data[(PNTLONGS*1)+OFFSET_NORMAL]);
		    glTexCoord2fv((float*)&data[(PNTLONGS*1)+OFFSET_UVS]);
		    glVertex3fv((float*)&data[(PNTLONGS*1)+OFFSET_POINT]);
		    glNormal3fv((float*)&data[(PNTLONGS*2)+OFFSET_NORMAL]);
		    glTexCoord2fv((float*)&data[(PNTLONGS*2)+OFFSET_UVS]);
		    glVertex3fv((float*)&data[(PNTLONGS*2)+OFFSET_POINT]);
		    glEnd();
		    data += PNTLONGS*3;
		}
		break;
	    case DRAW_POINTS|DRAW_COLORS:
		while(npolys--) {
		    glBegin( GL_TRIANGLE_STRIP );
		    glColor3fv((float*)&data[(PNTLONGS*0)+OFFSET_COLOR]);
		    glVertex3fv((float*)&data[(PNTLONGS*0)+OFFSET_POINT]);
		    glColor3fv((float*)&data[(PNTLONGS*1)+OFFSET_COLOR]);
		    glVertex3fv((float*)&data[(PNTLONGS*1)+OFFSET_POINT]);
		    glColor3fv((float*)&data[(PNTLONGS*2)+OFFSET_COLOR]);
		    glVertex3fv((float*)&data[(PNTLONGS*2)+OFFSET_POINT]);
		    glEnd();
		    data += PNTLONGS*3;
		}
		break;
	    case DRAW_POINTS|DRAW_COLORS|DRAW_NORMALS:
		while(npolys--) {
		    glBegin( GL_TRIANGLE_STRIP );
		    glColor3fv((float*)&data[(PNTLONGS*0)+OFFSET_COLOR]);
		    glNormal3fv((float*)&data[(PNTLONGS*0)+OFFSET_NORMAL]);
		    glVertex3fv((float*)&data[(PNTLONGS*0)+OFFSET_POINT]);
		    glColor3fv((float*)&data[(PNTLONGS*1)+OFFSET_COLOR]);
		    glNormal3fv((float*)&data[(PNTLONGS*1)+OFFSET_NORMAL]);
		    glVertex3fv((float*)&data[(PNTLONGS*1)+OFFSET_POINT]);
		    glColor3fv((float*)&data[(PNTLONGS*2)+OFFSET_COLOR]);
		    glNormal3fv((float*)&data[(PNTLONGS*2)+OFFSET_NORMAL]);
		    glVertex3fv((float*)&data[(PNTLONGS*2)+OFFSET_POINT]);
		    glEnd();
		    data += PNTLONGS*3;
		}
		break;
	}
    } else if(obj->objtype == OBJ_TRIMESH) {
	if(how == DRAW_LINES)
	    return;
	else {
	    vertlongs = *data++;
	    vertdata = (char *)data;
	    data += vertlongs;
	    while(1) {
		switch(*data++) {
		    case OP_BGNTMESH:
			glBegin( GL_TRIANGLE_STRIP );
			break;
		    case OP_SWAPTMESH:
			glEnd();
			glBegin( GL_TRIANGLE_STRIP );
			glVertex3fv( lastv );
			glVertex3fv( last_lastv );
			break;
		    case OP_ENDBGNTMESH:
			glEnd();
			glBegin( GL_TRIANGLE_STRIP );
			break;
		    case OP_ENDTMESH:
			glEnd();
			return;
		    default:
			fprintf(stderr,"drawsgiobj: bad tmesh op %d\n",*data);
			exit(1);
		}
		nverts = *data++;
		switch(how) {
		    case DRAW_POINTS:
			while(nverts--) {
			    avert = vertdata + *data++;
				last_lastv = lastv;
				lastv = (float *)(avert+BYTEOFF(OFFSET_POINT));
			    glVertex3fv((float*)(avert+BYTEOFF(OFFSET_POINT)));
			}
			break;
		    case DRAW_POINTS|DRAW_NORMALS:
			while(nverts--) {
			    avert = vertdata + *data++;
				last_lastv = lastv;
				lastv = (float *)(avert+BYTEOFF(OFFSET_POINT));
			    glNormal3fv((float*)(avert+BYTEOFF(OFFSET_NORMAL)));
			    glVertex3fv((float*)(avert+BYTEOFF(OFFSET_POINT)));
			}
			break;
		    case DRAW_POINTS|DRAW_UVS:
			while(nverts--) {
			    avert = vertdata + *data++;
				last_lastv = lastv;
				lastv = (float *)(avert+BYTEOFF(OFFSET_POINT));
			    glTexCoord2fv((float*)(avert+BYTEOFF(OFFSET_UVS)));
			    glVertex3fv((float*)(avert+BYTEOFF(OFFSET_POINT)));
			}
			break;
		    case DRAW_POINTS|DRAW_UVS|DRAW_NORMALS:
			while(nverts--) {
			    avert = vertdata + *data++;
				last_lastv = lastv;
				lastv = (float *)(avert+BYTEOFF(OFFSET_POINT));
			    glTexCoord2fv((float*)(avert+BYTEOFF(OFFSET_UVS)));
			    glNormal3fv((float*)(avert+BYTEOFF(OFFSET_NORMAL)));
			    glVertex3fv((float*)(avert+BYTEOFF(OFFSET_POINT)));
			}
			break;
		    case DRAW_POINTS|DRAW_COLORS:
			while(nverts--) {
			    avert = vertdata + *data++;
				last_lastv = lastv;
				lastv = (float *)(avert+BYTEOFF(OFFSET_POINT));
			    glColor3fv((float*)(avert+BYTEOFF(OFFSET_COLOR)));
			    glVertex3fv((float*)(avert+BYTEOFF(OFFSET_POINT)));
			}
			break;
		    case DRAW_POINTS|DRAW_COLORS|DRAW_NORMALS:
			while(nverts--) {
			    avert = vertdata + *data++;
				last_lastv = lastv;
				lastv = (float *)(avert+BYTEOFF(OFFSET_POINT));
			    glColor3fv((float*)(avert+BYTEOFF(OFFSET_COLOR)));
			    glNormal3fv((float*)(avert+BYTEOFF(OFFSET_NORMAL)));
			    glVertex3fv((float*)(avert+BYTEOFF(OFFSET_POINT)));
			}
		}
	    }
	}
    } else {
	fprintf(stderr,"drawsgiobj: bad object type %d\n",obj->objtype);
	exit(1);
    }
}

void writesgiobj(char *name,sgiobj *obj)
{
    FILE *outf;
    long endtype;
    int magic;

    outf = fopen(name,"w");
    if(!outf) {
        fprintf(stderr,"writesgiobj: can't open input file %s\n",name);
        exit(1);
    }
    magic = SOMAGIC;
    fwrite(&magic,sizeof(long),1,outf);
    while(obj) {
    fwrite(&obj->objtype,sizeof(long),1,outf);
    fwrite(&obj->nlongs,sizeof(long),1,outf);
    fwrite(obj->data,obj->nlongs*sizeof(long),1,outf);
    obj = obj->next;
    }
    endtype = OBJ_END;
    fwrite(&endtype,sizeof(long),1,outf);
    fclose(outf);
}
