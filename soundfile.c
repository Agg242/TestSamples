#include "dbg.h"
#include "soundfile.h"
#include "vampire.h"
#include "wavefile.h"
#include "aifffile.h"
#include "8svxfile.h"
#include <proto/exec.h>
#include <hardware/dmabits.h>


#define MAX_PAULA_VOLUME 64
#define MAX_ARNE_VOLUME 0x80
#define PAL_CLOCK_TICKS_PER_SEC 3546895
#define NTSC_CLOCK_TICKS_PER_SEC 3579545
#define MIN_PAL_AUDIO_DMA_TICKS 123
#define MIN_NTSC_AUDIO_DMA_TICKS 124
#define MAX_PAULA_SAMPLERATE PAL_CLOCK_TICKS_PER_SEC / MIN_PAL_AUDIO_DMA_TICKS
#define MAX_PAULA_SAMPLES 0xffff * sizeof(UWORD)
// www.apollo.core/sagadoc/audxper.htm
#define MIN_ARNE_SAMPLE_PERIOD 65


void check_Paula_requirements(Sound *sound)
{
    if(sound->for_paula)
    {
        if(sound->channels != 1)
        {
            printf("Stereo sample for Paula ? I don't feel like playing it\n");
            sound->for_paula = FALSE;
        }

        if(sound->bits_per_sample != 8)
        {
            printf("More than 16 bits for Paula ? In your dreams only.\n");
            sound->for_paula = FALSE;
        }

        if(sound->sample_rate > MAX_PAULA_SAMPLERATE)
        {
            printf("Sample rate > %d for Paula ? Not for me.\n", MAX_PAULA_SAMPLERATE);
            sound->for_paula = FALSE;
        }
        
        if(sound->data_size > MAX_PAULA_SAMPLES)
        {
            printf("Sample len > %d samples for Paula ? Not in this universe.\n", MAX_PAULA_SAMPLES);
            sound->for_paula = FALSE;            
        }
    }
}


void sound_alloc_buffer(Sound *sound)
{
    ULONG alignment = sizeof(UWORD);
    ULONG delta;
    ULONG attrs;
    
    // get alignment constraint
    if(sound->bits_per_sample == 16)
    {
        alignment *= 2;
        if(sound->channels == 2)
        {
            alignment *= 2;
        }
    }
        
    // alloc buffer
    check_Paula_requirements(sound);
    if(sound->for_paula)
    {
        attrs = MEMF_CHIP;
    }
    else
    {
        attrs = MEMF_FAST;
    }
    sound->buffer = AllocVec(sound->data_size + alignment, attrs);
    check_mem(sound->buffer); 
       
    // set aligned data pointer
    delta = (ULONG)(sound->buffer) % alignment;
    if(delta != 0)
    {
        delta = alignment - delta;
    }
    sound->data_ptr = (APTR) ((char *)(sound->buffer) + delta);

  #ifdef DEBUG
    printf("\nBuffer: %p, alignment: %ld, sample data: %p\n", 
           sound->buffer, alignment, sound->data_ptr);
  #endif

on_error:
    return;    
}


void load_wav(FILE *file, Sound *sound)
{
    WavFile wav;

    check(read_wav_header(file, &wav) == TRUE, "Unable to read WAV header");
    sound->sample_rate = wav.fmt.fmt_SampleRate;
    sound->bits_per_sample = wav.fmt.fmt_BitsPerSample;
    sound->channels = wav.fmt.fmt_NumChannels;
    sound->data_size = wav.data_size;
    
    sound_alloc_buffer(sound);
    check_mem(sound->buffer);    
    
    check(read_wav_data(file, &wav, sound->data_ptr) == TRUE, "Unable to load WAV data");

    return;
    
    
on_error:
    sound_cleanup(sound);
    return;    
}


void load_aiff(FILE *file, Sound *sound)
{
    AiffFile aiff;

    check(read_aiff_header(file, &aiff) == TRUE, "Unable to read AIFF header");
    sound->sample_rate = aiff.sampleRate;
    sound->bits_per_sample = aiff.comm.sampleSize;
    sound->channels = aiff.comm.numChannels;
    sound->data_size = aiff.data_size;
    
    sound_alloc_buffer(sound);
    check_mem(sound->buffer);    
    
    check(read_aiff_data(file, &aiff, sound->data_ptr) == TRUE, "Unable to load AIFF data");

    return;
    
    
on_error:
    sound_cleanup(sound);
    return;
}


