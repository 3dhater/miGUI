/* @(#)s_asinh.c 5.1 93/09/24 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#include "CRT.h"

/* asinh(x)
 * Method :
 *	Based on
 *		asinh(x) = sign(x) * log [ |x| + sqrt(x*x+1) ]
 *	we have
 *	asinh(x) := x  if  1+x*x=1,
 *		 := sign(x)*(log(x)+ln2)) for large |x|, else
 *		 := sign(x)*log(2|x|+1/(|x|+sqrt(x*x+1))) if|x|>2, else
 *		 := sign(x)*log1p(|x| + x^2/(1 + sqrt(1+x^2)))
 */

#include "float.h"

#include "math.h"
#include "math_private.h"

static const double
one =  1.00000000000000000000e+00, /* 0x3FF00000, 0x00000000 */
ln2 =  6.93147180559945286227e-01, /* 0x3FE62E42, 0xFEFA39EF */
huge=  1.00000000000000000000e+300;

double _C_DECL __CRT_ieee754_log(double x);
float _C_DECL __CRT_ieee754_logf(float x);
double _C_DECL __CRT_ieee754_sqrt(double x);
float _C_DECL __CRT_ieee754_sqrtf(float x);

double
_C_DECL
__CRT_asinh(double x)
{
	double t,w;
	int32_t hx,ix;
	GET_HIGH_WORD(hx,x);
	ix = hx&0x7fffffff;
	if(ix>=0x7ff00000) return x+x;	/* x is inf or NaN */
	if(ix< 0x3e300000) {	/* |x|<2**-28 */
	    if(huge+x>one) return x;	/* return x inexact except 0 */
	}
	if(ix>0x41b00000) {	/* |x| > 2**28 */
	    w = __CRT_ieee754_log(fabs(x))+ln2;
	} else if (ix>0x40000000) {	/* 2**28 > |x| > 2.0 */
	    t = fabs(x);
	    w = __CRT_ieee754_log(2.0*t+one/(__CRT_ieee754_sqrt(x*x+one)+t));
	} else {		/* 2.0 > |x| > 2**-28 */
	    t = x*x;
	    w =log1p(fabs(x)+t/(one+ __CRT_ieee754_sqrt(one+t)));
	}
	if(hx>0) return w; else return -w;
}

#pragma warning(push)
#pragma warning(disable: 4305)
static const float
ln2_f = 6.9314718246e-01, /* 0x3f317218 */
huge_f = 1.0000000000e+30;
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable: 4244)
float
_C_DECL
__CRT_asinhf(float x)
{
	float t, w;
	int32_t hx, ix;
	GET_FLOAT_WORD(hx, x);
	ix = hx & 0x7fffffff;
	if (ix >= 0x7f800000) return x + x;	/* x is inf or NaN */
	if (ix < 0x31800000) {	/* |x|<2**-28 */
		if (huge_f + x > one) return x;	/* return x inexact except 0 */
	}
	if (ix > 0x4d800000) {	/* |x| > 2**28 */
		w = __CRT_ieee754_logf(fabsf(x)) + ln2_f;
	}
	else if (ix > 0x40000000) {	/* 2**28 > |x| > 2.0 */
		t = fabsf(x);
		w = __CRT_ieee754_logf((float)2.0 * t + one / (__CRT_ieee754_sqrtf(x * x + one) + t));
	}
	else {		/* 2.0 > |x| > 2**-28 */
		t = x * x;
		w = log1pf(fabsf(x) + t / (one + __CRT_ieee754_sqrtf(one + t)));
	}
	if (hx > 0) return w; else return -w;
}
#pragma warning(pop)

long double
_C_DECL
__CRT_asinhl(long double x)
{
	return __CRT_asinh((double)x);
}