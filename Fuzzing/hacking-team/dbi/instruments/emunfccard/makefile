LD=~/android/toolchain/arm-2008q3/bin/arm-none-linux-gnueabi-ld
CC=~/android/toolchain/arm-2008q3/bin/arm-none-linux-gnueabi-gcc

all:
	(cd hijack_func; ./build.sh)
	$(CC) -c libt.c -o libt.o -Ilibnfc-nxp/inc -Ilibnfc-nxp/src
	$(CC) -c util.c -o util.o
	$(CC)  com.c -o com -static
	$(CC) -c ifc.c -o ifc.o -Ilibnfc-nxp/inc -Ilibnfc-nxp/src
	$(LD) -shared -Llibs -lc -soname,libt.so -o libt.so libt.o util.o ifc.o hijack_func/*.o

clean:
	rm -f *.o libt.so com
	rm -f hijack_func/*.o
