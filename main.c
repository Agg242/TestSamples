/* Audio samples testing tool
*/
#pragma dontwarn 65
#pragma dontwarn 214
#pragma dontwarn 307
#pragma dontwarn 306

#include <exec/types.h>
#include <hardware/cia.h>
#include <proto/dos.h>
#include <dos/dos.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "dbg.h"
#include "soundfile.h"
#include "vampire.h"

#ifdef KDEBUG
#include <proto/debug.h>
#endif

#define VERSTAG "\0$VER: TestSamples 1.0 (8.3.2023) Play at your own risk"

// volatile to forbid unwanted optimization as registers are subjetc to change
extern volatile struct CIA ciaa;

#define INPUT_SIZE 256
char file_name[INPUT_SIZE];


void wait_mouse(void);
void dialog(Sound *sound);
void ask_raw_parameters(Sound *sound);
void to_upcase(char *str);
void strip_newline(char *str);



// CIAF_GAMEPORT0 passe à 0 sur clic de la souris
void wait_mouse(void)
{
    while((ciaa.ciapra & CIAF_GAMEPORT0) != 0) ; // busy wait for left button clic
}



void dialog(Sound *sound)
{
    char *extension;
    BOOL known_extension;
    BOOL lone_file = TRUE;
    
    while(TRUE)
    {
        if(file_name[0] == '\0')
        {
            lone_file = FALSE;
            printf("\n'q' to quit\nFile to load: ");
            fgets(file_name, INPUT_SIZE, stdin);
            strip_newline(file_name);
            
            if((tolower(file_name[0]) == 'q') && (strlen(file_name) == 1))
                break;
        }
        
        known_extension = FALSE;
        extension = strrchr(file_name, '.');
        if(extension == NULL)
        {
            extension = "RAW";
        }
        else
        {
            extension++;
            to_upcase(extension);
        }
        
        if(strcmp(extension, "RAW") == 0)
        {
            ask_raw_parameters(sound);
        }
        
        if(sound_open(sound, file_name, extension))
        {
            sound_play(sound);
            printf("Left click to stop...");
            fflush(stdout);
            wait_mouse();
            printf("\n");
            sound_stop(sound);        
        }
        sound_cleanup(sound);

    
        if(lone_file)
        {
            break;  // lone file
        }
    }
}


#define MAX_BITSPERSAMPLE_LEN 2 + 2  // + newline + 0
#define MAX_SAMPLERATE_LEN 5 + 2  // + newline + 0
#define MAX_CHANNELS_LEN 1 + 2  // + newline + 0
#define MAX_BOOLEAN_LEN 5 + 2  // + newline + 0
#define MAX_AUDIOLEN_LEN 8 + 2  // + newline + 0

void ask_raw_parameters(Sound *sound)
{
    char input[INPUT_SIZE];
    ULONG uval32;
    UWORD uval16;

    printf("Bits per sample (8/16): ");
    fgets(input, MAX_BITSPERSAMPLE_LEN, stdin);
    strip_newline(input);
    sound->bits_per_sample = atoi(input);
   
    printf("Channels: ");
    fgets(input, MAX_CHANNELS_LEN, stdin);
    strip_newline(input);
    sound->channels = atoi(input);
   
    printf("Sample rate: ");
    fgets(input, MAX_SAMPLERATE_LEN, stdin);
    strip_newline(input);
    sound->sample_rate = atol(input);
   
    printf("Played by Paula (true/false): ");
    fgets(input, MAX_BOOLEAN_LEN, stdin);
    strip_newline(input);
    to_upcase(input);
    if(strcmp(input, "TRUE") == 0)
    {
        sound->for_paula = TRUE;
    }
    else
    {
        sound->for_paula = FALSE;
    }
}


void to_upcase(char *str)
{
    if(str != NULL)
    {
        while(*str)
        {
            *str = toupper(*str);
            str++;
        }
    }
}


void strip_newline(char *str)
{
    char *end = str;
    
    if(end != NULL)
    {
        while(*end)
        {
            end++;
        }
        
        if((end != str) && (*(--end) == '\n'))
        {
            *end = '\0';
        }
    }
}


#define ARGS_COUNT 3
#define ARG_SAMPLE 0
#define ARG_PAULA 1
#define ARG_VOLUME 2
BOOL parse_args(Sound *sound)
{
    BOOL res = FALSE;
    char template[] = "SAMPLE,PAULA/S,VOLUME/K/N";
    struct RDArgs *args;
    LONG params[ARGS_COUNT];
    file_name[0] = '\0';

    memset(params, 0, sizeof(params));
    
    args = ReadArgs(template, params, NULL);
    if(args != NULL)
    {
        if(params[ARG_SAMPLE] != 0) // sample
        {
            strncpy(file_name, (char *)params[ARG_SAMPLE], INPUT_SIZE - 1);
            file_name[INPUT_SIZE - 1] = '\0'; 
        }
        
        if(params[ARG_PAULA] != 0)
        {
            sound->for_paula = TRUE;
        }
        
        if(params[ARG_VOLUME] != 0)
        {
            sound->volume = *((LONG *)params[ARG_VOLUME]) & 0xff;
        }
        
        res = TRUE;
    }
    else  // parse error
    {
        puts(template);
    }
    
        
    if(args != NULL)
    {
        FreeArgs(args);
    }
    return res;
}



int main(int argc, char *argv[])
{    
    STRPTR ver = (STRPTR)VERSTAG;
    Sound sound;
    
    check(is_vampire_present(), "Apollo Vampire Board required.");
    init_vampire_regs();
    
    sound_cleanup(&sound);
    
    if(parse_args(&sound))
    {
        dialog(&sound);
        return 0;
    }
    else
    {
        return 2;
    }
    
    
on_error:
    return 1;
}
