
//Joseph Phan, Wed Lab5 5PM, 2/4/15, Potika
// Main program file MAIN5.C
// Written by Daniel W. Lewis
// Revised Jan 2, 2015
//
// Purpose: Create double size and half size versions of an image.

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/stat.h>
#include "bmp3.h"

IMAGE	*HalfSize(IMAGE *image) ;
IMAGE	*DoubleSize(IMAGE *image) ;

typedef struct
	{
	char	*format ;
	IMAGE	*(*function)() ;
	double	param ;
	} OPTION ;

OPTION	*GetOption(int argc, char **argv, OPTION options[]) ;
void	DisplayImage(char *filespec, char *format, ...) ;

int main(int argc, char **argv)
	{
	char *src_filespec, *dst_filespec ;
	static OPTION options[] =
		{
		{"Half Size",			HalfSize,			  0.0},
		{"Double Size",			DoubleSize,			  0.0},
		{NULL,					NULL,				  0.0}
		} ;
	OPTION *option ;
	IMAGE *image ;

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
//function definition: doubles the size of an image. each pixel of the image is copied four times to a fill a 2x2 subarray of pixels
IMAGE *DoubleSize(IMAGE *src)
	{
	// To be completed by student ...
	IMAGE *newImage;
	newImage = NewImage((src->rows)*2, (src->cols)*2+1); //creates a new image
	
	int i, j;
	for(i=0; i<src->rows; i++) {
		for (j=0; j<src->cols; j++) { //traverses through each pixel of the original image
			newImage->pxlrow[2*i][2*j].blu=src->pxlrow[i][j].blu;
			newImage->pxlrow[2*i+1][2*j].blu=src->pxlrow[i][j].blu;
			newImage->pxlrow[2*i][2*j+1].blu=src->pxlrow[i][j].blu;
			newImage->pxlrow[2*i+1][2*j+1].blu=src->pxlrow[i][j].blu;
			//copies blue component to 2x2 subarray in the new image

			newImage->pxlrow[2*i][2*j].grn=src->pxlrow[i][j].grn;
			newImage->pxlrow[2*i+1][2*j].grn=src->pxlrow[i][j].grn;
			newImage->pxlrow[2*i][2*j+1].grn=src->pxlrow[i][j].grn;
			newImage->pxlrow[2*i+1][2*j+1].grn=src->pxlrow[i][j].grn;
			//copies green component to 2x2 subarray in the new image


			newImage->pxlrow[2*i][2*j].red=src->pxlrow[i][j].red;
			newImage->pxlrow[2*i+1][2*j].red=src->pxlrow[i][j].red;
			newImage->pxlrow[2*i][2*j+1].red=src->pxlrow[i][j].red;
			newImage->pxlrow[2*i+1][2*j+1].red=src->pxlrow[i][j].red;
			//copies red component to 2x2 subarray in the new image
		}
	}
	return newImage;
	}

//function definition: creates a half sized copy of the original image. each pixel is computed as the average of a 2x2 array of pixels taken from the original image.
IMAGE *HalfSize(IMAGE *src)
	{
	// To be completed by student ...

	int newRow = (src->rows/2);
	int newCol= (src->cols/2);
        IMAGE *newImage;
        newImage = NewImage(src->rows/2, src->cols/2);//creates the new image

        int i, j;
	unsigned int sum_red;
	unsigned int sum_blu;
	unsigned int sum_grn;

        for(i=0; i<newRow; i++) { //traverse through each pixel of the original image
                for (j=0; j<newCol; j++) {
			sum_red=0;
			sum_blu=0;
			sum_grn=0;

			if ((2*i+1) ==src->rows) { //outside rows
				 if((2*j+1) == src->cols){ // outisde rows + cols
					newImage->pxlrow[i][j].blu = src->pxlrow[2*i][2*j].blu;
					newImage->pxlrow[i][j].red = src->pxlrow[2*i][2*j].red;
					newImage->pxlrow[i][j].grn= src->pxlrow[2*i][2*j].grn;
				}else{  //only outside rows
					sum_red+=src->pxlrow[2*i][2*j].red;
					sum_red+=src->pxlrow[2*i+1][2*j].red;
					newImage->pxlrow[i][j].red = (sum_red/2);
					//finds the average of red component and puts it in new image	
			
					sum_grn+=src->pxlrow[2*i][2*j].grn;
					sum_grn+=src->pxlrow[2*i+1][2*j].grn;
					newImage->pxlrow[i][j].grn = (sum_grn/2);
					//finds the average of green component and puts it in new image	
	
					sum_blu+=src->pxlrow[2*i][2*j].blu;
					sum_blu+=src->pxlrow[2*i+1][2*j].blu;
					newImage->pxlrow[i][j].blu = (sum_blu/2);
					//finds the average of blue component and puts it in new image				
				}
			}else {
				if((2j+1) ==src->cols){   //only inside rows
					sum_red+=src->pxlrow[2*i][2*j].red;
					sum_red+=src->pxlrow[2*i][2*j+1].red;
					newImage->pxlrow[i][j].red = (sum_red/2);
					//finds the average of red component and puts it in new image	

					sum_grn+=src->pxlrow[2*i][2*j].grn;
					sum_grn+=src->pxlrow[2*i][2*j+1].grn;
					newImage->pxlrow[i][j].grn = (sum_grn/2);
					//finds the average of green component and puts it in new image	
	
					sum_blu+=src->pxlrow[2*i][2*j].blu;
					sum_blu+=src->pxlrow[2*i][2*j+1].blu;
					newImage->pxlrow[i][j].blu = (sum_blu/2);
					//finds the average of blue component and puts it in new image		
				}else {
					sum_red+=src->pxlrow[2*i][2*j].red;
					sum_red+=src->pxlrow[2*i+1][2*j].red;
					sum_red+=src->pxlrow[2*i][2*j+1].red;
					sum_red+=src->pxlrow[2*i+1][2*j+1].red;
					newImage->pxlrow[i][j].red = (sum_red/4);
					//finds the average of red component and puts it in new image	

					sum_grn+=src->pxlrow[2*i][2*j].grn;
					sum_grn+=src->pxlrow[2*i+1][2*j].grn;
					sum_grn+=src->pxlrow[2*i][2*j+1].grn;
					sum_grn+=src->pxlrow[2*i+1][2*j+1].grn;
					newImage->pxlrow[i][j].grn = (sum_grn/4);
					//finds the average of green component and puts it in new image	
	
					sum_blu+=src->pxlrow[2*i][2*j].blu;
					sum_blu+=src->pxlrow[2*i+1][2*j].blu;
					sum_blu+=src->pxlrow[2*i][2*j+1].blu;
					sum_blu+=src->pxlrow[2*i+1][2*j+1].blu;
					newImage->pxlrow[i][j].blu = (sum_blu/4);
					//finds the average of blue component and puts it in new image			
				}
		
			}
		}
	}
	return newImage;
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

