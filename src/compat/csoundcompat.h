#ifndef FAKECSOUND_H
#define FAKECSOUND_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
    sysdep.h:

    Copyright (C) 1991 Barry Vercoe, John ffitch

    This file is part of Csound.

    The Csound Library is free software; you can redistribute it
    and/or modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    Csound is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with Csound; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
    02110-1301 USA
*/

typedef int_least32_t int32;
typedef int_least16_t int16;
typedef uint_least32_t uint32;
typedef uint_least16_t uint16;

#define FL(x) ((MYFLT) (x))

#if defined(__clang__) ||  defined(HAVE_GCC3)
#  define LIKELY(x)     __builtin_expect(!!(x),1)
#  define UNLIKELY(x)   __builtin_expect(!!(x),0)
#else
#  define LIKELY(x)     x
#  define UNLIKELY(x)   x
#endif

#ifdef USE_DOUBLE
/* Defined here as Android does not have log2 functions */
#define MYRECIPLN2  1.442695040888963407359924681001892137426 /* 1.0/log(2) */
#define LOG2(a) (MYRECIPLN2*log(a))       /* floating point logarithm base 2 */

#define ACOS acos
#define ASIN asin
#define ATAN atan
#define ATAN2 atan2
#define COS cos
#define SIN sin
#define TAN tan
#define COSH cosh
#define SINH sinh
#define TANH tanh
#define ACOSH acosh
#define ASINH asinh
#define ATANH atanh
#define EXP exp
#define LOG log
#define LOG10 log10
/* #define LOG2 log2 */
#define POWER pow
#define SQRT sqrt
#define HYPOT hypot
#define FABS fabs
#define FLOOR floor
#define CEIL ceil
#define FMOD fmod
#define MODF modf
#else
/* Defined here as Android does not have log2 functions */
#define MYRECIPLN2  1.442695040888963407359924681001892137426 /* 1.0/log(2) */
#define LOG2(a) (MYRECIPLN2*logf(a))       /* floating point logarithm base 2 */

#define ACOS acosf
#define ASIN asinf
#define ATAN atanf
#define ATAN2 atan2f
#define COS cosf
#define SIN sinf
#define TAN tanf
#define COSH coshf
#define SINH sinhf
#define TANH tanhf
#define ACOSH acoshf
#define ASINH asinhf
#define ATANH atanhf
#define EXP expf
#define LOG logf
#define LOG10 log10f
/* #define LOG2 log2f */
#define POWER powf
#define SQRT sqrtf
#define HYPOT hypotf
#define FABS(x) fabsf(FL(x))
#define FLOOR floorf
#define CEIL ceilf
#define FMOD fmodf
#define MODF modff
#endif

#if defined(__BUILDING_LIBCSOUND) || defined(CSOUND_CSDL_H)

/* macros for converting floats to integers */
/* MYFLT2LONG: converts with unspecified rounding */
/* MYFLT2LRND: rounds to nearest integer */

#ifdef USE_LRINT
#  ifndef USE_DOUBLE
#    define MYFLT2LONG(x) (x > LONG_MIN && x < LONG_MAX ? \
                           (int32) lrintf((float) (x)) : 0)
#    define MYFLT2LRND(x) (x > LONG_MIN && x < LONG_MAX ? \
                           (int32) lrintf((float) (x)) : 0)
#  else
#    define MYFLT2LONG(x) (x > LONG_MIN && x < LONG_MAX ? \
                           (int32) lrint((double) (x)) : 0)
#    define MYFLT2LRND(x) (x > LONG_MIN && x < LONG_MAX ? \
                           (int32) lrint((double) (x)) : 0)
#  endif
#elif defined(MSVC)
#include <emmintrin.h>
#  ifndef USE_DOUBLE
// From Agner Fog optimisation manuals p.144
static inline int MYFLT2LONG(float const x)
{
	return _mm_cvtss_si32(_mm_load_ss(&x));
}

static inline int MYFLT2LRND(float const x)
{
	return _mm_cvtss_si32(_mm_load_ss(&x));
}

#  else
static inline int MYFLT2LONG(double const x)
{
	return _mm_cvtsd_si32(_mm_load_sd(&x));
}

