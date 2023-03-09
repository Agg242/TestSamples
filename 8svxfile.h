#ifndef A8SVXFILE_H__
#define A8SVXFILE_H__

#include <stdio.h>
#include <exec/types.h>


#define sCmpNone       0    /* not compressed                        */
#define sCmpFibDelta   1    /* Fibonacci-delta encoding (Appendix C) */

typedef struct vhdrchunk_t {
	ULONG oneShotHiSamples;
	ULONG repeatHiSamples;
	ULONG samplesPerHiCycle;
	UWORD samplesPerSec;
	UBYTE ctOctave;
	UBYTE sCompression;
	ULONG volume;
} VhdrChunk;

typedef struct a8svxfile_t {
    VhdrChunk vhdr;
    ULONG data_size;
} A8svxFile;


extern BOOL read_8svx_header(FILE *file, A8svxFile *a8svx);
extern BOOL read_8svx_data(FILE *file, A8svxFile *a8svx, APTR buffer);

#endif
