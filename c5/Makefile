51033_H4_Main: 51033_H4.o 51033_H4_Main.c 51033_H4.h
	gcc -Wall  51033_H4_Main.c 51033_H4.o -lm -o 51033_H4_Main

51033_H4.o: 51033_H4_Fun.c 51033_H4.h
	gcc -Wall -c 51033_H4_Fun.c -lm -o 51033_H4.o

exec:
	./51033_H4_Main
debug:
	gcc -g 51033_H4_Fun.c 51033_H4_Main.c -lm -o 51033_H4_Main

clean:
	rm 51033_H4_Main ; 
	rm 51033_H4.o ; 
	rm  *.csv ; 
	rm lena_*.ppm ; 
	rm Parrots_*.ppm ; 
	rm 锐化后的lena.ppm ; 
	rm 边缘检测后的lena.ppm