void load_8svx(FILE *file, Sound *sound)
{
    A8svxFile a8svx;

    check(read_8svx_header(file, &a8svx) == TRUE, "Unable to read 8SVX header");
    sound->sample_rate = a8svx.vhdr.samplesPerSec;
    sound->bits_per_sample = 8;
    sound->channels = 1;
    sound->data_size = a8svx.data_size;
    
    sound_alloc_buffer(sound);
    check_mem(sound->buffer);    
    
    check(read_8svx_data(file, &a8svx, sound->data_ptr) == TRUE, "Unable to load AIFF data");

    return;
    
    
on_error:
    sound_cleanup(sound);
    return;
}


void load_raw(FILE *file, Sound *sound)
{
    size_t siz;
   
    fseek(file, 0, SEEK_END);
    sound->data_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    check(sound->data_size > 0, "Null data size");
    
    sound_alloc_buffer(sound);
    check_mem(sound->buffer);    
    
    siz = fread(sound->data_ptr, 1, sound->data_size, file);
    check(siz == sound->data_size, "Unable to read expected data size (%ld/%ld)", siz, sound->data_size);
    
    return;
    
    
on_error:
    sound_cleanup(sound);
    return;    
}



BOOL check_params(Sound *sound)
{
    BOOL res = TRUE;
    
    if((sound->bits_per_sample != 8) &&
       (sound->bits_per_sample != 16))
    {
        printf("Unsupported bits per sample : %hu\n", sound->bits_per_sample);
        res = FALSE;
    }
    
    if(sound->sample_rate == 0)
    {
        printf("Null sample rate : %lu\n", sound->sample_rate);
        res = FALSE;
    } 
    else
    {
        // Todo: check PAL/NTSC
        sound->sample_period = PAL_CLOCK_TICKS_PER_SEC / sound->sample_rate;
        if(sound->sample_period < MIN_ARNE_SAMPLE_PERIOD)
        {
            printf("Sample rate too high : %lu, playback will be ssslllooowww. Max supported rate is %lu\n", 
                   sound->sample_rate,
                   PAL_CLOCK_TICKS_PER_SEC / MIN_ARNE_SAMPLE_PERIOD);
        }
    }    
    
    switch(sound->channels)
    {
        case 1:
            // mono
            break;
            
        case 2:
            // stereo
            if(sound->bits_per_sample == 8)
            {
                printf("Stereo not supported with 8 bits sample\n");
                res = FALSE;
            }
            break;
            
        default:
            printf("Unsupported channels count : %hu\n", sound->channels);
            res = FALSE;                
            break;
    }

    sound->len = ((sound->data_size / (sound->channels)) / sizeof(UWORD)) / (sound->bits_per_sample / 8);
        
    return res;
}


BOOL sound_open(Sound *sound, char *filename, char *extension)
{
    FILE *file;
    ULONG attrs;
    size_t siz;
    
    sound->len = 0;
    sound->buffer = NULL;
    file = fopen(filename, "r");
    check(file != NULL, "Unable to open file %s", filename);

    if(strcmp(extension, "WAV") == 0)
    {
        load_wav(file, sound);
    }
    else if(strcmp(extension, "AIFF") == 0)
    {
        load_aiff(file, sound);
    }
    else if(strcmp(extension, "8SVX") == 0)
    {
        load_8svx(file, sound);
    }
    else if(strcmp(extension, "RAW") == 0)
    {
        load_raw(file, sound);
    }
    else
    {
        printf("Unknown file extension, change it to RAW if adequate\n");
        goto on_error;
    }

    check(sound->buffer != NULL, "Unable to load sound");
    
#ifdef DEBUG
    printf("Data size: %.7lu\n", sound->data_size);
    printf("Sample rate: %lu\n", sound->sample_rate);
    printf("Bits per sample: %.2hu\n", sound->bits_per_sample);
    printf("Channels: %hu\n", sound->channels);
#endif

    check(check_params(sound) == TRUE, "Playback aborted");

#ifdef DEBUG
    printf("Paula used: %hhd\n\n", sound->for_paula);
#endif
    
    if(file != NULL)
    {
        fclose(file);
    }
    return TRUE;

    
on_error:
    if(file != NULL)
    {
        fclose(file);
    }
    sound_cleanup(sound);

    return FALSE;    
}


