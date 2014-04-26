

/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

  This program can be distributed under the terms of the GNU GPL.
  See the file COPYING.

  gcc -Wall hello.c `pkg-config fuse --cflags --libs` -o hello
*/

#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdlib.h>


/*BUILTIN STRUCT CONTAINING ALL THE INFO AND FUNCTIONS OF THE MATH DIRECTORIES*/
struct builtin
{
    char *name; /* name of path */
    char *doc_contents;
    int param;
    char *(*f)(); /* function to execute for the path */
};



/*DOC FILE CONTENTS*/
const char * add_doc = "Adds 2 numbers together.\nThe file add/a/b contains a added to b.";
const char * sub_doc = "Subtracts 2 numbers.\nThe file sub/a/b contains b subtracted from a.";
const char * mul_doc = "Multiplies 2 numbers together.\nThe file mul/a/b contains a multiplied to b.";
const char * div_doc = "Divides 2 numbers.\nThe file div/a/b contains a divided by b.";
const char * fib_doc = "Fibonacci Sequence.\nThe file fib/a contains the first a-th elements of the sequence.";
const char * factor_doc = "Finds prime factors.\nThe file factor/a finds all prime factors of a.";
const char * exp_doc = "Raises a number to an exponent.\nThe file exp/a/b contains a raised to b power.";

/**************** MATH FUNCTIONS**************/
char* add(char* a, char* b) {
	double c = strtod(a, NULL);
	double d = strtod(b, NULL);
	
	char buf[1024] = "";
	sprintf(buf, "%f\n", c + d);
	
	return buf;
	
}

char* sub(char* a, char* b) {
	double c = strtod(a, NULL);
	double d = strtod(b, NULL);
	
	char buf[1024] = "";
	sprintf(buf, "%f\n", c - d);
	
	return buf;
}

char* mul(char* a, char* b) {
	double c = strtod(a, NULL);
	double d = strtod(b, NULL);
	
	char buf[1024] = "";
	sprintf(buf, "%f\n", c * d);
	
	return buf;
}

char* div1(char* a, char* b) {
	double c = strtod(a, NULL);
	double d = strtod(b, NULL);
	
	char buf[1024] = "";
	if(d == 0)
		return "ERROR: divide by zero";
	sprintf(buf, "%f\n", c / d);
	
	return buf;
}

char* exp1(char* a, char* b) {
	double c = strtod(a, NULL);
	double d = strtod(b, NULL);
	
	char buf[1024] = "";
	sprintf(buf, "%f\n", pow(c,d));
	
	return buf;
}

char* fib(char* a, char* b) {
	char buf[1024];
	char str[1024];
	strcpy(str, "1\n1\n");
	int i = 2;
	long long j = 1;
	long long k = 1;
	long long fib;
	double limit = strtod(b, NULL);
	if(fabs(limit - floor(limit)) > 0.001)
		return "Must be an integer";
	while( i < limit) {
		fib = j + k;
		sprintf(buf, "%lld\n", fib);
		strcat(str, buf);
		k = j;
		j = fib;
		i += 1;
	}
	
	return str;
}

long long isPrime(int n){
	int i;
	int count = 0;
	long long lim = (long long) ceil(sqrt((double) n));
	for(i = 1; i < lim; i++)
	{
		if(n % i == 0)
			count++;
		if(count >= 2)
			return 0;
	}
	return 1;
}

char* factor(char* a, char* b) {
	char buf[1024];
	char str[1024];
	strcpy(str, "");
	long long i;
	double limit = strtod(b, NULL);
	if(fabs(limit - floor(limit)) > 0.001)
		return "Must be an integer";
	
	long long n = (long long) limit;
	
	long long int_lim = (long long) ceil(sqrt(limit));
	
	
		
	if(isPrime(n))
	{
		sprintf(buf, "%lld\n", n);
		strcat(str, buf);
	}
	for(i = 2; i < int_lim; i++)
	{
		if(isPrime(i) == 1)
			if(n % i == 0)
			{
				sprintf(buf, "%lld\n", i);
				strcat(str, buf);
			}
	}
	return str;
}


/*TABLE OF STRUCTS*/
const struct builtin builtin[] = {
	{"add", "Adds 2 numbers together.\nThe file add/a/b contains a added to b.", 3, add},
	{"sub", "Subtracts 2 numbers.\nThe file sub/a/b contains b subtracted from a.", 3, sub},
	{"mul", "Multiplies 2 numbers together.\nThe file mul/a/b contains a multiplied to b.", 3, mul},
	{"div", "Divides 2 numbers.\nThe file div/a/b contains a divided by b.", 3, div1},
	{"fib", "Fibonacci Sequence.\nThe file fib/a contains the first a-th elements of the sequence.", 2, fib},
	{"exp", "Finds prime factors.\nThe file factor/a finds all prime factors of a.", 3, exp1},
	{"factor", "Raises a number to an exponent.\nThe file exp/a/b contains a raised to b power.", 2, factor},
	{0, 0, 0, 0}
};

