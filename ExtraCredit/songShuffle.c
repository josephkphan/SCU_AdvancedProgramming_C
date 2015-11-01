//Made by Joseph Phan, Potika, 3/11/15

#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>

//swaps the songs in the dynamic array
void swap(char *songlist, int i, int j) { 
    char temp[100];
    char *songPt1;
	char *songPt2;
	songPt1 = songlist + j * 100; 
	songPt2 = songlist + i * 100;
    strcpy (temp, songPt1);
    strcpy (songPt1, songPt2);
    strcpy (songPt2, temp);

}
//will shuffle the users songs and print out the time efficency of each shuffle
void shuffle (char *songlist, int songCounter) { 
    int i, j;
	clock_t start_t , end_t,total_t;
    double tcpu;
	start_t = clock();
    for (i=0;i<songCounter; i++) {
        j = rand() % songCounter; //swaps songs randomly
        swap(songlist, i, j);
    }   
	end_t = clock() ;
	total_t=(end_t - start_t);
	tcpu= (double)total_t/CLOCKS_PER_SEC;//calculates time took for shuffle
    printf("cpu time for shuffle %f\n",tcpu);

}

//will open the user's playlist and copy it into the dynamic char array and count how many songs the user has
void readPlaylist(char *songlist, int *songCounter) {
    FILE *infp;
    char temp[100];
    char ch;
	char *pt;
	int cnt =0; //counts the number of songs
	pt = songlist;
    if((infp = fopen("playlist.txt", "r")) ==NULL )
        printf("cannot open the file %s\n");
    while ((ch = fgets(temp, 100, infp)) != NULL) { 
        strcpy(pt, temp); //copies songs into dynamic array
        pt+=100;
        cnt=cnt+1;
        memset(temp,0,100);//clears out temp
    }
	*songCounter=cnt-1;
    fclose(infp);
}

//will create new files with the shuffle
void writePlaylist(char *songlist ) {
    FILE *outfp;
    int songCounter =0; //count how many songs the user has in his/her list
    char Playlist_of_the_day[7][25] = {"shuffleMonday.txt","shuffleTuesday.txt", "shuffleWednesday.txt","shuffleThursday.txt","shuffleFriday.txt", "shuffleSaturday.txt", "shuffleSunday.txt."}; 
    int i, j;
    for(i=0; i<7; i++){//performs a shuffle for each day of the week
		printf("Shuffle Playlist for %s\n",Playlist_of_the_day[i]);
        if((outfp = fopen(Playlist_of_the_day[i], "w")) ==NULL )
            printf("cannot open the file %s\n");
		songCounter=0; //resets counter to 0 per day
		readPlaylist(songlist, &songCounter);
        shuffle(songlist, songCounter);
        for(j=0; j<songCounter; j++){ 
            fputs(songlist,outfp);
            songlist+=100;   
        }  
        fclose (outfp);
    }
}

int main () {
    char *favorites;
    int rows = 5000;//allocates memory  for 5000 songs
    int cols = 100;//each songs has a maximum of 100 characters
	time_t t;
	gettimeofday(&t,NULL);//seed for time
    srand((unsigned) time(&t));
    favorites = (char*)calloc(rows*cols, sizeof(char));
    writePlaylist(favorites );
    free(favorites);
    return 0;
}

