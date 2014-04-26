Project Title: Mathfs

Project Description:
Using FUSE (File system in UserSpace) to implement a virtual filesystem. The 
file system implement does not store actual files but performs various math
operations based on the names and numbers given to it. Mathfs has the given
characteristics:


The root of the mathfs file system must contain seven directories, each 
representing a mathematical function:
	/factor - Computes the prime factors of a number.
        /fib - Computes the first n fibonacci numbers.
        /add - Adds two numbers
        /sub - Subtracts two numbers.
        /mul - Multiplies two numbers.
        /div - Divides two numbers.
        /exp - Raises a number to a given exponent.

An ls command executed on any of these directories should only show a single
file: doc. The contents of this file should be the documentation of the
particular function. This can be a brief textual description with a couple of
examples.

A function can be invoked by opening a "file" under the mathfs mount point and
reading its contents. For example:
	/factor/138 should act as a file containing the numbers 2, 3, 23, one
	per line and ending with a newline.
        /fib/3 should act as a file containing the numbers 1, 1, 2, one per line and ending with a newline.
        /add/5/3 should act as a file containing the number 8 and ending with a newline.
        /sub/5/3 should act as a file containing the number 2 and ending with a newline.
        /mul/5/3 should act as a file containing the number 15 and ending with a newline.
        /div/5/3 should act as a file containing the number 1.6666 and ending with a newline.
        /exp/2/3 should act as a file containing the number 8 and ending with a newline.

        Note that these "files" should not be listed by ls, as there are an infinite number of them.

How to run:

First make sure FUSE is installed. Installation instructions can be seen here:

http://fuse.sourceforge.net/

Then after downloading all the files into a directory, run make in the terminal.
This should create a file named mathfs. Create a directory, in our case, we will
call it math/. Run ./mathfs math. The system is now mounted to math/. Type ls 
math. The 7 math directories should appear. 

After finishing, unmount using fusermount -u math/. 
