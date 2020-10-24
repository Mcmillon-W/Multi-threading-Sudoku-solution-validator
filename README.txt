multi.c has the answer to the first question.

To visualise better that the PIDs are getting allocated and released
simultaneously.
It is recommended that
line 76 be  ->   sleep((double)rand()/(double)RAND_MAX*1); 

But if you want to see that the threads wait till a PID is assigned to them
decrease the range of PIDs available 
by changing line 9    ->     #define MAX_PID 500

and line 76  -> sleep((double)rand()/(double)RAND_MAX*100);   So that they sleep for longer time making other threads to wait longer

These are just for visualisation purposes and is recommended just to save time.
Otherwise the code will work fine and give the intended results. (ignore the warnings)





Pthread.c is the solution to the second. We have 11 threads.
1 thread to check all columns
1 thread to check all rows
9 threads to check all subgrids 
For it to work properly
the sudoku file just like input-Sudoku.txt,sample1.txt,sample2.txt has to passed as an argument.

like  ->    ./a.out input-Sudoku.txt

It will work for all test cases of normal 9*9 sudoku.


For both the files, to compile properly (-w to ignore warnings)
(linux machine)
gcc filename.c -lpthread -w