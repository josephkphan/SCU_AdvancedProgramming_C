//Joseph Phan - Coen 11 - Lab3 - 1/21/15 - Wednesday 5pm - Potika 

// igned row = GetRows(image);
//         unsigned col = GetCols(image);
//                 unsigned i, j;
//                 ain program file MAIN3.C
// Written by Daniel W. Lewis
// Revised Jan 2, 2015
//
// Purpose: Change the hue, saturation and value of an image.

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "bmp2.h"

IMAGE	*AdjustHue(IMAGE *image, double degrees) ;
IMAGE	*AdjustSaturation(IMAGE *image, double percent) ;
IMAGE	*AdjustValue(IMAGE *image, double percent) ;

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
		{"Hue (%.0f deg)",		AdjustHue,			-45},
		{"Saturation (%.1f)",	AdjustSaturation,	 70},
		{"Value (%.1f)",		AdjustValue,		120},
		{NULL,					NULL,				  0}
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

IMAGE *AdjustHue(IMAGE *image, double degrees)
	{
	// To be completed by student ...
	//This function adjusts the hue in all pixels of an image by a certain amount of degrees
	unsigned row = GetRows(image);
	unsigned col = GetCols(image);
	unsigned i, j;
	double hue, sat, val;
	
	for (i=0; i<row; i++) {
		for (j=0; j<col; j++) {		//traverses the array
			GetHSV(image, i, j, &hue, &sat, &val); 	//retreives the values
			PutHSV(image, i, j, hue + degrees, sat, val);		//adjusts the hue value
	
		}
	}
	return image;
	}

IMAGE *AdjustSaturation(IMAGE *image, double percent)
	{
	// To be completed by student ...
	// this function adjusts the saturation of all pixels by a certain percent 
	unsigned row = GetRows(image);
	unsigned col = GetCols(image);
	unsigned i, j;
	double hue, sat, val;

		for(i=0; i<row; i++) {
			for (j=0; j<col; j++) { //these loops traverses the image
				GetHSV( image, i, j, &hue, &sat, &val); 	//retreives the values
				PutHSV( image, i, j, hue, sat*(percent/100), val);	//adjusts the sat value
			}
		}
	return image;
	}

IMAGE *AdjustValue(IMAGE *image, double percent)
	{
	// To be completed by student ...
	// This function adjusts the value (perceived luminance of all pixels in an image by a certain percent
	unsigned row = GetRows(image);
	unsigned col = GetCols(image);
	unsigned i, j;
	double hue, sat, val; 

		for (i=0; i<row; i++) {
			for (j=0; j<col; j++) { //these for loops traverses through the 2D image
				GetHSV( image, i, j, &hue, &sat, &val); //retreives the values
				PutHSV( image, i, j, hue, sat, val*(percent/100)); //changes the value by the parameter percent
			}		
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

