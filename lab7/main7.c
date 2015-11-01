//Joseph Phan - 2/18/14 - Wednesday 5PM - Potika
// Main program file MAIN7.C
// Written by Daniel W. Lewis
// Revised Feb 13, 2015
//
// Purpose: Create horizontally and vertically mirrored versions of an image using multi-threading.

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/stat.h>
#include "bmp3.h"
#include <pthread.h>

#define ROWS_PER_THREAD ((image->rows+7)/NUM_THREADS)/2
#define COLS_PER_THREAD ((image->cols+7)/NUM_THREADS)/2

void *ThreadMirrorCols(void *arg);
IMAGE *MirrorLeftRight(IMAGE *image);

void *ThreadMirrorRows(void *arg);
IMAGE *MirrorUpDown(IMAGE *image);

IMAGE * image;
typedef struct
	{
	char	*format ;
	IMAGE	*(*function)() ;
	double	param ;
	} OPTION ;

#define NUM_THREADS 8 

OPTION	*GetOption(int argc, char **argv, OPTION options[]) ;
void	DisplayImage(char *filespec, char *format, ...) ;

int main(int argc, char **argv)
	{
	char *src_filespec, *dst_filespec ;
	static OPTION options[] =
		{
		{"Mirror Left-Right",	MirrorLeftRight,	   0},
		{"Mirror Up-Down",		MirrorUpDown,		   0},
		{NULL,					NULL,				   0}
		} ;
	OPTION *option ;

	if (argc != 3 && argc != 4)
		{
		fprintf(stderr, "Usage: %s src-file dst-file {option#}\n", argv[0]) ;
		exit(255) ;
		}

	src_filespec = argv[1] ;
	dst_filespec = argv[2] ;

	image = ReadBMP24(src_filespec) ;

	if ((option = GetOption(argc, argv, options))== NULL)
		{
		fprintf(stderr, "Usage: %s src-file dst-file {option#}\n", argv[0]) ;
		exit(255) ;
		}

	image = (option->function)(image, option->param) ;

	WriteBMP24(dst_filespec, image) ;
	FreeImage(image) ;
	DisplayImage(dst_filespec, option->format, option->param) ;

	return 0 ;
	}

//worker function that swaps pixels between columns
void *ThreadMirrorCols(void *arg)
	{
		// To be completed by student ...
    int start; //start point for thread
    start = (int) arg;
    int i,j;
    int cols, rows;
    cols = image->cols;
    rows = image->rows;
    for(i=0; i< rows; i++){
        for(j=start; j<start +COLS_PER_THREAD; j++){
            if(j==(cols+1)/2)//checks if thread passes halfway point
                break;
            PIXEL temp = image->pxlrow[i][j];     
            image->pxlrow[i][j] = image->pxlrow[i][cols-1-j];     
            image->pxlrow[i][cols-1-j] = temp;//swaps pixels between columns
        }
    }
    return NULL;    
	}
//reverses columns of an image by executing threads
IMAGE *MirrorLeftRight(IMAGE *image)
	{
	   	// To be completed by student ...
    pthread_t id[NUM_THREADS];
    int i, j;
    int start = 0;//start point for thread
    for(i=0; i<NUM_THREADS; i++){     //creates threads
        pthread_create(&id[i], NULL, ThreadMirrorCols, (void*)start);
        start+=COLS_PER_THREAD;//creates bounds for thread
    }
    for(j=0;j<NUM_THREADS;j++){
        pthread_join(id[j],NULL); //waits for thread to finish
    }

    return image;    
	}
//worker function that swaps pixel rows
void *ThreadMirrorRows(void *arg)
	{
    // To be completed by student ...
    int start, i,rows;//start is startpoint for thread.
    start = (int) arg;
    rows = image->rows;
    for(i=start; i< start + ROWS_PER_THREAD;i++){
        if(i>=image->rows/2) //checks if thread pass halfway point
            break;
        PIXEL *temp = image->pxlrow[i];     
        image->pxlrow[i] = image->pxlrow[rows-i-1];     
        image->pxlrow[rows-i-1] = temp; //swaps rows
     
	}
    return NULL;
    }
//reverses the rows of an image by executing threads
IMAGE *MirrorUpDown(IMAGE *image)
	{
    // To be completed by student ...	
    pthread_t id[NUM_THREADS];
    int i,j;
    int start = 0;
    for(i=0; i<NUM_THREADS; i++){     //creates the threads
        pthread_create(&id[i], NULL, ThreadMirrorRows, (void*)start);
        start+=ROWS_PER_THREAD;//sets column bounds for each thread
    }

    for(j=0;j<NUM_THREADS;j++){
        pthread_join(id[j],NULL); //waits for threads to finish
    }

    return image;    
	}


OPTION *GetOption(int argc, char **argv, OPTION options[])
	{
	unsigned number, index = 0 ;

	// count number of options
	for (number = 0; options[number].function != NULL; number++) ;

	if (argc == 4)
		{
		sscanf(argv[3], "%u", &index) ;
		}
	else
		{
		printf("\nOptions:\n\n") ;
		for (index = 0; index < number; index++)
			{
			char title[100] ;
			sprintf(title, options[index].format, options[index].param) ;
			printf("%2d: %s\n", index + 1, title) ;
			}

		printf("\nOption? ") ;
		scanf("%u", &index) ;
		}

	return (1 <= index && index <= number) ? &options[index - 1] : NULL ;
	}

void DisplayImage(char *filespec, char *format, ...)
	{
	char progspec[200], command[1000], *program_files, title[100] ;
	struct stat filestat ;
	va_list args ;
	
	va_start(args, format) ;
	vsprintf(title, format, args) ;
	va_end(args) ;

#if defined(_WIN32)
	program_files = getenv("ProgramFiles(x86)") ;
	if (program_files == NULL) program_files = getenv("C:\\Program Files") ;
	if (program_files == NULL) program_files = "" ;

	sprintf(progspec, "%s\\IrfanView\\i_view32.exe", program_files) ;
	if (stat(progspec, &filestat) == 0 && (filestat.st_mode & S_IFREG) != 0)
		{
		sprintf(command, "\"%s\" %s /hide=7 /title=%s", progspec, filespec, title) ;
		}
	else sprintf(command, "mspaint \"%s\"\n", filespec) ;
#elif defined(__unix)
	sprintf(command, "qiv \"%s\"", filespec) ;
#elif defined(__APPLE__)
	sprintf(command, "open \"%s\"", filespec) ;
#endif 
	system(command) ;
	}

