main: app.c channel.c user.c bot.c utils.c t_socket.c config.c
	gcc -g -pthread -ggdb -o main app.c message.c channel.c user.c bot.c irc_message.c badge.c tag.c utils.c t_socket.c config.c -lm

clean:
	rm -rf *.o main
