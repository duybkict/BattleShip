#include "unp.h"

/**
 * make and then "./tcpcli01 [filename]" to run
 *
 */
int main(int argc, char **argv)
{
	struct sockaddr_in servaddr;
	int sockfd;

	// Check for valid arguments
	if (argc < 2) {
		err_quit("usage: tcpcli <IPaddress>");
	}

	// Connect sockets between client and server
	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));
	
	// The third argument should be the input file name
	FILE * file = fopen(argv[2], "r");	 
	if (file != NULL) {
		str_cli(file, sockfd);
	} else {
		// If not, input will be provided from keyboard	
		str_cli(stdin, sockfd);
	}
	
	fclose(file);
	exit(0);
}
