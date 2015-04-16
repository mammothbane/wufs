typically:	mkfs.wufs

mkfs.wufs:	mkfs.wufs.c wufs.c wufs.h wufs_fs.h 
	gcc -g -Wall -o mkfs.wufs mkfs.wufs.c wufs.c

install:
	@echo '(Install in /sbin to use with mkfs -t wufs.)'

clean:	
	@rm -f mkfs.wufs *.o *~ \#*
	@echo Clean.

dump:
	@mkfs.wufs a4.img
	@od -A x -ax < a4.img > a4.od
	@echo dump of a4.img written to a4.od

