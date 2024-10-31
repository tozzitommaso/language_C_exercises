#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>


void controllaParametri(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Non hai inserito i parametri necessari \n");
		printf("Uso: $./server <porta>\n");
		exit(0);
	}
}

int main(int argc, char *argv[])
{

	controllaParametri(argc, argv);

	struct sockaddr_in servizio, rem_indirizzo;
	int pid, soa, socketfd, on = 1, fromlen = sizeof(servizio);

	memset((char *)&servizio, 0, sizeof(servizio));

	servizio.sin_family = AF_INET;
	servizio.sin_addr.s_addr = htonl(INADDR_ANY);
	servizio.sin_port = htons(atoi(argv[1]));

	socketfd = socket(AF_INET, SOCK_STREAM, 0);

	setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	bind(socketfd, (struct sockaddr *)&servizio, sizeof(servizio));
	listen(socketfd, 10);

	for (;;)
	{
		printf("\n\nServer in ascolto...\n");
		fflush(stdout);

		soa = accept(socketfd, (struct sockaddr *)&rem_indirizzo, &fromlen);

		pid = fork();

		if (pid == 0)
		{
			char nome_file[100];
			close(socketfd);
			int nread = read(soa, nome_file, sizeof(nome_file));
			nome_file[nread] = '\0';
			close(1);
			dup(soa);
			close(soa);
			execl("/usr/bin/cat", "cat", nome_file, (char *)0);
			return -1;
		}
		close(soa);
	}
	close(socketfd);

	return 0;
}