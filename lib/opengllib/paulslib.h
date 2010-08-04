#ifndef PAULSLIB_H
#define PAULSLIB_H

#define ABS(x) (x < 0 ? -(x) : (x))
#define MIN(x,y) (x < y ? x : y)
#define MAX(x,y) (x > y ? x : y)
#define SIGN(x) (x < 0 ? (-1) : 1)
#define MODULUS(p) (sqrt(p.x*p.x + p.y*p.y + p.z*p.z))
#define DISTANCE(p1,p2) \
	(sqrt((p1.x-p2.x)*(p1.x-p2.x) + \
	(p1.y-p2.y)*(p1.y-p2.y) + \
	(p1.z-p2.z)*(p1.z-p2.z)))
#define CROSSPROD(p1,p2,p3) \
	p3.x = p1.y*p2.z - p1.z*p2.y; \
	p3.y = p1.z*p2.x - p1.x*p2.z; \
	p3.z = p1.x*p2.y - p1.y*p2.x
#define DOTPRODUCT(v1,v2) ( v1.x*v2.x + v1.y*v2.y + v1.z*v2.z )

#define SWAP_2(x) ( (((x) & 0xff) << 8) | ((unsigned short)(x) >> 8) )
#define SWAP_4(x) ( ((x) << 24) | \
         (((x) << 8) & 0x00ff0000) | \
         (((x) >> 8) & 0x0000ff00) | \
         ((x) >> 24) )
#define FIX_SHORT(x) (*(unsigned short *)&(x) = \
         SWAP_2(*(unsigned short *)&(x)))
#define FIX_LONG(x) (*(unsigned *)&(x) = \
         SWAP_4(*(unsigned *)&(x)))
#define FIX_FLOAT(x) FIX_LONG(x)

typedef struct {
   double real,imag;
} COMPLEX;

typedef struct {
	double x,y;
} XY;

typedef struct {
	double x,y,z;
} XYZ;

typedef struct {
	XYZ p1,p2,p3;		/* Vertices */
	XYZ c;         	/* Centroid */
	XYZ n;				/* Normal   */
} FACET3;

typedef struct {
	XYZ p[3];			/* Vertices */
	XYZ c;				/* Centroid */
	XYZ n[3];			/* Normal   */
} TRIANGLE;
typedef struct {
	XYZ p[3];			/* Vertices */
	XYZ c;				/* Centroid */
	XYZ n[3];			/* Normal   */
	double val[3];                  /* Value    */
} TRIANGLEF;
typedef struct {
   int p1,p2,p3;
} ITRIANGLE;

typedef struct {
   XYZ p1,p2,p3,p4;  /* Vertices */
   XYZ c;        	 	/* Centroid */
   XYZ n;         	/* Normal   */
} FACET4;

typedef struct {
	XYZ p1,p2;
} EDGE;
typedef struct {
   int p1,p2;
} IEDGE;

typedef struct {
	XYZ center;
	double radius;
} SPHERE;

typedef struct {
   int np;
   XYZ p[4];
} POLY34;

typedef struct {
  XYZ p[8];
  XYZ n[8];
  double val[8];
} GRIDCELL;
typedef struct {
  double val[8];
} FIELD;

typedef struct {
   double r,g,b;
} COLOUR;

typedef struct {
   unsigned char r,g,b;
} PIXEL;
typedef struct {
   unsigned char b,g,r;
} PIXELBGR;
typedef struct {
   unsigned char r,g,b,a;
} PIXELA;

typedef struct {
   double h,v;
} Point;

typedef struct {
	int h,v;
} POINT;

typedef struct {
   double left,top,right,bottom;
} Rect;

typedef struct {
	XYZ p1,p2;
} BOX;

#define YES 1
#define NO  0
#define TRUE  1
#define FALSE 0
#define CLOCKWISE 1
#define ANTICLOCKWISE (-1)
#define COUNTERCLOCKWISE (-1)
#define CONVEX 1
#define CONCAVE (-1)
#define ESC 27

#define THREEPI         9.424777960769379715387930
#define EPSILON         0.0000001
#define EPS             0.00001

#define DTOR            0.0174532925
#define RTOD            57.2957795
#define TWOPI           6.283185307179586476925287
#define PI              3.141592653589793238462643
#define PID2            1.570796326794896619231322
#define PID3            1.047197551196397746154214
#define PID4            0.7853981633974483096156608
#define PIPI            9.869604401089358618834491
#define E               2.718281828459045235360287
#define GAMMA           0.5772156649015328606065120
#define LOG2            0.3010299956639811952137389
#define LOG3            0.4772112547196624372950279
#define LOGPI           0.4971498726941338543512683
#define LOGE            0.4342944819032518276511289
#define LOG2E           1.4426950408889634073
#define LN2             0.6931471805599453094172321
#define LN3             1.098612288668109691395245
#define LNPI            1.144729885849400174143427
#define LNSQRT2         0.3465735902799726547
#define LNGAMMA         -0.549539293981644822337662
#define SQRT2           1.4142135623730950488
#define SQRT3           1.7320508075688772935
#define SQRT5           2.2360679774997896964
#define SQRT7           2.6457513110645905905
#define SQRTPI          1.772453850905516027298167

/* Converts from rand() to range 0-1 ie: 1/(2^15-1) */
#define RANDSCALE       0.0000305185095

/* Prototypes  ---------------------------------------------------------*/
void   SplineCurve(XYZ *,int,int *,int,XYZ *,int);
void   SplineKnots(int *,int,int);
void   SplinePoint(int *,int,int,double,XYZ *,XYZ *);
double SplineBlend(int,int,int *,double);
XYZ    Bezier3(XYZ,XYZ,XYZ,double);
XYZ    Bezier4(XYZ,XYZ,XYZ,XYZ,double);
XYZ    Bezier(XYZ *,int,double);
double BezierBlend(int,double,int);
XYZ    CubicBezier(XYZ,XYZ,XYZ,XYZ,double);
XYZ    VertexInterp(double,XYZ,XYZ,double,double);
double FieldInterp(XYZ,XYZ,XYZ,double,double);
int    LinRegress(double *,double *,int,double *,double *,double *);

