#include "csoundcompat.h"

void AuxAlloc(size_t nbytes, AUXCH *auxchp)
{
	auxchp->size = nbytes;
	auxchp->auxp = calloc(nbytes, 1);
	auxchp->endp = (char *)auxchp->auxp + nbytes;
}

void AuxDealloc(AUXCH *auxchp)
{
	auxchp->size = 0;
	if (auxchp->auxp != NULL) {
		free(auxchp->auxp);
	}
	auxchp->endp = NULL;
}
