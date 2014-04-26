all: 
		gcc -Wall mathfs.c `pkg-config fuse --cflags --libs` -lm -w -o mathfs
