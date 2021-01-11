#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <dirent.h>
#include <string.h>

int np = 0;

void usage()
{
	//Describes the usage/help summary for the program.

	printf("A program used for searching filesystems.\n");
	printf("Usage Summary:\n");
	printf("	-h: Help options\n");
	printf("	-f: File or fragment to open\n");
	printf("	-a: Search accuracy\n");
}

void search(FILE *fptr, double acc, double len, char *filename, char *dirname)
{
	DIR *d;
	struct dirent *dir;
	int y = 0;
	int i = 0;
	int ii = 0;
	FILE *tempfile = NULL;	//The current comparrison file
	long fileplc;			//File place
	char j;				//Current character from given file
	char k;				//Current character from file in current comparison
	long tempplace;			//Holds the file place for backwards iterations
	char tempfilename[255];		//Holds the name of the current comparrison file.
	double tempacc;			//A temp value to count how many error bytes we have left
	char path[255];
	path[0] = '\0';
	char *slash = "/";
	strcat(strcat(path, dirname), slash);
	int thing = 0;


	//printf("Dirname is %s\n", dirname);
	d = opendir(dirname);
	if (d)
	{
		//printf("Opened dir\n");

		while ((dir = readdir(d)) != NULL) 									//Loops through directory
		{
			strcpy(tempfilename, path);
			//printf("\n\nTempfile name is now %s\n\n", tempfilename);
      		//printf("%s\n", dir->d_name);
      		strcat(tempfilename, dir->d_name);
      		//path[strlen(path) - 1] = '\0';
           	//printf("Path is %s, tempfilename is %s\n", path, tempfilename);
      		if((tempfile = fopen(tempfilename, "rb")) == NULL)				//Open tempfile
				printf("Error opening test!");							
			if(strcmp(filename, tempfilename) && dir->d_type != DT_DIR)		//See if it's our given file, or if it's a directory. If not, we proceed.
			{
				thing = 0;

				
				for(ii = 0; ii < len-acc; ii++)
				{
					//
					rewind(fptr);
					fseek(fptr, ii, SEEK_SET);
					do 
					{
						//printf("\n\nNew It 2\n\n");
						tempplace = ftell(tempfile);				//Hold the current position of our tempfile
						tempacc = acc - ii;

						#pragma omp parallel for private(j, k, thing)
						for(i = 0; i < (int) len; i++)
						{
							if(thing != 1)
							{


							#pragma omp critical
							{	
							j = fgetc(fptr);
							k = fgetc(tempfile);
							}
							if (j != k)
							{
								tempacc--;
								if(j == '\n')
									j = ';';
								if(k == '\n')
									k = ';';
								//fileplc = ftell(fptr);
								//printf("Character number: %ld       \t", fileplc);
								//printf("%c     %c         Error room left: %lf\n", j, k, tempacc);
				
							}

							if(tempacc <= 0 || feof(tempfile))
								thing = 1;
							}
						}

						thing = 0;

					
						if(feof(tempfile))
							break;
						if(tempacc > 0)
						{
							y = 1;
							break;
						}
					
						rewind(fptr);
						fseek(fptr, ii, SEEK_SET);
						fseek(tempfile, (tempplace + 1), SEEK_SET);
					}while(!feof(tempfile));

					if(tempacc > 0 && y)
					{
						printf("\n\nFragment match found!\nFile in %s has accuracy of %lf%%\n", tempfilename, ((len - (acc - tempacc))/len*100));
						rewind(tempfile);
						fseek(tempfile, (tempplace), SEEK_SET);
						fileplc = ftell(tempfile);
						printf("File position on match is: %ld\n", fileplc);
						printf("Starting at text matching: ");
						for(y = 0; y < acc; y++)
						{
							k = fgetc(tempfile);
							printf("%c", k);
						}
						y = 0;
						printf("\n");
						break;
					}
					if(tempacc <= 0)
						break;
					rewind(tempfile);

				}


			}
			else if(dir->d_type == DT_DIR && strcmp(dir->d_name, "..") && strcmp(dir->d_name, "."))
			{
					//printf("Searching new dir %s\n", tempfilename);
					//strcat(path, strcat("/", tempfilename));
					//strcat(path, tempfilename);
					//printf("Path is now %s\n", tempfilename);
					search(fptr, acc, len, filename, tempfilename);
			}
			fclose(tempfile);
			memset(tempfilename,0,strlen(tempfilename));
			//return 0;
		}
	closedir(d);
	}
	else
		printf("Error opening dir!\n");
}

int main (int argc, char *argv[])
{
	
	int opt = 0; 			//options menu
	double accuracy = 90;	//The accuracy in %form
	double acc;   			//How many bytes we can get wrong.
	int type;
	long len;				//Length of file
	char *filename;
	FILE *fptr = NULL;		//The given file

	

	//Options menu
	opt = getopt( argc, argv,"ht:f:a:" );
	//Options Menu
	while (opt != -1)
	{
		switch (opt)
		{
			case 'h':
                		usage();
                		return 0;


			case 't':
					type = atoi(optarg);
					break;

			case 'f':
					filename = optarg;
					if((fptr = fopen(filename, "rb")) == NULL)
					{
						printf("Error opening given file!\n");
						return 0;
					}
					break;


			case 'a':
					accuracy = atoi(optarg);
					break;
				


		}

		opt = getopt( argc, argv, "ht:f:a:" );
	}
	

	
	accuracy = 100-accuracy;				//Inverses the accuracy to see what percent of the file we can get wrong
	fseek(fptr, 0, SEEK_END);				//Finds the end
	len = ftell(fptr);						//Then records the length of the given file
	printf("Filesize is: %ld\n", len);		//Prints filesize
	rewind(fptr);							//Go back to start
	acc = trunc((accuracy/100)*len);		//Records the number of bytes we can miss
	printf("Acc is: %lf\n", acc);



	search(fptr, acc, len, filename, ".");


	fclose(fptr);
	return 0;
}