Included is a copy of the filesystem used for testing, as “filesystem.zip”. This contains the c files as well. The goal of this application was to create a binary search program, which looks through all directories at a lower level than the current, looking for matches on a given file fragment. The program is targeted at binary and text filetypes. The first program is sequential and included is an OpenMP and a Pthread implementation of parallel versions of the program.

The following programs have been tested be compatible with Linux based OS only.

//Compiling
To compile the programs, use the following commands: 
	Direc.c: 'gcc Direc.c -lm'
	DirecOMP.c: 'gcc DirecOMP.c -lm -fopenmp'
	DirecPthread.c: 'gcc DirecPthread.c -lm -fopenmp'

//Execution
The program takes the following arguments:
	-h: Help menu
	-f: The filename containing the fragment
	-a: The required accuracy of the match

The run commands used must be:
	Direc.c: './a.out -f FILENAME'
	DirecOMP.c: './a.out -f FILENAME'
	DirecPthread.c: './a.out -f FILENAME'

where FILENAME is the name of the file to be used.
 	

//Output
	Each program should output the Filesize of the given fragment/file, and the Acc, or the max number of bytes of difference the file may have. This is followed by notifications, if any, that a match has been found on the fragment, denoted by "Fragment match found!". The program should then tell you the relative path of the matching file, the accuracy, the position 
