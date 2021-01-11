//Honey, Andy
//Final Exam
//Operating Sys&Parallel Programming


The following programs have been tested be compatible with Linux based OS only.

//Compiling
To compile the programs, use the following commands: 
	hps.c: 'gcc hps.c -o hps.out'
	hpsomp.c: 'gcc hpsomp.c -fopenmp -o hpsomp.out'
	hpsmpi.c: 'mpicc hpsmpi.c -o hpsmpi.out'

//Execution
The program takes two arguments, n and d. These are position dependent arguments, not flags.

The run commands used must be:
	hps.c: './hps.out N D'
	hpsomp.c: './hpsomp.out N D'
	hpsmpi.c: 'mpirun -np P hpsmpi.out N D'

where N is the number of fractions to add, D is the number of digits to display/calculate to, and P is the numer of processes to use.

To change the number of threads/processors in hpsomp.c, run the command: 'export OMP_NUM_THREADS=NP', where NP is substituted for your number.

//Runtime
The runtime is processor dependent, however a baseline of N*D/10^8s can be used for the sequential program. The estimated execution time subject to parallelization is estimated at 80% of the program.

The run time varies, increasing exponentially as N and D increase. The following are run times are tested examples of N = 1,000,000 and D = 100 
 	hps.c: 
		1.856s
 	hpsomp.c:
		np=1: 1.866s
		np=2: 0.938s - .9s
		np=3: 0.628s - 1.2s
		np=4: 0.484s - 1.4s

 	hpsmpi.c:
		np=1: 2.146s
		np=2: 1.321s
		np=3: 1.097s
		np=4: 0.896s
 	
One will note that using hpsmpi.c hinders performance at np=1, and is outperformed by hpsomp.c. This is due to the startup and ending time of MPI functions, so for lower numbers, hpsomp.c is more useful due to less overhead.

Program execution time can be estimated more accurately by finding the product of N and D, where in the above example, N*D = 10^8. It is advised to use hps.mpi for programs with an ND product > 10^8, as the overhead begins to be outweighed by the speed of the program at this point. 

//Output
	Each program should output sum of 1/1 + 1/2 + 1/3... 1/n in decimal form, with decimal places d.