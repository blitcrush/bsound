/*
    compress.c:

    Copyright (C) 2006 by Barry Vercoe; adapted by John ffitch

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

#ifndef COMPRESS_H
#define COMPRESS_H

#include "../compat/csoundcompat.h"

typedef struct {
	MYFLT *ar, *aasig, *acsig;
	uint32_t karstride, kaastride, kacstride;
	MYFLT kthresh, kloknee, khiknee, kratio, katt, krls, ilook;
	MYFLT e0dbfs;

	MYFLT   thresh, loknee, hiknee, ratio, curatt, currls;
	MYFLT   envthrsh, envlo, kneespan, kneemul, kneecoef, ratcoef;
	double  cenv, c1, c2, d1, d2, ampmul;
	MYFLT *abuf, *cbuf, *aptr, *cptr, *clim, lmax, *lmaxp;
	int32   newenv;
	AUXCH   auxch;
	MYFLT   bias;
	MYFLT onedsr;
} CMPRS;

void compress_init(MYFLT samplerate, CMPRS *p);
void compress2_init(MYFLT samplerate, CMPRS *p);
void compress_perf(uint32_t nsmps, CMPRS *p);

#endif
