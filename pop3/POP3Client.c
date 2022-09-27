#include <stdio.h> /* for printf() and fprintf() */ 
#include <sys/socket.h> /* for socket(), sendto() and recvfrom() */
#include <arpa/inet.h> /* for sockaddr_in and inet_addr() */
#include <stdlib.h> /* for atoi() and exit() */
#include <string.h> /* for memset() */
#include <unistd.h> /* for close() */
#include <netdb.h> /* for hostent and gethostbyname() */

#define POP3_PORT 110
#define RESPONSE_BUFFER_SIZE 255

void GetNumEmail();

int main(int argc, char *argv[])
{
	int sock; /* Socket descriptor */
	struct sockaddr_in servAddr; /* server address */ 
	char *serverName; /* IP address of server */
	struct hostent *host;

	if(argc != 2) 
	{
		printf("Usage: %s [hostname]\n", argv[0]);
		exit(1);
	}

	serverName = argv[1];

	/* translate the domain Name to IP address */
	if((host = gethostbyname(argv[1])) == NULL) {
		perror("fail to get host by name\n");
		exit(1);
	}

	/* Create a TCP socket */
	if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1 )
		printf("socket() failed.\n");

	/* Construct the server address structure */
	memset(&servAddr, 0, sizeof(servAddr));/*Zero out structure*/
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(POP3_PORT);
	//serverName="POP3.163.com";
	servAddr.sin_addr = *((struct in_addr *)host -> h_addr);

	/* connect to server */
	char response_buf_connect[RESPONSE_BUFFER_SIZE]={0};
	if(connect(sock, (struct sockaddr *)&servAddr, sizeof(servAddr))==-1){
		printf("connect failed");
		exit(1);
	}
	if( recv(sock,response_buf_connect,RESPONSE_BUFFER_SIZE,0)==-1 ){
		printf("recv() failed\n");
		return -1;
	}
	printf("%s",response_buf_connect);/* read the connect response */

	printf("Success to connect the server!\n");
	printf("Welcome to join %s!\n", inet_ntoa(servAddr.sin_addr));
	
	//sleep(3000);
	// printf("Please enter your user name\n>>>");
	// char user_email[30]; 
	// scanf("%s", user_email);
	// char user1[10];
	// strcpy(user1, "USER");
	// strcpy(user,strcat(user1, user_email));
	// printf("%s\n",user);

	/* sign in USER */
	char user[30]={"USER maruyamaayabupt@163.com"};
	char response_buf_USER[RESPONSE_BUFFER_SIZE]={0};
	if(send(sock,user,strlen(user),0)!=strlen(user)){
		printf("send() sent a different number of bytes than expected.\n");
	}
	else{
		printf("USER Successfully send\n");
	}
	if( recv(sock,response_buf_USER,RESPONSE_BUFFER_SIZE,0)==-1 )
		printf("recv() failed\n");
		return -1;
	printf("%s\n",response_buf_USER);

	//sleep(3000);

	/* sign in PASS */
	char* password={"PASS RBRGWYPWYPKXFMJC"};
	char response_buf_PASS[RESPONSE_BUFFER_SIZE]={0};
	if(send(sock,password,strlen(password),0)!=strlen(password)){
		printf("send() sent a different number of bytes than expected.\n");
	}
	else{
		printf("PASS Successfully send\n");
	}
	if( recv(sock,response_buf_PASS,RESPONSE_BUFFER_SIZE,0)==-1 )
		return -1;
	printf("%s\n",response_buf_PASS);

 	GetNumEmail(sock);
	close(sock);

	exit(0);
}

//获取邮件的数量
void GetNumEmail(int connection)
{
	char sStat[10]={"STAT"};
	char response_buf[RESPONSE_BUFFER_SIZE]={0};

	send(connection,sStat,strlen(sStat),0);
	if( recv(connection,response_buf,RESPONSE_BUFFER_SIZE,0)==-1 )
		printf("receive error.\n");

	char szResponse[20]={0};
	strncpy(szResponse,response_buf,3);
	printf("%s\n",szResponse);


}

















