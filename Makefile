main: app.c channel.c user.c
	gcc -ggdb -o main app.c channel.c user.c -lm

clean:
	rm -rf *.o main