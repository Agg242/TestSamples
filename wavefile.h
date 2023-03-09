#ifndef WAVEFILE_H__
#define WAVEFILE_H__

#include <stdio.h>
#include <exec/types.h>


typedef struct fmtchunk_t {
    UWORD fmt_AudioFormat;
    UWORD fmt_NumChannels;
    ULONG fmt_SampleRate;
    ULONG fmt_ByteRate;
    UWORD fmt_BlockAlign;
    UWORD fmt_BitsPerSample;
} FmtChunk;

typedef struct wavfile_t {
    FmtChunk fmt;
    ULONG data_size;
} WavFile;


extern BOOL read_wav_header(FILE *file, WavFile *wav);
extern BOOL read_wav_data(FILE *file, WavFile *wav, APTR buffer);

#endif
