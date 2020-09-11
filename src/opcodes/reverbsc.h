/*
    reverbsc.c:

    Copyright 1999, 2005 Sean Costello and Istvan Varga

    8 delay line FDN reverb, with feedback matrix based upon
    physical modeling scattering junction of 8 lossless waveguides
    of equal characteristic impedance. Based on Julius O. Smith III,
    "A New Approach to Digital Reverberation using Closed Waveguide
    Networks," Proceedings of the International Computer Music
    Conference 1985, p. 47-53 (also available as a seperate
    publication from CCRMA), as well as some more recent papers by
    Smith and others.

    Csound orchestra version coded by Sean Costello, October 1999

    C implementation (C) 2005 Istvan Varga

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

#ifndef REVERBSC_H
#define REVERBSC_H

#include "../compat/csoundcompat.h"

typedef struct _reverbsc_delayLine {
	int32_t         writePos;
	int32_t         bufferSize;
	int32_t         readPos;
	int32_t         readPosFrac;
	int32_t         readPosFrac_inc;
	int32_t         dummy;
	int32_t         seedVal;
	int32_t         randLine_cnt;
	double      filterState;
	MYFLT       buf[1];
} reverbsc_delayLine;

typedef struct {
	MYFLT *aoutL, *aoutR, *ainL, *ainR;
	uint32_t kaoutLStride, kaoutRStride, kainLStride, kainRStride;
	MYFLT kFeedBack, kLPFreq, iSampleRate, iPitchMod, iSkipInit;
	double      sampleRate;
	double      dampFact;
	MYFLT       prv_LPFreq;
	int32_t         initDone;
	reverbsc_delayLine *delayLines[8];
	AUXCH       auxData;
} SC_REVERB;

void sc_reverb_init(SC_REVERB *p);
void sc_reverb_perf(uint32_t nsmps, SC_REVERB *p);

#endif
