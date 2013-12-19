all:
	gcc -c FunDef.c -o FunDef.o
	gcc -c Display.c -o Display.o
	gcc FunDef.o Display.o Main.c -lm -o main
exec:
	./main
clean:
	rm main
