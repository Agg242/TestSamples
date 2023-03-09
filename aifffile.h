#ifndef AIFFFILE_H__
#define AIFFFILE_H__

#include <stdio.h>
#include <exec/types.h>


/* SampleFrame = numChannels * (sampleSize / 8) 
   data size = SampleFrame * numSampleFrames */
typedef struct commchunk_t {
    UWORD numChannels;
    ULONG numSampleFrames;   
    UWORD sampleSize;    // bits per sample
    // lazy extented value extract:
    UWORD sr_exponent;
    UWORD sr_mantissa_hi; // in fact contains the value
    ULONG sr_mantissa_mid;
    UWORD sr_mantissa_lo;    
} CommChunk;

typedef struct aifffile_t {
    CommChunk comm;
    ULONG data_size;
    ULONG sampleRate;
} AiffFile;


extern BOOL read_aiff_header(FILE *file, AiffFile *aiff);
extern BOOL read_aiff_data(FILE *file, AiffFile *aiff, APTR buffer);

#endif
