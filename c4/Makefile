51033_H4_Main: 51033_H4.o 51033_H4_Main.c 51033_H4.h
	gcc  51033_H4_Main.c 51033_H4.o -lm -o 51033_H4_Main

51033_H4.o: 51033_H4_Fun.c 51033_H4.h
	gcc -c 51033_H4_Fun.c -lm -o 51033_H4.o

clean:
	rm 51033_H4_Main
	rm 51033_H4.o
	rm lena_*.ppm
	rm Parrots_*.ppm
	rm  *.csv
