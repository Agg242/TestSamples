#include "8svxfile.h"
#include <stdio.h>
#include "dbg.h"
#include "chunkutil.h"


BOOL read_8svx_header(FILE *file, A8svxFile *a8svx)
{
    BOOL res = FALSE;
    ULONG size, chunk_size;
    
    check(file != NULL, "Null file pointer");
    check(a8svx != NULL, "Null A8svxFile pointer");
    
    size = get_chunk_size("FORM", file);
    check(size != 0, "Read aborted");

    check(check_id("8SVX", file) == TRUE, "8svx read aborted");    

    size = get_chunk_size("VHDR", file);
    check(size != 0, "Read aborted");
    
    size = fread(&(a8svx->vhdr), sizeof(VhdrChunk), 1, file);
    check(size == 1, "Error reading VHDR chunk");

#ifdef DEBUG
    printf("oneShotHiSamples: %lu\n", a8svx->vhdr.oneShotHiSamples);
    printf("repeatHiSamples: %lu\n", a8svx->vhdr.repeatHiSamples);
    printf("samplesPerHiCycle: %lu\n", a8svx->vhdr.samplesPerHiCycle);
    printf("samplesPerSec: %hu\n", a8svx->vhdr.samplesPerSec);
    printf("ctOctave: %hhu\n", a8svx->vhdr.ctOctave);
    printf("sCompression: %hhu\n", a8svx->vhdr.sCompression);
    printf("volume (fixed): %lx\n\n", a8svx->vhdr.volume);
#endif

    a8svx->data_size = get_chunk_size("BODY", file);
    check(a8svx->data_size != 0, "Read aborted");

    res = TRUE;

on_error:
    return res;
	
}


BOOL read_8svx_data(FILE *file, A8svxFile *a8svx, APTR buffer)
{
    size_t size;
    
    check(file != NULL, "Null file pointer");
    check(a8svx != NULL, "Null AiffFile pointer");
    check(buffer != NULL, "Null buffer pointer");
    
    size = fread(buffer, 1, a8svx->data_size, file);
    check(size == a8svx->data_size, "Unable to read all data (%ld/%ld)", size, a8svx->data_size);
    
    return TRUE;
    
on_error:
    return FALSE;
}

