#include "wavefile.h"
#include <stdio.h>
#include "dbg.h"
#include "chunkutil.h"


void switch_fmt_endianness(FmtChunk *fmt);


BOOL read_wav_header(FILE *file, WavFile *wav)
{
    BOOL res = FALSE;
    ULONG size, chunk_size;
    
    check(file != NULL, "Null file pointer");
    check(wav != NULL, "Null WaveFile pointer");
    
    size = get_chunk_size("RIFF", file);
    check(size != 0, "Read aborted");

    check(check_id("WAVE", file) == TRUE, "Wav read aborted");    
    
    chunk_size = get_chunk_size("fmt ", file);
    chunk_size = SWAP_LONG(chunk_size);
    check(chunk_size >= sizeof(FmtChunk), "fmt chunk too small, read aborted");

    size = fread(&(wav->fmt), sizeof(FmtChunk), 1, file);
    check(size == 1, "Error reading fmt chunk");
    switch_fmt_endianness(&(wav->fmt));
    
    if(chunk_size > sizeof(FmtChunk))
    {
        fseek(file, chunk_size - sizeof(FmtChunk), SEEK_CUR);
    }
    
    chunk_size = get_chunk_size("data", file);
    check(chunk_size != 0, "Read aborted");
    wav->data_size = SWAP_LONG(chunk_size);
    
    res = TRUE;
    
on_error:
    return res;
}


BOOL read_wav_data(FILE *file, WavFile *wav, APTR buffer)
{
    size_t i;
    UWORD *data = buffer;
    
    check(file != NULL, "Null file pointer");
    check(wav != NULL, "Null WaveFile pointer");
    check(buffer != NULL, "Null buffer pointer");
    
    i = fread(buffer, 1, wav->data_size, file);
    check(i == wav->data_size, "Unable to read all data (%ld/%ld)", i, wav->data_size);
    
    if(wav->fmt.fmt_BitsPerSample == 16)
    {
        for(i = 0; i < (wav->data_size / sizeof(WORD)); i++)
        {
            data[i] = SWAP_WORD(data[i]);
        }
    }
    return TRUE;
    
on_error:
    return FALSE;
}



void switch_fmt_endianness(FmtChunk *fmt)
{
    fmt->fmt_AudioFormat = SWAP_WORD(fmt->fmt_AudioFormat);
    fmt->fmt_NumChannels = SWAP_WORD(fmt->fmt_NumChannels);
    fmt->fmt_SampleRate = SWAP_LONG(fmt->fmt_SampleRate);
    fmt->fmt_ByteRate = SWAP_LONG(fmt->fmt_ByteRate);
    fmt->fmt_BlockAlign = SWAP_WORD(fmt->fmt_BlockAlign);
    fmt->fmt_BitsPerSample = SWAP_WORD(fmt->fmt_BitsPerSample);
}