#define CTRL_LEN 38 // AUDF_STEREO AUDF_ONESHOT AUDF_16BITS
void sound_play(Sound *sound)
{
  #ifdef DEBUG
    char str[CTRL_LEN] = "";
  #endif
    UWORD ctrl = 0;
    UWORD period = 0;
    
    
    vampire_regs->dmacon = DMAF_AUD0;  // shutting audio0 DMA

    if(sound->for_paula)
    {
      #ifdef DEBUG
        printf("PAULA AUD0LEN= %hu, AUD0PER= %hu\n", 
               sound->len, sound->sample_period);
        fflush(stdout);
      #endif
        if(sound->volume == 0)
        {
            sound->volume = MAX_PAULA_VOLUME;
        }    
        // played on left and right channels
        vampire_regs->aud8[0].ac8_ptr = sound->data_ptr;
        vampire_regs->aud8[0].ac8_vol = sound->volume;
        vampire_regs->aud8[0].ac8_len = sound->len;
        vampire_regs->aud8[0].ac8_per = sound->sample_period;
        vampire_regs->aud8[1].ac8_ptr = sound->data_ptr;
        vampire_regs->aud8[1].ac8_vol = sound->volume;
        vampire_regs->aud8[1].ac8_len = sound->len;
        vampire_regs->aud8[1].ac8_per = sound->sample_period;
        vampire_regs->dmacon = DMAF_SETCLR | DMAF_MASTER | DMAF_AUD0 | DMAF_AUD1;
    }
    else // ARNE
    {
        if(sound->channels == 2)
        {
            ctrl |= AUDF_STEREO;
        }
        if(sound->bits_per_sample == 16)
        {
            ctrl |= AUDF_16BITS;
        }
        
        if(sound->volume == 0)
        {
            sound->volume = MAX_ARNE_VOLUME;
        }    

                
      #ifdef DEBUG
        if(ctrl & AUDF_STEREO)
        {
            strncat(str, "AUDF_STEREO ", CTRL_LEN);
        }
        if(ctrl & AUDF_ONESHOT)
        {
            strncat(str, "AUDF_ONESHOT ", CTRL_LEN);
        }
        if(ctrl & AUDF_16BITS)
        {
            strncat(str, "AUDF_16BITS ", CTRL_LEN);
        }
        printf("ARNE AUD0LEN= %lu, AUD0CTRL= %s, AUD0PER= %hu\n", 
               sound->len, str, sound->sample_period);
        fflush(stdout);
      #endif    

        vampire_regs->aud16[0].ac16_ptr = sound->data_ptr;
//        vampire_regs->aud16[0].ac16_vol = (UWORD)0xffff; // ARNE can scream louder
        vampire_regs->aud16[0].ac16_vol = (sound->volume) | (sound->volume << 8); // left + right
        vampire_regs->aud16[0].ac16_len = sound->len;
        vampire_regs->aud16[0].ac16_per = sound->sample_period;
        vampire_regs->aud16[0].ac16_ctrl = ctrl;
        vampire_regs->dmacon = DMAF_SETCLR | DMAF_MASTER | DMAF_AUD0;
    }

}


void sound_stop(Sound *sound)
{
    if(sound->for_paula)
    {
        vampire_regs->dmacon = DMAF_AUD0 | DMAF_AUD1;  // shutting audio0 and audio1 DMA
    }
    else
    {
        vampire_regs->dmacon = DMAF_AUD0;  // shutting audio0 DMA
    }
}


void sound_cleanup(Sound *sound)
{
    if(sound != NULL)
    {
        if(sound->buffer != NULL)
        {
            // cleanup
            FreeVec(sound->buffer);
            sound->buffer = NULL;
        }
        memset(sound, 0, sizeof(Sound));
    }
}
