
/* @(#)e_sinh.c 1.3 95/01/18 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

/* FDLIBM___ieee754_sinh(x)
 * Method : 
 * mathematically FDLIBM_sinh(x) if defined to be (FDLIBM_exp(x)-FDLIBM_exp(-x))/2
 *	1. Replace x by |x| (FDLIBM_sinh(-x) = -FDLIBM_sinh(x)). 
 *	2. 
 *		                                    E + E/(E+1)
 *	    0        <= x <= 22     :  FDLIBM_sinh(x) := --------------, E=FDLIBM_expm1(x)
 *			       			        2
 *
 *	    22       <= x <= lnovft :  FDLIBM_sinh(x) := FDLIBM_exp(x)/2 
 *	    lnovft   <= x <= ln2ovft:  FDLIBM_sinh(x) := FDLIBM_exp(x/2)/2 * FDLIBM_exp(x/2)
 *	    ln2ovft  <  x	    :  FDLIBM_sinh(x) := x*shuge (overflow)
 *
 * Special cases:
 *	FDLIBM_sinh(x) is |x| if x is +INF, -INF, or NaN.
 *	only FDLIBM_sinh(0)=0 is exact for finite x.
 */

#include "fdlibm.h"

#ifdef __STDC__
static const double one = 1.0, shuge = 1.0e307;
#else
static double one = 1.0, shuge = 1.0e307;
#endif

#ifdef __STDC__
	double FDLIBM___ieee754_sinh(double x)
#else
	double FDLIBM___ieee754_sinh(x)
	double x;
#endif
{	
	double t,w,h;
	int ix,jx;
	unsigned lx;

    /* High word of |x|. */
	jx = __HI(x);
	ix = jx&0x7fffffff;

    /* x is INF or NaN */
	if(ix>=0x7ff00000) return x+x;	

	h = 0.5;
	if (jx<0) h = -h;
    /* |x| in [0,22], return sign(x)*0.5*(E+E/(E+1))) */
	if (ix < 0x40360000) {		/* |x|<22 */
	    if (ix<0x3e300000) 		/* |x|<2**-28 */
		if(shuge+x>one) return x;/* FDLIBM_sinh(tiny) = tiny with inexact */
	    t = FDLIBM_expm1(FDLIBM_fabs(x));
	    if(ix<0x3ff00000) return h*(2.0*t-t*t/(t+one));
	    return h*(t+t/(t+one));
	}

    /* |x| in [22, FDLIBM_log(maxdouble)] return 0.5*FDLIBM_exp(|x|) */
	if (ix < 0x40862E42)  return h*FDLIBM___ieee754_exp(FDLIBM_fabs(x));

    /* |x| in [FDLIBM_log(maxdouble), overflowthresold] */
	lx = *( (((*(unsigned*)&one)>>29)) + (unsigned*)&x);
	if (ix<0x408633CE || (ix==0x408633ce)&&(lx<=(unsigned)0x8fb9f87d)) {
	    w = FDLIBM___ieee754_exp(0.5*FDLIBM_fabs(x));
	    t = h*w;
	    return t*w;
	}

    /* |x| > overflowthresold, FDLIBM_sinh(x) overflow */
	return x*shuge;
}