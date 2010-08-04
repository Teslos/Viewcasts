#ifndef OPENGLLIB_H
#define OPENGLLIB_H

int WindowDump(int,int,int,int,char*);
void ClearBuffers(int);
void CreateBox(XYZ,XYZ,int);
void CreateGrid(XYZ,XYZ,int);
void CreatePolygon(XYZ *,XYZ *,COLOUR *,int);
void CreateSphere(XYZ,double,int,int,double,double,double,double);
void CreateSimpleSphere(XYZ,double,int,int);
void CreateEllipsoid(XYZ,double,int,double,double,double);
void CreateDisk(XYZ,XYZ,double,double,int,double,double);
void CreateCone(XYZ,XYZ,double,double,int,double,double);
void CreateTorus(XYZ,double,double,int,int,double,double,double,double,int);
void CreateStars(XYZ,double);
PIXELA *ReadRawTexture(int,int,char *,COLOUR,int);
PIXELA *ReadPPMTexture(char *,COLOUR,int,int *,int *,int);
PIXELA *ReadRawTexture2(int,int,char *,int);
void DrawGLText(int,int,char *);
void DrawTextXY(double,double,double,double,char *);
void CreateLabelVector(char *,XYZ,XYZ,XYZ,XYZ *,int *);
void Draw3DPlus(XYZ,double);

#endif /* OPENGLLIB_H */
