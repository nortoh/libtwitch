main: app.c channel.c
	gcc -ggdb -o main app.c channel.c -lm

clean:
	rm -rf *.o main