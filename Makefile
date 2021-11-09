main: app.c channel.c user.c
	gcc -pthread -ggdb -o main app.c channel.c user.c bot.c utils.c t_socket.c -lm

clean:
	rm -rf *.o main