/************** FUSE APIs****************/
static int mathfs_getattr(const char *path, struct stat *stbuf)
{
	int res = 0;
	char path2[40];
	char *path_split[1024];
	int i = 0;
	memset(stbuf, 0, sizeof(struct stat));
	
	memcpy(path2, path, strlen(path) + 1);
	
	path_split[i] = strtok(path2, "/");
	if(path_split[i] == NULL) {
		path_split[i] = "/";
		
		stbuf->st_mode = S_IFDIR | 0775;
		stbuf->st_nlink = 9;
		return 0;
	}
	while(path_split[i] != NULL) {
		i++;
		path_split[i] = strtok(NULL, "/");
	}
	
	int j = 0;
	
	while(builtin[j].name != 0) {
		if(strcmp(path_split[0], builtin[j].name) == 0) {
			if(i == 1){
				stbuf->st_mode = S_IFDIR | 0775;
				stbuf->st_nlink = 3;
				return 0;
			}
			else {
				if(strcmp(path_split[1], "doc") == 0) {
					stbuf->st_mode = S_IFREG | 0664;
					stbuf->st_nlink = 3;
					stbuf->st_size = strlen(builtin[j].doc_contents);
					return 0;
				}
				else if(i == builtin[j].param) {
					stbuf->st_mode = S_IFREG | 0664;
					stbuf->st_nlink = 3;
					stbuf->st_size = strlen(builtin[j].f(path_split[i-2],path_split[i-1]));
					return 0;
				}
				else if(i > builtin[j].param) {
					return -ENOENT;
				}
				else {
					stbuf->st_mode = S_IFDIR | 0775;
					stbuf->st_nlink = 3;
					return 0;
				}
			}
		}
		j++;
	}
	
	if(builtin[j].name == 0)
		res = -ENOENT;
	return res;
}

static int mathfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			 off_t offset, struct fuse_file_info *fi)
{
	(void) offset;
	(void) fi;

	if (strcmp(path, "/") == 0){
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		filler(buf, "add", NULL, 0);
		filler(buf, "sub", NULL, 0);
		filler(buf, "mul", NULL, 0);
		filler(buf, "div", NULL, 0);
		filler(buf, "exp", NULL, 0);
		filler(buf, "fib", NULL, 0);
		filler(buf, "factor", NULL, 0);
	}
	else if (strcmp(path, "/add") == 0){
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		filler(buf, "doc", NULL, 0);
	}
	else if (strcmp(path, "/sub") == 0){
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		filler(buf, "doc", NULL, 0);
	}
	else if (strcmp(path, "/mul") == 0){
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		filler(buf, "doc", NULL, 0);
	}
	else if (strcmp(path, "/div") == 0){
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		filler(buf, "doc", NULL, 0);
	}
	else if (strcmp(path, "/fib") == 0){
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		filler(buf, "doc", NULL, 0);
	}
	else if (strcmp(path, "/exp") == 0){
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		filler(buf, "doc", NULL, 0);
	}
	else if (strcmp(path, "/factor") == 0){
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		filler(buf, "doc", NULL, 0);
	}
	else
		return -ENOENT;
	return 0;
}



static int mathfs_open(const char *path, struct fuse_file_info *fi)
{
	char path2[40];
	char *path_split[1024];
	int i = 0;
	
	memcpy(path2, path, strlen(path) + 1);
	
	path_split[i] = strtok(path2, "/");
	if(path_split[i] == NULL) {
		path_split[i] = "/";
		i++;
	}
	while(path_split[i] != NULL) {
		i++;
		path_split[i] = strtok(NULL, "/");
	}
	
	int j = 0;
	
	while(builtin[j].name != 0) {
		if(strcmp(path_split[0], builtin[j].name) == 0) {
			if(strcmp(path_split[1], "doc") == 0) {
				if ((fi->flags & 3) != O_RDONLY)
					return -EACCES;
			}
			else if(i == builtin[j].param) {
				if ((fi->flags & 3) != O_RDONLY)
					return -EACCES;
			}
			
			break;
		}
		j++;
	}
	
	if(builtin[j].name == 0)
		return -ENOENT;
/*
	if (strcmp(path, hello_path) != 0)
		return -ENOENT;

	if ((fi->flags & 3) != O_RDONLY)
		return -EACCES;
*/
	return 0;
}

static int mathfs_read(const char *path, char *buf, size_t size, off_t offset,
		      struct fuse_file_info *fi)
{
	size_t len;
	(void) fi;
	
	char path2[40];
	char *path_split[1024];
	int i = 0;
	memcpy(path2, path, strlen(path) + 1);
	
	path_split[i] = strtok(path2, "/");
	if(path_split[i] == NULL) {
		path_split[i] = "/";
		i++;
	}
	while(path_split[i] != NULL) {
		i++;
		path_split[i] = strtok(NULL, "/");
	}
	
	int j = 0;
	
	while(builtin[j].name != 0) {
		if(strcmp(path_split[0], builtin[j].name) == 0) {
			if(strcmp(path_split[1], "doc") == 0) {
				len = strlen(builtin[j].doc_contents);
				if (offset < len) {
					if (offset + size > len)
						size = len - offset;
					memcpy(buf, builtin[j].doc_contents + offset, size);
				} 
				else
					size = 0;
				return size;
			}
			else if(i == builtin[j].param) {
				len = strlen(builtin[j].f(path_split[i-2], path_split[i-1]));
				if (offset < len) {
					if (offset + size > len)
						size = len - offset;
					memcpy(buf, builtin[j].f(path_split[i-2], path_split[i-1]) + offset, size);
				} 
				else
					size = 0;
				return size;
			}
			
			break;
		}
		j++;
	}
	
	if(builtin[j].name == 0)
		return -ENOENT;
	
	return 0;
}




static struct fuse_operations hello_oper = {
	.getattr	= mathfs_getattr,
	.readdir	= mathfs_readdir,
	.open		= mathfs_open,
	.read		= mathfs_read,
};




int main(int argc, char *argv[])
{
	
	return fuse_main(argc, argv, &hello_oper, NULL);
} 
