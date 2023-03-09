#ifndef SOUNDFILE_H__
#define SOUNDFILE_H__

#include <exec/types.h>


typedef struct sound_t {
    APTR buffer;        // allocated mem
    APTR data_ptr;      // aligned pointer
    ULONG data_size;
    ULONG sample_rate;
    ULONG len;          // words sample length based on channels and bits
    UWORD bits_per_sample;
    UWORD channels;
    UWORD sample_period;
    UWORD volume;
    BOOL for_paula;
} Sound;


extern BOOL sound_open(Sound *sound, char *filename, char *extension);
extern void sound_play(Sound *sound);
extern void sound_stop(Sound *sound);
extern void sound_cleanup(Sound *sound);


#endif