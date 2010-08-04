/*
 * This routine makes nice labels. It is taken from
 * Graphic Gems. The routine is transform from original
 * that way that it can work with color3d routines which
 * draw labels on the graphs.
 * 
 * Author: Toni Ivas     28.12.2001
 */
 
/*
 * Nice Numbers for Graph Labels
 * by Paul Heckbert
 * from "Graphics Gems", Academic Press, 1990
 */

/*
 * label.c: demonstrate nice graph labeling
 *
 * Paul Heckbert	2 Dec 88
 */

#include <stdio.h>
#include <math.h>
#include "grafik.h"
/*#include "GraphicsGems.h"*/

double nicenum();
void loose_label(double min, double max, char** labels, int *nticks);
/* expt(a,n)=a^n for integer n */

#ifdef POW_NOT_TRUSTWORTHY
/* if roundoff errors in pow cause problems, use this: */

double expt(a, n)
double a;
register int n;
{
    double x;

    x = 1.;
    if (n>0) for (; n>0; n--) x *= a;
    else for (; n<0; n++) x /= a;
    return x;
}

#else
#   define expt(a, n) pow(a, (double)(n))
#endif


void nice_label(double min, double max, char** labels, int *nticks)
{
  loose_label(min, max, labels, nticks);
}

/*
 * loose_label: demonstrate loose labeling of data range from min to max.
 * (tight method is similar)
 */

void loose_label(double min, double max, char** labels, int *nticks)
{
    char str[6], temp[20];
    int nfrac;
    int i=0;
    double d;				/* tick mark spacing */
    double graphmin, graphmax;		/* graph range min and max */
    double range, x;

    /* we expect min!=max */
    range = nicenum(max - min, 0);
    d = nicenum(range/(*nticks-1), 1);
    graphmin = floor(min/d)*d;
    graphmax = ceil(max/d)*d;
    nfrac = MAX(-floor(log10(d)), 0);	/* # of fractional digits to show */
    sprintf(str, "%%.%df", nfrac);	/* simplest axis labels */
    /*printf("graphmin=%g graphmax=%g increment=%g\n", graphmin, graphmax, d);*/
    for (x=graphmin; x<graphmax+.5*d; x+=d) {
	sprintf(temp, str, x);
	/*print the values in array of labels*/
	strcpy(labels[i], temp);
	*nticks = i++;
    }
}

/*
 * nicenum: find a "nice" number approximately equal to x.
 * Round the number if round=1, take ceiling if round=0
 */

double nicenum(double x, int round)
{
    int expv;				/* exponent of x */
    double f;				/* fractional part of x */
    double nf;				/* nice, rounded fraction */

    expv = floor(log10(x));
    f = x/expt(10., expv);		/* between 1 and 10 */
    if (round)
	if (f<1.5) nf = 1.;
	else if (f<3.) nf = 2.;
	else if (f<7.) nf = 5.;
	else nf = 10.;
    else
	if (f<=1.) nf = 1.;
	else if (f<=2.) nf = 2.;
	else if (f<=5.) nf = 5.;
	else nf = 10.;
    return nf*expt(10., expv);
}