static inline int MYFLT2LRND(double const x)
{
	return _mm_cvtsd_si32(_mm_load_sd(&x));
}
#  endif
#else
#  ifndef USE_DOUBLE
#    define MYFLT2LONG(x) ((int32) (x))
#    if defined(HAVE_GCC3) && defined(__i386__) && !defined(__ICC)
#      define MYFLT2LRND(x) ((int32) lrintf((float) (x)))
#    else
static inline int32 MYFLT2LRND(float fval)
{
	return ((int32)(fval + (fval < 0.0f ? -0.5f : 0.5f)));
}
#    endif
#  else
#    define MYFLT2LONG(x) ((int32) (x))
#    if defined(HAVE_GCC3) && defined(__i386__) && !defined(__ICC)
#      define MYFLT2LRND(x) ((int32) lrint((double) (x)))
#    else
static inline int32 MYFLT2LRND(double fval)
{
	return ((int32)(fval + (fval < 0.0 ? -0.5 : 0.5)));
}
#    endif
#  endif
#endif
#endif

/*
    csoundCore.h:

    Copyright (C) 1991-2006 Barry Vercoe, John ffitch, Istvan Varga

    This file is part of Csound.

    The Csound Library is free software; you can redistribute it
    and/or modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    Csound is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with Csound; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
    02110-1301 USA
*/

#ifndef __MYFLT_DEF
#define __MYFLT_DEF
#ifndef USE_DOUBLE
#define MYFLT float
#else
#define MYFLT double
#endif
#endif

#define LOG10D20        0.11512925              /* for db to ampfac   */
#define DV32768         FL(0.000030517578125)

#ifndef PI
#define PI      (3.141592653589793238462643383279502884197)
#endif /* pi */
#define TWOPI   (6.283185307179586476925286766559005768394)
#define HALFPI  (1.570796326794896619231321691639751442099)
#define PI_F    ((MYFLT) PI)
#define TWOPI_F ((MYFLT) TWOPI)
#define HALFPI_F ((MYFLT) HALFPI)
#define INF     (2147483647.0)
#define ROOT2   (1.414213562373095048801688724209698078569)

#define MAXPOS     0x7FFFFFFFL

#define DFLT_DBFS (FL(1.0))

typedef struct auxch {
	size_t  size;
	void *auxp, *endp;
} AUXCH;

void AuxAlloc(size_t nbytes, AUXCH *auxchp);
void AuxDealloc(AUXCH *auxchp);

/*
    csound.h:

    Copyright (C) 2003 2005 2008 2013 by John ffitch, Istvan Varga,
					 Mike Gogins, Victor Lazzarini,
					 Andres Cabrera, Steven Yi

    This file is part of Csound.

    The Csound Library is free software; you can redistribute it
    and/or modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    Csound is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with Csound; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
    02110-1301 USA
*/

#if (defined(WIN32) || defined(_WIN32)) && !defined(SWIG)
#  if defined(__BUILDING_LIBCSOUND)
#    define BSOUND_API	__declspec(dllexport)
#  else
#    define BSOUND_API	__declspec(dllimport)
#  endif
#elif defined(__GNUC__) && (__GNUC__ >= 4) /* && !defined(__MACH__) */
#  define BSOUND_API	__attribute__ ( (visibility("default")) )
#else
#  define BSOUND_API
#endif

/*
    interlocks.h:

    Copyright (C) 2011 John ffitch

    This file is part of Csound.

    The Csound Library is free software; you can redistribute it
    and/or modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    Csound is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with Csound; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
    02110-1301 USA
*/

// ZAK
#define ZR (0x0001)
#define ZW (0x0002)
#define ZB (0x0003)

// Writes to inputs
#define WI (0x0004)

//Tables
#define TR (0x0008)
#define TW (0x0010)
#define TB (0x0018)

//Channels
#define _CR (0x0020)
#define _CW (0x0040)
#define _CB (0x0060)

//Stack
#define SK (0x0080)

//Printing
#define WR (0x0100)

// Internal oddities -- SPOUT
#define IR (0x0200)
#define IW (0x0400)
#define IB (0x0600)

//Deprecated
#define _QQ (0x8000)

#endif
