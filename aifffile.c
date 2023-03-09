#include "aifffile.h"
#include <stdio.h>
#include "dbg.h"
#include "chunkutil.h"


#define STD_EXPONENT 16398

BOOL read_aiff_header(FILE *file, AiffFile *aiff)
{
    BOOL res = FALSE;
    ULONG size, chunk_size;
    
    check(file != NULL, "Null file pointer");
    check(aiff != NULL, "Null AiffFile pointer");
    
    size = get_chunk_size("FORM", file);
    check(size != 0, "Read aborted");

    check(check_id("AIFF", file) == TRUE, "Aiff read aborted");    

    size = get_chunk_size("COMM", file);
    check(size != 0, "Read aborted");
    
    size = fread(&(aiff->comm), sizeof(CommChunk), 1, file);
    check(size == 1, "Error reading COMM chunk");

    if(STD_EXPONENT > aiff->comm.sr_exponent)
    {
        aiff->sampleRate = aiff->comm.sr_mantissa_hi >> (STD_EXPONENT - aiff->comm.sr_exponent);
    }
    else
    {
        aiff->sampleRate = aiff->comm.sr_mantissa_hi << (aiff->comm.sr_exponent - STD_EXPONENT);
    }

#ifdef DEBUG
    printf("  numChannels: %hu\n", aiff->comm.numChannels);
    printf("  numSampleFrames: %lu\n", aiff->comm.numSampleFrames);   
    printf("  sampleSize: %hu\n", aiff->comm.sampleSize);    // bits per sample
    printf("  sr_exponent: %hu\n", aiff->comm.sr_exponent);
    printf("  sr_mantissa_hi: %hu\n", aiff->comm.sr_mantissa_hi); // in fact contains the value
    printf("  sr_mantissa_mid: %lu\n", aiff->comm.sr_mantissa_mid);
    printf("  sr_mantissa_lo: %hu\n", aiff->comm.sr_mantissa_lo);    
    printf("  sample rate: %lu\n\n", aiff->sampleRate);
#endif

    size = get_chunk_size("SSND", file);
    check(size != 0, "Read aborted");
    aiff->data_size = size - (2 * sizeof(ULONG)); // skip offset and blocksize fields
    fseek(file, 2 * sizeof(ULONG), SEEK_CUR);

    res = TRUE;

on_error:
    return res;
}


BOOL read_aiff_data(FILE *file, AiffFile *aiff, APTR buffer)
{
    size_t size;
    
    check(file != NULL, "Null file pointer");
    check(aiff != NULL, "Null AiffFile pointer");
    check(buffer != NULL, "Null buffer pointer");
    
    size = fread(buffer, 1, aiff->data_size, file);
    check(size == aiff->data_size, "Unable to read all data (%ld/%ld)", size, aiff->data_size);
    
    return TRUE;
    
on_error:
    return FALSE;
}
