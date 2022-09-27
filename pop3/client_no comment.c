#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termio.h>
#include <sys/shm.h>
#include <netdb.h>
 
#define PORT 110
#define BUFFER_SIZE 10000

unsigned char *base64_decode(unsigned char *str);
unsigned char *base64_encode(unsigned char *str);


static int find_string(char *strs[], char *str, int len)
{
    int i = 1;
    while(i < len + 1){
        if(strcmp(*strs, str) == 0){
            break;
        }
        i++;
        strs++;
    }
    if(i == len + 1){
        return -1;
    }
    return i;
}

int mygetch()
{
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

int getpasswd(char *passwd, int size)
{
    int c, n = 0;
    do
    {
        c = mygetch();
        if (c != '\n' && c != 'r' && c != 127)
        {
            passwd[n] = c;
            printf("*");
            n++;
        }
        else if ((c != '\n' | c != '\r') && c == 127)
        {
            if (n > 0)
            {
                n--;
                printf("\b \b");
            }
        }
    }while (c != '\n' && c != '\r' && n < (size - 1));
    passwd[n] = '\0';
    return n;
}

char *strrpc(char *dest,char *src,char *before,char *after){
    size_t src_size = strlen(src);
    size_t before_str_size = strlen(before);
    size_t after_str_size = strlen(after);
 
    size_t dest_offset = 0;
    size_t src_offset = 0;
    for(src_offset = 0;src_offset < src_size; ){
        if(!strncmp(src + src_offset,before,before_str_size)){
            memcpy(dest + dest_offset,after, after_str_size);
            src_offset += before_str_size;
            dest_offset += after_str_size;
        }else{
            memmove(dest + dest_offset,src + src_offset,1);
            dest_offset++;
            src_offset++;
        }
    }
 
    return dest;
}

int main(int argc, char *argv[])
{
    int sock_cli;
    fd_set rfds;
    struct timeval tv;
    int retval, maxfd;
    struct hostent *host;
    int count;
    char* str1;



	if((host = gethostbyname(argv[1])) == NULL) {
		perror("fail to get host by name\n");
		exit(1);
	}


    sock_cli = socket(AF_INET,SOCK_STREAM, 0);
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr = *((struct in_addr *)host -> h_addr); 

    
    if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect\n");
        exit(1);
    }else{
        char recvbuf[BUFFER_SIZE];
        int len;
        len = recv(sock_cli, recvbuf, sizeof(recvbuf),0);
        printf("%s", recvbuf);
        memset(recvbuf, 0, sizeof(recvbuf));
    }

    printf("Please enter your email address\n");


            char sendbuf[BUFFER_SIZE]="USER maruyamaayabupt@163.com\n";

            printf("%s", sendbuf);
            if(send(sock_cli, sendbuf, strlen(sendbuf),0)!=strlen(sendbuf))
                printf("send() error");
            memset(sendbuf, 0, sizeof(sendbuf));
        //}
        FD_ZERO(&rfds);
        //FD_SET(0, &rfds);
        maxfd = 0;
        if(maxfd < sock_cli)
            maxfd = sock_cli;
        FD_SET(sock_cli, &rfds);
        tv.tv_sec = 300;
        tv.tv_usec = 0;
        retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
        if(FD_ISSET(sock_cli,&rfds))
        {
            char recvbuf[BUFFER_SIZE];
            char recvbufcpy[BUFFER_SIZE];
            int len;
            len = recv(sock_cli, recvbuf, sizeof(recvbuf),0);
            printf("%s", recvbuf);
            strcpy(recvbufcpy,recvbuf);
            memset(recvbuf, 0, sizeof(recvbuf));
            str1 = "+OK";
        }
    //}

    sleep(1);

    printf("Please enter your password\n");


             char passWord[25];
             printf("请输入密码:\n");
             getpasswd(passWord, 25);
             printf("\n");
             printf("你输入的密码是:%s\n", passWord);

             char sendbuf_pass[BUFFER_SIZE]="PASS ";
             strcat(sendbuf_pass,passWord);
             strcat(sendbuf_pass,"\n");
             send(sock_cli, sendbuf_pass, strlen(sendbuf_pass),0); //
             sleep(2);
             memset(sendbuf_pass, 0, sizeof(sendbuf_pass));
            printf("%s", sendbuf_pass);
            if(send(sock_cli, sendbuf_pass, strlen(sendbuf_pass),0)!=strlen(sendbuf_pass)) 
                printf("send() error");
            memset(sendbuf_pass, 0, sizeof(sendbuf_pass));
        //}
        FD_ZERO(&rfds);

        maxfd = 0;
        if(maxfd < sock_cli)
            maxfd = sock_cli;
        FD_SET(sock_cli, &rfds);
        tv.tv_sec = 300;
        tv.tv_usec = 0;
        retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
        if(FD_ISSET(sock_cli,&rfds))
        {
            char recvbuf[BUFFER_SIZE];
            char recvbufcpy[BUFFER_SIZE];
            int len;
            len = recv(sock_cli, recvbuf, sizeof(recvbuf),0);
            printf("%s", recvbuf);
            strcpy(recvbufcpy,recvbuf);
            memset(recvbuf, 0, sizeof(recvbuf));
            str1 = "+OK";
            if(strncmp(str1,recvbufcpy,3)==0)

                printf("Successfully login!\n");

        }
    //}

    printf("\n***** Welcome to the POP3 client side! *****\n");
    sleep(3);
    system("clear");
    count = 0;


    while(1){
        if (count == 0){
            printf("*********************************************************\n");
            printf("1. Show email status \n");
            printf("2. Check email list \n");
            printf("3. Display email in detail \n");
            printf("4. Search text in all mails \n");
            printf("5. Display by subjects \n");
            printf("6. Download the mail and delete in the remote service \n");
            printf("7. Quit the email \n");
            printf("*********************************************************\n");
            printf("mypop >>>\n");
        }
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        maxfd = 0;
        FD_SET(sock_cli, &rfds);
        if(maxfd < sock_cli)
            maxfd = sock_cli;
        tv.tv_sec = 300;
        tv.tv_usec = 0;
        retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
        if(retval == -1)
        {
            printf("select出错, 客户端程序退出\n");
            break;
        }
        else if(retval == 0)
        {
            printf("客户端没有任何输入信息, 并且服务器也没有信息到来, waiting...\n");
            break;
        }
        else
        {
            if(FD_ISSET(sock_cli,&rfds))
            {   
                printf("\n$$$$$$$$$$$$\n");

                char recvbuf[BUFFER_SIZE];
                char recvbufcpy[BUFFER_SIZE];
                int len;
                len = recv(sock_cli, recvbuf, sizeof(recvbuf),0);
                printf("%c\n",recvbuf[len-1]);
                strcpy(recvbufcpy,recvbuf);

                printf("%s",recvbuf);
                memset(recvbuf, 0, sizeof(recvbuf));

                if(recvbufcpy[len-1]=='l'||recvbufcpy[len-1]==':'){
                    
                    len = recv(sock_cli, recvbuf, sizeof(recvbuf),0);
                    printf("%c\n",recvbuf[len-1]);
                    strcpy(recvbufcpy,recvbuf);
     
                    printf("%s\n",recvbuf);
                    memset(recvbuf, 0, sizeof(recvbuf));
                }

                count = 0;
                int back;
                printf("\n*********************************************************\n");
                printf("Enter any to continue\nmypop >>>");
                scanf("%d",&back);
                system("clear");
            }

            char num[10];
            if(FD_ISSET(0, &rfds))
            {
                count = 0;
                printf("\n***********\n");

                scanf("%s",num);
                printf("%c\n",num[0]);
                
                if (num[0]=='1'){
                    char sendbuf[BUFFER_SIZE]="STAT\n";
            
                    printf("%s\n",sendbuf);
                    send(sock_cli, sendbuf, strlen(sendbuf),0); 
                    memset(sendbuf, 0, sizeof(sendbuf));
                }

                else if (num[0]=='2'){
                    char sendbuf[BUFFER_SIZE]="LIST\n";
         
                    printf("%s\n",sendbuf);
                    send(sock_cli, sendbuf, strlen(sendbuf),0); 
                    memset(sendbuf, 0, sizeof(sendbuf));
                }

                else if (num[0]=='3'){
                    char sendbuf[BUFFER_SIZE];

                    memset(num,0,10);

                    FD_ZERO(&rfds);
                    FD_SET(0, &rfds);
                    maxfd = 0;

                    FD_SET(sock_cli, &rfds);

                    printf("\nEnter which email you want to check:\n");
                    printf("mypop >>>\n");
                    scanf("%s",num);
                    printf("%c\n",num[0]);

                    sendbuf[0]='R';
                    sendbuf[1]='E';
                    sendbuf[2]='T';
                    sendbuf[3]='R';
                    sendbuf[4]=' ';
                    sendbuf[5]=num[0];
                    sendbuf[6]='\n';


                    printf("%s\n",sendbuf);
                    send(sock_cli, (char*)sendbuf, strlen((char*)sendbuf),0); 
                    memset(sendbuf, 0, sizeof(sendbuf));
                    sleep(2);


                    FD_ZERO(&rfds);

                    FD_SET(0, &rfds);
                    maxfd = 0;

                    FD_SET(sock_cli, &rfds);

                    if(maxfd < sock_cli)
                        maxfd = sock_cli;
                    tv.tv_sec = 300;
                    tv.tv_usec = 0;

                    retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
                    if(FD_ISSET(sock_cli,&rfds))
                    {   
                        printf("\n---$$$$$$$$$$$$---\n");
                        char recvbuf[BUFFER_SIZE];
                        char recvbufcpy[BUFFER_SIZE];
                        int len;
                        len = recv(sock_cli, recvbuf, sizeof(recvbuf),0);
                        strcpy(recvbufcpy,recvbuf);
                        const char begin[10] = "base64";
			            const char end[10] = "-";
                        char *Begin = NULL;
	                    char *End = NULL;
                        char content[1000];
                        Begin = strstr(recvbufcpy, begin);
	                    End = strstr(Begin, end);
                        if(Begin == NULL || End == NULL || Begin > End)
	                    {
		                    printf("Not find\n");
	                    }else{
	
                            memcpy(content, Begin+10, End-Begin-10);
                            printf("邮件内容是%s\n",base64_decode((unsigned char*)content));
                            char dest[BUFFER_SIZE];
                            memset(dest,0,BUFFER_SIZE);                               
                            unsigned char *decode;
                            decode=base64_decode((unsigned char*)content);
                            strncat((char*)decode,"\n",2);
                            strrpc(dest,recvbufcpy,content,(char*)decode);
                            printf("邮件的内容是%s\n",dest);
                            memset(dest, 0, sizeof(dest));
	                    }

                        memset(recvbuf, 0, sizeof(recvbuf));
                        memset(recvbufcpy, 0, sizeof(recvbufcpy));


                    }

                    sendbuf[0]='N';
                    sendbuf[1]='O';
                    sendbuf[2]='O';
                    sendbuf[3]='P';
                    sendbuf[4]='\n';

                    printf("%s\n",sendbuf);
                    send(sock_cli, (char*)sendbuf, strlen((char*)sendbuf),0); 
                    memset(sendbuf, 0, sizeof(sendbuf));
                    
                    sleep(1);

                }

                else if (num[0]=='4'){
                    int length;
                    char sendbuf[BUFFER_SIZE]="STAT\n";
                    char recvbuf[BUFFER_SIZE];
                    int mail;
                    int countemail=0;

                    send(sock_cli, sendbuf, strlen(sendbuf),0); 
                    memset(sendbuf, 0, sizeof(sendbuf));


                    FD_ZERO(&rfds);

                    FD_SET(0, &rfds);
                    maxfd = 0;

                    FD_SET(sock_cli, &rfds);

                    if(maxfd < sock_cli)
                        maxfd = sock_cli;
                    tv.tv_sec = 300;
                    tv.tv_usec = 0;

                    retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
                    if(FD_ISSET(sock_cli,&rfds))
                    {
                        recv(sock_cli, recvbuf, 15,0);
                        mail = recvbuf[4];
                        memset(recvbuf, 0, sizeof(recvbuf));
                    }

                    printf("you have %c emails\n",mail);

                    char searchtext[10];

                    FD_ZERO(&rfds);

                    FD_SET(0, &rfds);
                    maxfd = 0;

                    FD_SET(sock_cli, &rfds);

                    printf("\nPlease input the text you want to search:\n");
                    printf("mypop >>>\n");


                    scanf("%s",searchtext);
                    length=strlen(searchtext);
                    printf("%s\n",searchtext);
                    printf("--%d--\n",length);
		
			        int i;
                    for(i = 1;i < ((int)mail-48)+1;i++){

                        FD_ZERO(&rfds);

                        FD_SET(0, &rfds);
                        maxfd = 0;

                        FD_SET(sock_cli, &rfds);

                        if(maxfd < sock_cli)
                            maxfd = sock_cli;
                        tv.tv_sec = 300;
                        tv.tv_usec = 0;

                        sendbuf[0]='R';
                        sendbuf[1]='E';
                        sendbuf[2]='T';
                        sendbuf[3]='R';
                        sendbuf[4]=' ';
                        sendbuf[5]=(char)('0' + i );
                        sendbuf[6]='\n';
                        printf("%s",sendbuf);
                        send(sock_cli, sendbuf, strlen(sendbuf),0); 
                        memset(sendbuf, 0, sizeof(sendbuf));

                        sleep(2);


                        FD_ZERO(&rfds);

                        FD_SET(0, &rfds);
                        maxfd = 0;

                        FD_SET(sock_cli, &rfds);

                        if(maxfd < sock_cli)
                            maxfd = sock_cli;
                        tv.tv_sec = 10;
                        tv.tv_usec = 0;

                        retval = select(maxfd+1, &rfds, NULL, NULL, &tv);

                        if(FD_ISSET(sock_cli,&rfds))
                        {
                            printf("$$$$$$$$$$$$$$");
                            int len;
                            char recvbufcpy[BUFFER_SIZE];
                            int countequal=0;
                            recv(sock_cli, recvbuf, sizeof(recvbuf) ,0);
                            strcpy(recvbufcpy,recvbuf);
                            len = strlen(recvbufcpy);
                            printf("<-mail length:%d->\n",len);
                            memset(recvbuf, 0, sizeof(recvbuf));
                            
				            int m,n;
                            for(m=0; m<(len-length) ;m++){
                                for(n=0; n<length ;n++){
                                    printf("%c %c\n",recvbufcpy[m+n],searchtext[n]);
                                    if(recvbufcpy[m+n]==searchtext[n]){
                                        countequal++;
                                    }
                                }
                                if(countequal==length){
                                    printf("The mail of e%d.eml has the text.\n",i);
                                    countemail++;
                                    break;
                                }
                                countequal=0;
                            }
                            memset(recvbufcpy, 0, sizeof(recvbufcpy));

                            printf("%d %d\n",countequal,countemail);

                            if(i==((int)mail-48)){
                                printf("There is (are) %d mail(s) including the text.\n",countemail);
                            }
                        }

                        sleep(1);
                    }

                    memset(num,0,10);

                    sendbuf[0]='N';
                    sendbuf[1]='O';
                    sendbuf[2]='O';
                    sendbuf[3]='P';
                    sendbuf[4]='\n';

                    printf("%s\n",sendbuf);
                    send(sock_cli, (char*)sendbuf, strlen((char*)sendbuf),0); 
                    memset(sendbuf, 0, sizeof(sendbuf));
                    
                    sleep(1);

                }

                else if (num[0]=='5'){
                    char sendbuf[BUFFER_SIZE]="STAT\n";
                    char recvbuf[BUFFER_SIZE];
                    int mail;

                    send(sock_cli, sendbuf, strlen(sendbuf),0); 
                    memset(sendbuf, 0, sizeof(sendbuf));


                    FD_ZERO(&rfds);

                    FD_SET(0, &rfds);
                    maxfd = 0;

                    FD_SET(sock_cli, &rfds);

                    if(maxfd < sock_cli)
                        maxfd = sock_cli;
                    tv.tv_sec = 300;
                    tv.tv_usec = 0;

                    retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
                    if(FD_ISSET(sock_cli,&rfds))
                    {
                        recv(sock_cli, recvbuf, 15,0);
                        mail = recvbuf[4];
                        memset(recvbuf, 0, sizeof(recvbuf));
                    }

                    printf("you have %c emails\n",mail);

                    const char begin[10] = "Subject:";
                    const char end[10] = "Mime-Ver";
                    char *Begin = NULL;
	                char *End = NULL;
                    char Subject[1000];
			        int i;
                    for(i = 1;i < ((int)mail-48)+1;i++){

                        FD_ZERO(&rfds);

                        FD_SET(0, &rfds);
                        maxfd = 0;

                        FD_SET(sock_cli, &rfds);

                        if(maxfd < sock_cli)
                            maxfd = sock_cli;
                        tv.tv_sec = 300;
                        tv.tv_usec = 0;

                        sendbuf[0]='R';
                        sendbuf[1]='E';
                        sendbuf[2]='T';
                        sendbuf[3]='R';
                        sendbuf[4]=' ';
                        sendbuf[5]=(char)('0' + i );
                        sendbuf[6]='\n';
                        printf("%s",sendbuf);
                        send(sock_cli, sendbuf, strlen(sendbuf),0); 
                        memset(sendbuf, 0, sizeof(sendbuf));

                        sleep(2);


                        FD_ZERO(&rfds);

                        FD_SET(0, &rfds);
                        maxfd = 0;

                        FD_SET(sock_cli, &rfds);

                        if(maxfd < sock_cli)
                            maxfd = sock_cli;
                        tv.tv_sec = 10;
                        tv.tv_usec = 0;

                        retval = select(maxfd+1, &rfds, NULL, NULL, &tv);

                        if(FD_ISSET(sock_cli,&rfds))
                        {
                            int len;
                            char recvbufcpy[BUFFER_SIZE];
                            int countequal=0;
                            recv(sock_cli, recvbuf, sizeof(recvbuf) ,0);
                            strcpy(recvbufcpy,recvbuf);
                            len = strlen(recvbufcpy);
                            printf("<-mail length:%d->\n",len);
                            memset(recvbuf, 0, sizeof(recvbuf));

                            Begin = strstr(recvbufcpy, begin)-1;
                            End = strstr(Begin, end);
                            if(Begin == NULL || End == NULL || Begin > End){
                                printf("Thank you\n");
                            }
                            else{
                                memcpy(Subject, Begin+10, End-Begin-10);
                                printf("邮件主题是: %s\n", Subject);
                            }
                            memset(recvbufcpy, 0, sizeof(recvbufcpy));
                        }
                        sleep(1);
                    }

                    memset(num,0,10);
                    sendbuf[0]='N';
                    sendbuf[1]='O';
                    sendbuf[2]='O';
                    sendbuf[3]='P';
                    sendbuf[4]='\n';
                    printf("%s\n",sendbuf);
                    send(sock_cli, (char*)sendbuf, strlen((char*)sendbuf),0); 
                    memset(sendbuf, 0, sizeof(sendbuf));
                    sleep(1);
                    
                }

                else if (num[0]=='6'){
                    char sendbuf[BUFFER_SIZE];
                    
                    memset(num,0,10);
                    
                    FD_ZERO(&rfds);
                    
                    FD_SET(0, &rfds);
                    maxfd = 0;
                    
                    FD_SET(sock_cli, &rfds);
                    
                    printf("\nEnter which email you want to check:\n");
                    printf("mypop >>>\n");
                    scanf("%s",num);
                    printf("%c\n",num[0]);

                    sendbuf[0]='R';
                    sendbuf[1]='E';
                    sendbuf[2]='T';
                    sendbuf[3]='R';
                    sendbuf[4]=' ';
                    sendbuf[5]=num[0];
                    sendbuf[6]='\n';

         
                    printf("%s\n",sendbuf);
                    send(sock_cli, (char*)sendbuf, strlen((char*)sendbuf),0); 
                    memset(sendbuf, 0, sizeof(sendbuf));
                    sleep(2);

                    
                    FD_ZERO(&rfds);
                    
                    FD_SET(0, &rfds);
                    maxfd = 0;
                    
                    FD_SET(sock_cli, &rfds);
                    
                    if(maxfd < sock_cli)
                        maxfd = sock_cli;
                    tv.tv_sec = 300;
                    tv.tv_usec = 0;

                    retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
                    if(FD_ISSET(sock_cli,&rfds))
                    {   
                        char recvbuf[BUFFER_SIZE];
                        char recvbufcpy[BUFFER_SIZE];
                        int len;
                        len = recv(sock_cli, recvbuf, sizeof(recvbuf),0);
                        strcpy(recvbufcpy,recvbuf);
                        printf("%s", recvbufcpy);
                        memset(recvbuf, 0, sizeof(recvbuf));


                        const char begin[10] = "base64";
                        const char end[10] = "-";
                        char *Begin = NULL;
	                    char *End = NULL;
                        char content[1000];
                        Begin = strstr(recvbufcpy, begin);
	                    End = strstr(Begin, end);
                        if(Begin == NULL || End == NULL || Begin > End)
	                        {
		                        printf("Not find\n");
	                        }
                            else
                            {
		                        memcpy(content, Begin+10, End-Begin-10);

                                char dest[BUFFER_SIZE];
                                memset(dest,0,BUFFER_SIZE);
                                unsigned char *decode;
                                decode=base64_decode((unsigned char*)content);
                                strncat((char*)decode,"\n",2);
                                strrpc(dest,recvbufcpy,content,(char*)decode);
	                            printf("邮件的内容是%s\n",dest);
                                FILE *fp;
                                int i; 
                                
                                char filename[10]="0.eml";
                                filename[0]=(char)num[0];
                                printf("下载完成，文件名为：%s\n",filename);
                                fp=fopen(filename,"w");
                                if(fp==NULL) 
                                    {
                                        printf("File cannot open! " );
                                        exit(0);
                                    }

                                        fprintf(fp,"%s\t", dest);
                                    
                                    fclose(fp);
                                    memset(dest, 0, sizeof(dest));
                                    memset(filename, 0, sizeof(filename));
                            }
                            memset(recvbuf, 0, sizeof(recvbuf));
                            memset(recvbufcpy, 0, sizeof(recvbufcpy));
                    }

                    memset(sendbuf, 0, sizeof(sendbuf));
                    sendbuf[0]='D';
                    sendbuf[1]='E';
                    sendbuf[2]='L';
                    sendbuf[3]='E';
                    sendbuf[4]=' ';
                    sendbuf[5]=num[0];
                    sendbuf[6]='\n';

                    printf("%s\n",sendbuf);
                    send(sock_cli, (char*)sendbuf, strlen((char*)sendbuf),0); 
                    memset(sendbuf, 0, sizeof(sendbuf));
                    sleep(1);
                    printf("The email has been deleted.\n");


                }

                else if (num[0]=='7'){
                    char sendbuf[BUFFER_SIZE]="QUIT\n";

                    printf("%s\n",sendbuf);
                    send(sock_cli, sendbuf, strlen(sendbuf),0); 
                    memset(sendbuf, 0, sizeof(sendbuf));
                    break;
                }
                else{
                    printf("invalid number, please enter again!\n");
                }
                count++;
                memset(num, 0, 10);
            }
        }
        
    }
    
    close(sock_cli);
    return 0;
}


