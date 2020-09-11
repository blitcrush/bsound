#include "bsound.h"
#include <stdlib.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
	LPVOID lpReserved)
{
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}
#endif

//
// reverbsc
//

CMPRS *bsound_compress_init(MYFLT *ar, MYFLT *aasig, MYFLT *acsig,
	uint32_t karstride, uint32_t kaastride, uint32_t kacstride,
	MYFLT kthresh, MYFLT kloknee, MYFLT khiknee, MYFLT kratio, MYFLT katt,
	MYFLT krls, MYFLT ilook, MYFLT e0dbfs, MYFLT samplerate)
{
	CMPRS *cmprs = (CMPRS *)calloc(1, sizeof(CMPRS));
	cmprs->ar = ar;
	cmprs->aasig = aasig;
	cmprs->acsig = acsig;
	cmprs->karstride = karstride;
	cmprs->kaastride = kaastride;
	cmprs->kacstride = kacstride;
	cmprs->kthresh = kthresh;
	cmprs->kloknee = kloknee;
	cmprs->khiknee = khiknee;
	cmprs->kratio = kratio;
	cmprs->katt = katt;
	cmprs->krls = krls;
	cmprs->ilook = ilook;
	cmprs->e0dbfs = e0dbfs;
	compress_init(samplerate, cmprs);
	return cmprs;
}

CMPRS *bsound_compress2_init(MYFLT *ar, MYFLT *aasig, MYFLT *acsig,
	uint32_t karstride, uint32_t kaastride, uint32_t kacstride,
	MYFLT kthresh, MYFLT kloknee, MYFLT khiknee, MYFLT kratio, MYFLT katt,
	MYFLT krls, MYFLT ilook, MYFLT e0dbfs, MYFLT samplerate)
{
	CMPRS *cmprs = (CMPRS *)calloc(1, sizeof(CMPRS));
	cmprs->ar = ar;
	cmprs->aasig = aasig;
	cmprs->acsig = acsig;
	cmprs->karstride = karstride;
	cmprs->kaastride = kaastride;
	cmprs->kacstride = kacstride;
	cmprs->kthresh = kthresh;
	cmprs->kloknee = kloknee;
	cmprs->khiknee = khiknee;
	cmprs->kratio = kratio;
	cmprs->katt = katt;
	cmprs->krls = krls;
	cmprs->ilook = ilook;
	cmprs->e0dbfs = e0dbfs;
	compress2_init(samplerate, cmprs);
	return cmprs;
}

void bsound_compress_perf(uint32_t num_samples, CMPRS *cmprs)
{
	compress_perf(num_samples, cmprs);
}

void bsound_compress_destroy(CMPRS *cmprs)
{
	AuxDealloc(&cmprs->auxch);
	free(cmprs);
}

SC_REVERB *bsound_reverbsc_init(MYFLT *aoutL, MYFLT *aoutR, MYFLT *ainL,
	MYFLT *ainR, uint32_t kaoutLStride, uint32_t kaoutRStride,
	uint32_t kainLStride, uint32_t kainRStride, MYFLT kFeedBack,
	MYFLT kLPFreq, MYFLT iSampleRate, MYFLT iPitchMod, double dampFact)
{
	SC_REVERB *sc_reverb = (SC_REVERB *)calloc(1, sizeof(SC_REVERB));
	sc_reverb->aoutL = aoutL;
	sc_reverb->aoutR = aoutR;
	sc_reverb->ainL = ainL;
	sc_reverb->ainR = ainR;
	sc_reverb->kaoutLStride = kaoutLStride;
	sc_reverb->kaoutRStride = kaoutRStride;
	sc_reverb->kainLStride = kainLStride;
	sc_reverb->kainRStride = kainRStride;
	sc_reverb->kFeedBack = kFeedBack;
	sc_reverb->kLPFreq = kLPFreq;
	sc_reverb->iSampleRate = iSampleRate;
	sc_reverb->iPitchMod = iPitchMod;
	sc_reverb->dampFact = dampFact;
	sc_reverb_init(sc_reverb);
	return sc_reverb;
}

void bsound_reverbsc_perf(uint32_t num_samples, SC_REVERB *reverbsc)
{
	sc_reverb_perf(num_samples, reverbsc);
}

void bsound_reverbsc_destroy(SC_REVERB *reverbsc)
{
	AuxDealloc(&reverbsc->auxData);
	free(reverbsc);
}
