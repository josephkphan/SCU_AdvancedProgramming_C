//Joseph Phan, Wed 5PM, Lab 8, Potika, 2/25/15
// Main program file MAIN8.C
// Written by Daniel W. Lewis
// Revised Jan 2, 2015
//
// Purpose: Adjust the tone (bass and treble) of an audio recording.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "wav.h"

AUDIO *AdjustTone(AUDIO *audio, unsigned percent_bass, unsigned percent_treble) ;

void DisplayAudio(char *filespec) ;

int main(int argc, char **argv)
	{
	char *src_filespec, *dst_filespec ;
	unsigned percent_bass, percent_treble ;
	int tone_degrees ;
	AUDIO *audio ;

	if (argc != 4)
		{
		fprintf(stderr, "Usage: %s src-file dst-file tone-degrees\n", argv[0]) ;
		exit(255) ;
		}

	src_filespec = argv[1] ;
	dst_filespec = argv[2] ;
	tone_degrees = atoi(argv[3]) ;

	percent_bass	= (tone_degrees >= 0) ? 0 : -tone_degrees / 0.9 ; 
	percent_treble	= (tone_degrees <= 0) ? 0 : +tone_degrees / 0.9 ;

	audio = ReadWAV16(src_filespec) ;
	audio = AdjustTone(audio, percent_bass, percent_treble) ;
	WriteWAV16(dst_filespec, audio) ;

	DisplayAudio(dst_filespec) ;

	return 0 ;
	}
//this function audjusts the level of treble and bass in the audio file
AUDIO *AdjustTone(AUDIO *audio, unsigned percent_bass, unsigned percent_treble)
	{
	// To be completed by student ...
    AUDIO *newAudio;//makes a new audio structure
    double sample_avg, sample_diff, bass_part, orig_part, treb_part, orig_sample;
    double prev_sample=0;
    int i,j;
    newAudio = NewAudio(audio->num_samples, audio->sample_rate);
    sample_avg =0;
    for (i=0; i<audio->num_samples; i++) {
        sum=0;

        orig_sample = audio->samples[i];
        sample_avg = .9*sample_avg + .1*orig_sample;    
        sample_diff = orig_sample - prev_sample;

        bass_part = sample_avg * (percent_bass/100);
        orig_part = orig_sample * (100-percent_bass - percent_treble)/100;
        treb_part = sample_diff * (percent_treble/100);

        newAudio->samples[i] = 2*bass_part + orig_part + 2*treb_part;

        prev_sample = orig_sample;//calculates the adjusted treble and bass values and stores them in the current sample
    }
    return newAudio; 
    }

void DisplayAudio(char *filespec)
	{
	char progspec[200], command[1000], *program_files ;
	struct stat filestat ;

#if defined(_WIN32)
	program_files = getenv("ProgramFiles(x86)") ;
	if (program_files == NULL) program_files = getenv("C:\\Program Files") ;
	if (program_files == NULL) return ;

	sprintf(progspec, "%s\\Audacity\\audacity.exe", program_files) ;
	if (stat(progspec, &filestat) != 0) return ;
	if ((filestat.st_mode & S_IFREG) == 0) return ;

	sprintf(command, "\"%s\" %s", progspec, filespec) ;
#elif defined(__unix)
	sprintf(command, "audacity \"%s\"", filespec) ;
#elif defined(__APPLE__)
	sprintf(command, "/Applications/Audacity/Audacity.app/Contents/MacOS/Audacity \"%s\"", filespec) ;
#endif 
	system(command) ;
	}
