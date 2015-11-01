//Joseph Phan, Wednesday 5pm 3/4/15, Potika Lab 9
// Main program file MAIN9.C
// Written by Daniel W. Lewis
// Revised Jan 2, 2015
//
// Purpose: Create and manipulate a linked list of audio segments

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "wav.h"

typedef struct LIST_TAG
	{
	struct LIST_TAG	*nextnode ;
	AUDIO			*segment ;
	} LIST ;

LIST *	CreateList(AUDIO *segment1, AUDIO *segment2, AUDIO *segment3) ;
AUDIO *	CombineSegments(LIST *list) ;

void DisplayAudio(char *filespec) ;

int main(int argc, char **argv)
	{
	AUDIO *src, *dst, *part1, *part2, *part3 ;
	char *src_filespec, *dst_filespec ;
	unsigned one_third ;
	LIST *list ;

	if (argc != 3)
		{
		fprintf(stderr, "Usage: %s src-file dst-file\n", argv[0]) ;
		exit(255) ;
		}

	src_filespec = argv[1] ;
	dst_filespec = argv[2] ;

	src = ReadWAV16(src_filespec) ;
	one_third = src->num_samples / 3 ;

	part1 = CopySegment(src, 0, one_third) ;
	part2 = CopySegment(src, one_third + 1, 2*one_third) ;
	part3 = CopySegment(src, 2*one_third+1, src->num_samples - 1) ;

	list  = CreateList(part1, part2, part3) ;
	dst   = CombineSegments(list) ;

	WriteWAV16(dst_filespec, dst) ;
	DisplayAudio(dst_filespec) ;

	return 0 ;
	}

//creates a link list containing three audio segments and returns a pointer to the list
LIST *CreateList(AUDIO *segment1, AUDIO *segment2, AUDIO *segment3)
	{
	// To be completed by student ...
        LIST *firstNode;
        LIST *secondNode;
        LIST *thirdNode;

        firstNode = (LIST *)malloc(1*sizeof(LIST));
        firstNode->segment = segment1;      //creates the first node containing segment1

        secondNode = (LIST *)malloc(1*sizeof(LIST));
        secondNode->segment = segment2; //creates the second node containing segment 2
        firstNode->nextnode = secondNode;// connects first node to second node 

        thirdNode = (LIST *)malloc(1*sizeof(LIST));
        thirdNode->segment = segment3;  //creates the third node containing segment3
        secondNode->nextnode = thirdNode; //connects the second node to third
        thirdNode->nextnode = NULL;//third node's pointer next node points to ground

        return firstNode; //returns head of list
	}

//combine all the audio segments in a linked list into a single segment. Returns a pointer to a new memory representation of the combined segment.
AUDIO *CombineSegments(LIST *list)
	{
	// To be completed by student ...
        AUDIO *newAudio;
        LIST *temp = list;
        newAudio = NewAudio(0,temp->segment->sample_rate);//creates a new audio file where it will store all three samples
        while (list!=NULL) {
            newAudio = InsertSegment(newAudio,list->segment,newAudio->num_samples);
            list=list->nextnode;
        }
        //copies all three segments into newAudio
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
