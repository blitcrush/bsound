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

#include "compress.h"

void compress_init(MYFLT samplerate, CMPRS *p)
{
	int32    delsmps;

	p->onedsr = FL(1.0) / samplerate;
	p->thresh = (MYFLT)MAXPOS;
	p->loknee = (MYFLT)MAXPOS;                 /* force reinits        */
	p->hiknee = (MYFLT)MAXPOS;
	p->ratio = (MYFLT)MAXPOS;
	p->curatt = (MYFLT)MAXPOS;
	p->currls = (MYFLT)MAXPOS;
	/* round to nearest integer */
	if (UNLIKELY((delsmps = MYFLT2LONG(p->ilook * samplerate)) <= 0L))
		delsmps = 1L;                             /* alloc 2 delay bufs   */
	AuxAlloc(delsmps * 2 * sizeof(MYFLT), &p->auxch);
	p->abuf = (MYFLT *)p->auxch.auxp;
	p->cbuf = p->abuf + delsmps;                /*   for asig & csig    */
	p->clim = p->cbuf + delsmps;
	p->aptr = p->abuf;
	p->cptr = p->cbuf;
	p->lmaxp = p->clim - 1;
	p->lmax = FL(0.0);
	p->cenv = 0.0;
	p->newenv = 0;
	p->bias = FL(0.0);
}

/* compress2 is compress but with dB inputs in range [-90,0] rather
   than [0.90], by setting p->bias valuex -- JPff */
void compress2_init(MYFLT samplerate, CMPRS *p)
{
	compress_init(samplerate, p);
	p->bias = FL(90.0);
}

void compress_perf(uint32_t nsmps, CMPRS *p)
{
	MYFLT *ar, *ainp, *cinp;
	uint32_t n;

	/* VL: scale by 0dbfs, code is tuned to work in 16bit range */
	MYFLT scal = FL(32768.0) / p->e0dbfs;

	if (p->kthresh != p->thresh) {             /* check for changes:   */
		p->thresh = p->kthresh;
		p->envthrsh = (MYFLT)exp((p->thresh + p->bias) * LOG10D20);
	}
	if (p->kloknee != p->loknee ||
		p->khiknee != p->hiknee ||
		p->kratio != p->ratio) {
		MYFLT ratio, K;
		p->loknee = p->kloknee;
		p->hiknee = p->khiknee;
		p->ratio = p->kratio;
		p->envlo = (MYFLT)exp((p->loknee + p->bias) * LOG10D20);
		if ((p->kneespan = p->hiknee - p->loknee) < FL(0.0))
			p->kneespan = FL(0.0);
		if ((ratio = p->ratio) < FL(0.01))         /* expand max is 100 */
			ratio = FL(0.01);
		K = (MYFLT)LOG10D20 * (FL(1.0) - ratio) / ratio;
		p->ratcoef = K;                            /* rat down per db */
		if (p->kneespan > FL(0.0)) {
			p->kneecoef = K * FL(0.5) / p->kneespan; /* y = x - (K/2span)x*x */
			p->kneemul = (MYFLT)exp(p->kneecoef * p->kneespan * p->kneespan);
		} else
			p->kneemul = FL(1.0);
	}
	if (p->katt != p->curatt) {
		if ((p->curatt = p->katt) < p->onedsr)
			p->c2 = 0.0;
		else
			p->c2 = pow(0.5, p->onedsr / p->curatt);
		p->c1 = 1.0 - p->c2;
	}
	if (p->krls != p->currls) {
		if ((p->currls = p->krls) < p->onedsr)
			p->d2 = 0.0;
		else
			p->d2 = pow(0.5, p->onedsr / p->currls);
		p->d1 = 1.0 - p->d2;
	}
	ar = p->ar;
	ainp = p->aasig;
	cinp = p->acsig;
	for (n = 0; n < nsmps; n++) {   /* now for each sample of both inputs:  */
		MYFLT asig, lsig;
		double csig;
		asig = *p->aptr;                  /* get signals from delay line  */
		csig = *p->cptr;
		*p->aptr = ainp[n * p->kaastride] * scal;               /*   & replace with incoming    */
		lsig = FABS(cinp[n * p->kacstride] * scal);
		//lsig = -lsig;                   /*   made abs for control       */
		*p->cptr = lsig;
		if (p->cptr == p->lmaxp) {        /* if prev ctrl was old lamax   */
			MYFLT *lap, newmax = FL(0.0);
			for (lap = p->cptr + 1; lap < p->clim; lap++)
				if (*lap >= newmax) {
					newmax = *lap;              /*   find next highest abs      */
					p->lmaxp = lap;
				}
			for (lap = p->cbuf; lap <= p->cptr; lap++)
				if (*lap >= newmax) {
					newmax = *lap;              /*   in lkahd circular cbuf     */
					p->lmaxp = lap;
				}
			p->lmax = newmax;
		} else if (lsig >= p->lmax) {       /* else keep lkahd max & adrs   */
			p->lmax = lsig;                 /*   on arrival                 */
			p->lmaxp = p->cptr;
		}
		if (csig > p->cenv)               /* follow a rising csig env     */
			p->cenv = p->c1 * csig + p->c2 * p->cenv;
		else if (p->cenv > p->lmax)       /* else if env above lookahead  */
			p->cenv = p->d1 * csig + p->d2 * p->cenv;    /*  apply release  */
		else goto lvlchk;
		p->newenv = 1;

lvlchk:
		if (p->cenv > p->envlo) {         /* if env exceeds loknee amp    */
			if (p->newenv) {                /*   calc dbenv & ampmul        */
				double dbenv, excess;
				p->newenv = 0;
				dbenv = log(p->cenv + 0.001) / LOG10D20;      /* for softknee */
				if ((excess = dbenv - (p->loknee + p->bias)) < p->kneespan)
					p->ampmul = exp(p->kneecoef * excess * excess);
				else {
					excess -= p->kneespan;      /* or ratio line */
					p->ampmul = p->kneemul * exp(p->ratcoef * excess);
				}
			}
			asig *= (MYFLT)p->ampmul;       /* and compress the asig */
		} else if (p->cenv < p->envthrsh)
			asig = FL(0.0);                 /* else maybe noise gate */
		ar[n * p->karstride] = asig / scal;
		if (++p->aptr >= p->cbuf) {
			p->aptr = p->abuf;
			p->cptr = p->cbuf;
		} else p->cptr++;
	}
}
