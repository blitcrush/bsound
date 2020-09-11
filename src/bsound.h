#ifndef BSOUND_H
#define BSOUND_H

#include "compat/csoundcompat.h"
#include "opcodes/compress.h"
#include "opcodes/reverbsc.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // Prevent MSVC's auto-format from indenting this block
#endif
#endif

//
// compress
//

BSOUND_API CMPRS *bsound_compress_init(MYFLT *ar, MYFLT *aasig, MYFLT *acsig,
	uint32_t karstride, uint32_t kaastride, uint32_t kacstride,
	MYFLT kthresh, MYFLT kloknee, MYFLT khiknee, MYFLT kratio, MYFLT katt,
	MYFLT krls, MYFLT ilook, MYFLT e0dbfs, MYFLT samplerate);
BSOUND_API CMPRS *bsound_compress2_init(MYFLT *ar, MYFLT *aasig, MYFLT *acsig,
	uint32_t karstride, uint32_t kaastride, uint32_t kacstride,
	MYFLT kthresh, MYFLT kloknee, MYFLT khiknee, MYFLT kratio, MYFLT katt,
	MYFLT krls, MYFLT ilook, MYFLT e0dbfs, MYFLT samplerate);
BSOUND_API void bsound_compress_perf(uint32_t num_samples, CMPRS *cmprs);
BSOUND_API void bsound_compress_destroy(CMPRS *cmprs);

//
// reverbsc
//

BSOUND_API SC_REVERB *bsound_reverbsc_init(MYFLT *aoutL, MYFLT *aoutR,
	MYFLT *ainL, MYFLT *ainR, uint32_t kaoutLStride, uint32_t kaoutRStride,
	uint32_t kainLStride, uint32_t kainRStride, MYFLT kFeedBack,
	MYFLT kLPFreq, MYFLT iSampleRate, MYFLT iPitchMod, double dampFact);
BSOUND_API void bsound_reverbsc_perf(uint32_t num_samples, SC_REVERB *reverbsc);
BSOUND_API void bsound_reverbsc_destroy(SC_REVERB *reverbsc);

#ifdef __cplusplus
}
#endif

#endif
