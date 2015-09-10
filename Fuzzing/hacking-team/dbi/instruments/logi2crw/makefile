LD=~/android/toolchain/arm-2008q3/bin/arm-none-linux-gnueabi-ld
CC=~/android/toolchain/arm-2008q3/bin/arm-none-linux-gnueabi-gcc

all:
	(cd hijack_func; ./build.sh)
	$(CC) -c libt.c -o libt.o -Ilibnfc-nxp/inc -Ilibnfc-nxp/src
	$(CC) -c util.c -o util.o
	$(LD) -shared -Llibs -lc -soname,libt.so -o libt.so libt.o util.o hijack_func/*.o

clean:
	rm -f hijack_func/*.o
	rm *.o libt.so
	rm hijacks.h