double Angle2D(double,double,double,double);
double DotProduct(XYZ,XYZ);
double VectorAngle(XYZ,XYZ);
XYZ    CrossProduct(XYZ,XYZ);
double VectorLength(XYZ,XYZ);
double Modulus(XYZ);
void   Normalise(XYZ *);
int    VectorEqual(XYZ,XYZ);
XYZ    VectorInvert(XYZ);
XYZ    VectorSub(XYZ,XYZ);
XYZ    VectorAdd(XYZ,XYZ);
XYZ    VectorMul(XYZ,double);
void   MakeVector(XYZ *,double,double,double);
XY     Vector2DRotate(XY,double);
XYZ    RotateX(XYZ,double);
XYZ    RotateY(XYZ,double);
XYZ    RotateZ(XYZ,double);
XYZ    MidPoint(XYZ,XYZ);
XYZ    Centroid(XYZ *,int);
double FowlerAngle(double,double);
void   UpdateBounds(XYZ,XYZ *,XYZ *);
XYZ    CalcNormal(XYZ,XYZ,XYZ);
int    LineLineIntersect(XYZ,XYZ,XYZ,XYZ,XYZ *,XYZ *,double *, double *);
int    FindClosest(XYZ,XYZ *,int,double *,XYZ *);
int    ClipFacet(XYZ *,XYZ,XYZ);
int    ClipLine2Box(XYZ *,XYZ *,XYZ,XYZ);
int    PointInBox(XYZ,XYZ,XYZ);
int    ContourFacet(XYZ *,XYZ,XYZ,XYZ *,XYZ *);
int    LineFacet(XYZ,XYZ,XYZ,XYZ,XYZ,XYZ *);
int    LineIntersect(double,double,double,double,
                     double,double,double,double,
                     double *,double *);
double PointLine2D(XY,XY,XY,XY *,double *);
int    ClockWise(XY *,int);
int    ConvexPolygon(XY *,int);
void   XYZ2Polar(double,double,double,double *,double *,double *);
void   Polar2XYZ(double,double,double,double,double,double);
int    Triangulate(int,XYZ *,ITRIANGLE *,int *);
int    CircumCircle(double,double,
	       double,double,double,double,double,double,
			 double *,double *,double *);

/* FILE IO ---------------------------------------------------------*/
int    RLECompress(unsigned char *,unsigned char *,int);
void   RLEUncompress(unsigned char *,unsigned char *,int);
int    ReadLine(FILE *,char *,int);
int    ReadUntil(FILE *,int,char *);
void   ReadToString(FILE *,char *);
int    ReadString(FILE *,char *,int);
int    ReadShort(FILE *,short *,int);
int    WriteShort(FILE *,short,int);
int    ReadUShort(FILE *,unsigned short *,int);
int    WriteUShort(FILE *,unsigned short,int);
int    ReadLong(FILE *,long *,int);
int 	 WriteLong(FILE *,long,int);
int    WriteULong(FILE *,unsigned long,int);
int    ReadInt(FILE *,int *,int);
int    ReadUInt(FILE *,unsigned int *,int);
int    WriteInt(FILE *,int,int);
int    WriteUInt(FILE *,unsigned int,int);
int    ReadDouble(FILE *,double *,int);
int    WriteDouble(FILE *,double,int);
int    ReadFloat(FILE *,float *,int);
int    WriteFloat(FILE *,float,int);
int    ReadSkip(FILE *,int);
void   SkipUntil(FILE *,int);
void   StrToLower(char *);
void   StrToUpper(char *);
void   EchoFile(char *,FILE *);
int    IndexCopy(char *,char *,int,int);

/* MISC FUNCTIONS ---------------------------------------------------*/
int    ClipDouble(double *,double,double);
int    ClipInteger(int *,int,int);
void   SwapDouble(double *,double *);
void   SwapXYZ(XYZ *,XYZ *);
void   MinMaxXYZ(XYZ,XYZ *,XYZ *);

/* COLOUR STUFF -----------------------------------------------------*/
COLOUR GetColour(double,double,double,int);
int    AsciiColour(double,double,double);
double AbsColour(COLOUR);
void   ClipColour(COLOUR *);
void   InvertColour(COLOUR *);

/* POLYGON STUFF -----------------------------------------------------*/
void   PlaneParam(XYZ *,double *,double *,double *,double *);
int    PointIn3DPolygon(int,XYZ *,XYZ,double *);
int    WhichSide(double,double,double,double,double,double);
double PlanePoint(double,double,XYZ,XYZ,XYZ);
int    InTriangle(double,double,XYZ,XYZ,XYZ);
double PolygonLength(XYZ *,int);
double PolygonCurve(XYZ *,int);
int    PolygoniseCube(GRIDCELL,double,TRIANGLE *);
int    PolygoniseTri(GRIDCELL,double,TRIANGLE *,int,int,int,int);
int    PolygonCompare(POLY34 *,POLY34 *);

/* GEOMETRIC OUTPUT --------------------------------------------------*/
void   WriteRadFacet(FILE *,XYZ *,int,char *);
void   WriteDXFHeader(FILE *);
void   WriteDXFFooter(FILE *);
void   WriteDXFFacet(FILE *,XYZ *,int);

/* SYSTEM SPECIFIC  --------------------------------------------------*/
double GetRunTime(void);

#endif /* PAULSLIB_H */