unsigned char *base64_decode(unsigned char *code)  
{  

    int table[]={0,0,0,0,0,0,0,0,0,0,0,0,
    		 0,0,0,0,0,0,0,0,0,0,0,0,
    		 0,0,0,0,0,0,0,0,0,0,0,0,
    		 0,0,0,0,0,0,0,62,0,0,0,
    		 63,52,53,54,55,56,57,58,
    		 59,60,61,0,0,0,0,0,0,0,0,
    		 1,2,3,4,5,6,7,8,9,10,11,12,
    		 13,14,15,16,17,18,19,20,21,
    		 22,23,24,25,0,0,0,0,0,0,26,
    		 27,28,29,30,31,32,33,34,35,
    		 36,37,38,39,40,41,42,43,44,
    		 45,46,47,48,49,50,51
    	       };  
    long len;  
    long str_len;  
    unsigned char *res;  
    int i,j;  
  

    len=strlen((const char*)code);  

    if(strstr((const char*)code,"=="))  
        str_len=len/4*3-2;  
    else if(strstr((const char*)code,"="))  
        str_len=len/4*3-1;  
    else  
        str_len=len/4;  
  
    res=malloc(sizeof(unsigned char)*str_len+1);  
    res[str_len]='\0';  
  

    for(i=0,j=0;i < len-2;j+=3,i+=4)  
    {  
        res[j]=((unsigned char)table[code[i]])<<2 | (((unsigned char)table[code[i+1]])>>4); 
        res[j+1]=(((unsigned char)table[code[i+1]])<<4) | (((unsigned char)table[code[i+2]])>>2); 
        res[j+2]=(((unsigned char)table[code[i+2]])<<6) | ((unsigned char)table[code[i+3]]); 
    }  
  
    return res;  
  
}  

unsigned char *base64_encode(unsigned char *str)   
{  
    long len;  
    long str_len;  
    unsigned char *res;  
    int i,j;  

    char *base64_table="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";  
  

    str_len=strlen((const char*)str);  
    if(str_len % 3 == 0)  
        len=str_len/3*4;  
    else  
        len=(str_len/3+1)*4;  
  
    res=malloc(sizeof(unsigned char)*len+1);  
    res[len]='\0';  
  

    for(i=0,j=0;i<len-2;j+=3,i+=4)  
    {  
        res[i]=base64_table[str[j]>>2]; 
        res[i+1]=base64_table[(str[j]&0x3)<<4 | (str[j+1]>>4)]; 
        res[i+2]=base64_table[(str[j+1]&0xf)<<2 | (str[j+2]>>6)]; 
        res[i+3]=base64_table[str[j+2]&0x3f];
    }  
  
    switch(str_len % 3)  
    {  
        case 1:  
            res[i-2]='=';  
            res[i-1]='=';  
            break;  
        case 2:  
            res[i-1]='=';  
            break;  
    }  
    return res;
}  
