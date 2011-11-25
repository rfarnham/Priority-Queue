gcc heap.c -o heap.o -c
gcc test.c -o test.o -c
gcc test.o heap.o -o test
