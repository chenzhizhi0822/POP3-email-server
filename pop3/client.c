#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
//#include <termio.h>
#include <sys/shm.h>
#include <netdb.h> /* for hostent and gethostbyname() */
 
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

// int mygetch()
// {
//     struct termios oldt, newt;
//     int ch;
//     tcgetattr(STDIN_FILENO, &oldt);
//     newt = oldt;
//     newt.c_lflag &= ~(ICANON | ECHO);
//     tcsetattr(STDIN_FILENO, TCSANOW, &newt);
//     ch = getchar();
//     tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
//     return ch;
// }

// int getpasswd(char *passwd, int size)
// {
//     int c, n = 0;
//     do
//     {
//         c = mygetch();
//         if (c != '\n' && c != 'r' && c != 127)
//         {
//             passwd[n] = c;
//             printf("*");
//             n++;
//         }
//         else if ((c != '\n' | c != '\r') && c == 127)//判断是否是回车或则退格
//         {
//             if (n > 0)
//             {
//                 n--;
//                 printf("\b \b");//输出退格
//             }
//         }
//     }while (c != '\n' && c != '\r' && n < (size - 1));
//     passwd[n] = '\0';//消除一个多余的回车
//     return n;
// }

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


	/* translate the domain Name to IP address */
	if((host = gethostbyname(argv[1])) == NULL) {
		perror("fail to get host by name\n");
		exit(1);
	}

    ///定义sockfd
    sock_cli = socket(AF_INET,SOCK_STREAM, 0);
    ///定义sockaddr_in
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT); ///Server port
    servaddr.sin_addr = *((struct in_addr *)host -> h_addr); ///Server ip

    //连接服务器，成功返回0，错误返回-1
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

    //USER
    //while(1){

        /*用户输入信息了,开始处理信息并发送*/
        //if(FD_ISSET(0, &rfds))
        //{
            //char sendbuf[BUFFER_SIZE];
            char sendbuf[BUFFER_SIZE]="USER maruyamaayabupt@163.com\n";
            //fgets(sendbuf, sizeof(sendbuf), stdin);
            printf("%s", sendbuf);
            if(send(sock_cli, sendbuf, strlen(sendbuf),0)!=strlen(sendbuf)) //发送
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


        /*服务器发来了消息*/
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
            if(strncmp(str1,recvbufcpy,3)==0){
                
            }
            else{
                //printf("%d\n",strncmp(str1,recvbufcpy,3));
                exit(0);
            }
        }

    //}

    sleep(1);

    printf("Please enter your password\n");

    //PASS
    //while(1){

        /*用户输入信息了,开始处理信息并发送*/
        //if(FD_ISSET(0, &rfds))
        //{

            //char sendbuf[BUFFER_SIZE];
            //int l;

            // char passWord[25];
            // printf("请输入密码:\n");
            // getpasswd(passWord, 25);
            // printf("\n");
            // printf("你输入的密码是:%s\n", passWord);

            // char sendbuf_pass[BUFFER_SIZE]="PASS ";
            // strcat(sendbuf_pass,passWord);
            // strcat(sendbuf_pass,"\n");
            // send(sock_cli, sendbuf_pass, strlen(sendbuf_pass),0); //
            // sleep(2);
            // memset(sendbuf_pass, 0, sizeof(sendbuf_pass));
            char sendbuf_pass[BUFFER_SIZE]="PASS RBRGWYPWYPKXFMJC\n";
            //fgets(sendbuf_pass, sizeof(sendbuf_pass), stdin);
            printf("%s", sendbuf_pass);
            if(send(sock_cli, sendbuf_pass, strlen(sendbuf_pass),0)!=strlen(sendbuf_pass)) //发送
                printf("send() error");
            memset(sendbuf_pass, 0, sizeof(sendbuf_pass));
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
        /*服务器发来了消息*/
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
            else{
                exit(0);
            }
            //printf("%d\n",strncmp(str1,recvbufcpy,3));
        }
    //}

    printf("\n***** Welcome to the POP3 client side! *****\n");
    sleep(3);
    system("clear");
    count = 0;

    //main function
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
        /*把可读文件描述符的集合清空*/
        FD_ZERO(&rfds);
        /*把标准输入的文件描述符加入到集合中*/
        FD_SET(0, &rfds);
        maxfd = 0;
        /*把当前连接的文件描述符加入到集合中*/
        FD_SET(sock_cli, &rfds);
        /*找出文件描述符集合中最大的文件描述符*/ 
        if(maxfd < sock_cli)
            maxfd = sock_cli;
        /*设置超时时间*/
        tv.tv_sec = 300;
        tv.tv_usec = 0;
        /*等待聊天*/
        //select属于非阻塞方式，不同于send(),recv()
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
            /*服务器发来了消息*/
            if(FD_ISSET(sock_cli,&rfds))
            {   
                printf("\n$$$$$$$$$$$$\n");

                char recvbuf[BUFFER_SIZE];
                char recvbufcpy[BUFFER_SIZE];
                int len;
                len = recv(sock_cli, recvbuf, sizeof(recvbuf),0);
                printf("%c\n",recvbuf[len-1]);
                strcpy(recvbufcpy,recvbuf);
                //printf("%s\n", base64_encode((unsigned char*)recvbufcpy));
                //printf("%s\n", base64_decode((unsigned char*)recvbufcpy));
                printf("%s",recvbuf);
                memset(recvbuf, 0, sizeof(recvbuf));

                /* 没接收完的情况 */
                if(recvbufcpy[len-1]=='l'||recvbufcpy[len-1]==':'){
                    
                    // printf("\n√\n");
                    // printf("$$$$$$$$$$$$\n");

                    len = recv(sock_cli, recvbuf, sizeof(recvbuf),0);
                    printf("%c\n",recvbuf[len-1]);
                    strcpy(recvbufcpy,recvbuf);
                    //printf("%s\n", base64_encode((unsigned char*)recvbufcpy));
                    //printf("%s\n", base64_decode((unsigned char*)recvbufcpy));
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

            char num[10];//用户输入
            /*用户输入信息了,开始处理信息并发送*/
            if(FD_ISSET(0, &rfds))
            {
                count = 0;
                printf("\n***********\n");

                scanf("%s",num);
                printf("%c\n",num[0]);
                
                /* 1.获得邮箱状态 */
                if (num[0]=='1'){
                    char sendbuf[BUFFER_SIZE]="STAT\n";
                    //fgets(sendbuf, sizeof(sendbuf), stdin);
                    printf("%s\n",sendbuf);
                    send(sock_cli, sendbuf, strlen(sendbuf),0); //发送
                    memset(sendbuf, 0, sizeof(sendbuf));
                }
                /* 2.获得邮件列表 */
                else if (num[0]=='2'){
                    char sendbuf[BUFFER_SIZE]="LIST\n";
                    //fgets(sendbuf, sizeof(sendbuf), stdin);
                    printf("%s\n",sendbuf);
                    send(sock_cli, sendbuf, strlen(sendbuf),0); //发送
                    memset(sendbuf, 0, sizeof(sendbuf));
                }
                /* 3.获得指定邮件内容 */
                else if (num[0]=='3'){
                    char sendbuf[BUFFER_SIZE];
                    /* 第二次输入，确定要查看的index */
                    memset(num,0,10);
                    /*把可读文件描述符的集合清空*/
                    FD_ZERO(&rfds);
                    /*把标准输入的文件描述符加入到集合中*/
                    FD_SET(0, &rfds);
                    maxfd = 0;
                    /*把当前连接的文件描述符加入到集合中*/
                    FD_SET(sock_cli, &rfds);
                    /*找出文件描述符集合中最大的文件描述符*/ 
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

                    //fgets(sendbuf, sizeof(sendbuf), stdin);
                    printf("%s\n",sendbuf);
                    send(sock_cli, (char*)sendbuf, strlen((char*)sendbuf),0); //发送
                    memset(sendbuf, 0, sizeof(sendbuf));
                    sleep(2);

                    /*把可读文件描述符的集合清空*/
                    FD_ZERO(&rfds);
                    // /*把标准输入的文件描述符加入到集合中*/
                    FD_SET(0, &rfds);
                    maxfd = 0;
                    /*把当前连接的文件描述符加入到集合中*/
                    FD_SET(sock_cli, &rfds);
                    /*找出文件描述符集合中最大的文件描述符*/ 
                    if(maxfd < sock_cli)
                        maxfd = sock_cli;
                    tv.tv_sec = 300;
                    tv.tv_usec = 0;
                    /*等待聊天*/
                    //select属于非阻塞方式，不同于send(),recv()
                    retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
                    if(FD_ISSET(sock_cli,&rfds))
                    {   
                        printf("\n---$$$$$$$$$$$$---\n");
                        char recvbuf[BUFFER_SIZE];
                        char recvbufcpy[BUFFER_SIZE];
                        int len;
                        len = recv(sock_cli, recvbuf, sizeof(recvbuf),0);//接收+OK
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
	                	    //Begin += strlen(begin);
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
                        //printf("%s", recvbufcpy);
                        memset(recvbuf, 0, sizeof(recvbuf));
                        memset(recvbufcpy, 0, sizeof(recvbufcpy));

                    }

                    sendbuf[0]='N';
                    sendbuf[1]='O';
                    sendbuf[2]='O';
                    sendbuf[3]='P';
                    sendbuf[4]='\n';

                    printf("%s\n",sendbuf);
                    send(sock_cli, (char*)sendbuf, strlen((char*)sendbuf),0); //发送NOOP,让程序进while里的接收
                    memset(sendbuf, 0, sizeof(sendbuf));
                    
                    sleep(1);

                }
                /* 4.按文本搜索 */
                else if (num[0]=='4'){
                    int length;//计输入长度
                    char sendbuf[BUFFER_SIZE]="STAT\n";//先发一个STAT确定有几个邮件;
                    char recvbuf[BUFFER_SIZE];
                    int mail;//计邮件数
                    int countemail=0;//计匹配邮件一共有多少

                    send(sock_cli, sendbuf, strlen(sendbuf),0); //发送
                    memset(sendbuf, 0, sizeof(sendbuf));

                    /*把可读文件描述符的集合清空*/
                    FD_ZERO(&rfds);
                    // /*把标准输入的文件描述符加入到集合中*/
                    FD_SET(0, &rfds);
                    maxfd = 0;
                    /*把当前连接的文件描述符加入到集合中*/
                    FD_SET(sock_cli, &rfds);
                    /*找出文件描述符集合中最大的文件描述符*/ 
                    if(maxfd < sock_cli)
                        maxfd = sock_cli;
                    tv.tv_sec = 300;
                    tv.tv_usec = 0;
                    /*等待聊天*/
                    //select属于非阻塞方式，不同于send(),recv()
                    retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
                    if(FD_ISSET(sock_cli,&rfds))
                    {
                        recv(sock_cli, recvbuf, 15,0);
                        mail = recvbuf[4];
                        memset(recvbuf, 0, sizeof(recvbuf));
                    }

                    printf("you have %c emails\n",mail);

                    char searchtext[10];
                    /* 第二次输入，确定要搜索的文本 */
                    /*把可读文件描述符的集合清空*/
                    FD_ZERO(&rfds);
                    /*把标准输入的文件描述符加入到集合中*/
                    FD_SET(0, &rfds);
                    maxfd = 0;
                    /*把当前连接的文件描述符加入到集合中*/
                    FD_SET(sock_cli, &rfds);
                    /*找出文件描述符集合中最大的文件描述符*/ 
                    printf("\nPlease input the text you want to search:\n");
                    printf("mypop >>>\n");

                    //fgets(searchtext,strlen(searchtext),stdin);
                    scanf("%s",searchtext);
                    length=strlen(searchtext);
                    printf("%s\n",searchtext);
                    printf("--%d--\n",length);

                    for(int i = 1;i < ((int)mail-48)+1;i++){
                        /*把可读文件描述符的集合清空*/
                        FD_ZERO(&rfds);
                        // /*把标准输入的文件描述符加入到集合中*/
                        FD_SET(0, &rfds);
                        maxfd = 0;
                        /*把当前连接的文件描述符加入到集合中*/
                        FD_SET(sock_cli, &rfds);
                        /*找出文件描述符集合中最大的文件描述符*/ 
                        if(maxfd < sock_cli)
                            maxfd = sock_cli;
                        tv.tv_sec = 300;
                        tv.tv_usec = 0;
                        /*等待聊天*/
                        //select属于非阻塞方式，不同于send(),recv()
                        sendbuf[0]='R';
                        sendbuf[1]='E';
                        sendbuf[2]='T';
                        sendbuf[3]='R';
                        sendbuf[4]=' ';
                        sendbuf[5]=(char)('0' + i );
                        sendbuf[6]='\n';
                        printf("%s",sendbuf);
                        send(sock_cli, sendbuf, strlen(sendbuf),0); //发送
                        memset(sendbuf, 0, sizeof(sendbuf));

                        sleep(2);

                        /*把可读文件描述符的集合清空*/
                        FD_ZERO(&rfds);
                        // /*把标准输入的文件描述符加入到集合中*/
                        FD_SET(0, &rfds);
                        maxfd = 0;
                        /*把当前连接的文件描述符加入到集合中*/
                        FD_SET(sock_cli, &rfds);
                        /*找出文件描述符集合中最大的文件描述符*/ 
                        if(maxfd < sock_cli)
                            maxfd = sock_cli;
                        tv.tv_sec = 10;
                        tv.tv_usec = 0;
                        /*等待聊天*/
                        //select属于非阻塞方式，不同于send(),recv()
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
                            
                            for(int m=0; m<(len-length) ;m++){
                                for(int n=0; n<length ;n++){
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
                            // if(countequal==length){
                            //     printf("The mail of e%d.eml has the text.\n",i);
                            //     countemail++;
                            // }
                            if(i==((int)mail-48)){
                                printf("There is (are) %d mail(s) including the text.\n",countemail);
                            }
                        }

                        sleep(1);
                    }

                    memset(num,0,10);//将输入数组归0

                    sendbuf[0]='N';
                    sendbuf[1]='O';
                    sendbuf[2]='O';
                    sendbuf[3]='P';
                    sendbuf[4]='\n';

                    printf("%s\n",sendbuf);
                    send(sock_cli, (char*)sendbuf, strlen((char*)sendbuf),0); //发送NOOP,让程序进while里的接收
                    memset(sendbuf, 0, sizeof(sendbuf));
                    
                    sleep(1);

                }
                /* 5.显示主题 */
                else if (num[0]=='5'){
                    char sendbuf[BUFFER_SIZE]="STAT\n";//先发一个STAT确定有几个邮件;
                    char recvbuf[BUFFER_SIZE];
                    int mail;//计邮件数

                    send(sock_cli, sendbuf, strlen(sendbuf),0); //发送
                    memset(sendbuf, 0, sizeof(sendbuf));

                    /*把可读文件描述符的集合清空*/
                    FD_ZERO(&rfds);
                    // /*把标准输入的文件描述符加入到集合中*/
                    FD_SET(0, &rfds);
                    maxfd = 0;
                    /*把当前连接的文件描述符加入到集合中*/
                    FD_SET(sock_cli, &rfds);
                    /*找出文件描述符集合中最大的文件描述符*/ 
                    if(maxfd < sock_cli)
                        maxfd = sock_cli;
                    tv.tv_sec = 300;
                    tv.tv_usec = 0;
                    /*等待聊天*/
                    //select属于非阻塞方式，不同于send(),recv()
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

                    for(int i = 1;i < ((int)mail-48)+1;i++){
                        /*把可读文件描述符的集合清空*/
                        FD_ZERO(&rfds);
                        // /*把标准输入的文件描述符加入到集合中*/
                        FD_SET(0, &rfds);
                        maxfd = 0;
                        /*把当前连接的文件描述符加入到集合中*/
                        FD_SET(sock_cli, &rfds);
                        /*找出文件描述符集合中最大的文件描述符*/ 
                        if(maxfd < sock_cli)
                            maxfd = sock_cli;
                        tv.tv_sec = 300;
                        tv.tv_usec = 0;
                        /*等待聊天*/
                        //select属于非阻塞方式，不同于send(),recv()
                        sendbuf[0]='R';
                        sendbuf[1]='E';
                        sendbuf[2]='T';
                        sendbuf[3]='R';
                        sendbuf[4]=' ';
                        sendbuf[5]=(char)('0' + i );
                        sendbuf[6]='\n';
                        printf("%s",sendbuf);
                        send(sock_cli, sendbuf, strlen(sendbuf),0); //发送
                        memset(sendbuf, 0, sizeof(sendbuf));

                        sleep(2);

                        /*把可读文件描述符的集合清空*/
                        FD_ZERO(&rfds);
                        // /*把标准输入的文件描述符加入到集合中*/
                        FD_SET(0, &rfds);
                        maxfd = 0;
                        /*把当前连接的文件描述符加入到集合中*/
                        FD_SET(sock_cli, &rfds);
                        /*找出文件描述符集合中最大的文件描述符*/ 
                        if(maxfd < sock_cli)
                            maxfd = sock_cli;
                        tv.tv_sec = 10;
                        tv.tv_usec = 0;
                        /*等待聊天*/
                        //select属于非阻塞方式，不同于send(),recv()
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

                    memset(num,0,10);//将输入数组归0
                    sendbuf[0]='N';
                    sendbuf[1]='O';
                    sendbuf[2]='O';
                    sendbuf[3]='P';
                    sendbuf[4]='\n';
                    printf("%s\n",sendbuf);
                    send(sock_cli, (char*)sendbuf, strlen((char*)sendbuf),0); //发送NOOP,让程序进while里的接收
                    memset(sendbuf, 0, sizeof(sendbuf));
                    sleep(1);
                    
                }
                /* 6.保存并删除 */
                else if (num[0]=='6'){
                    char sendbuf[BUFFER_SIZE];
                    /* 第二次输入，确定要查看的index */
                    memset(num,0,10);
                    /*把可读文件描述符的集合清空*/
                    FD_ZERO(&rfds);
                    /*把标准输入的文件描述符加入到集合中*/
                    FD_SET(0, &rfds);
                    maxfd = 0;
                    /*把当前连接的文件描述符加入到集合中*/
                    FD_SET(sock_cli, &rfds);
                    /*找出文件描述符集合中最大的文件描述符*/ 
                    printf("\nEnter which email you want to download and delete:\n");
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

                    //fgets(sendbuf, sizeof(sendbuf), stdin);
                    printf("%s\n",sendbuf);
                    send(sock_cli, (char*)sendbuf, strlen((char*)sendbuf),0); //发送
                    memset(sendbuf, 0, sizeof(sendbuf));
                    sleep(2);

                    /*把可读文件描述符的集合清空*/
                    FD_ZERO(&rfds);
                    // /*把标准输入的文件描述符加入到集合中*/
                    FD_SET(0, &rfds);
                    maxfd = 0;
                    /*把当前连接的文件描述符加入到集合中*/
                    FD_SET(sock_cli, &rfds);
                    /*找出文件描述符集合中最大的文件描述符*/ 
                    if(maxfd < sock_cli)
                        maxfd = sock_cli;
                    tv.tv_sec = 300;
                    tv.tv_usec = 0;
                    /*等待聊天*/
                    //select属于非阻塞方式，不同于send(),recv()
                    retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
                    if(FD_ISSET(sock_cli,&rfds))
                    {   
                        char recvbuf[BUFFER_SIZE];
                        char recvbufcpy[BUFFER_SIZE];
                        int len;
                        len = recv(sock_cli, recvbuf, sizeof(recvbuf),0);//接收全部
                        strcpy(recvbufcpy,recvbuf);
                        printf("%s", recvbufcpy);
                        memset(recvbuf, 0, sizeof(recvbuf));
                        //memset(recvbufcpy, 0, sizeof(recvbufcpy));

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
                                //printf("邮件内容是%s\n",base64_decode(content));
                                char dest[BUFFER_SIZE];
                                memset(dest,0,BUFFER_SIZE);                               
                                unsigned char *decode;
                                decode=base64_decode((unsigned char*)content);
                                strncat((char*)decode,"\n",2);
                                strrpc(dest,recvbufcpy,content,(char*)decode);
	                            printf("邮件的内容是%s\n",dest);
                                FILE *fp;//文件指针
                                int i; 
                                /*文件的打开*/
                                char filename[10]="0.eml";
                                filename[0]=num[0];
                                printf("下载完成，文件名为：%s\n",filename);
                                fp=fopen(filename,"w");//fopen打开文件，这个文件可以是当前不存在的。“w”以写入的形式打开，“r”以读的形式打开
                                if(fp==NULL) //判断如果文件指针为空
                                    {
                                        printf("File cannot open! " );
                                        exit(0);//在以0的形式退出，必须在文件开头有#include <stdlib.h>,stdlib 头文件即standard library标准库头文件
                                    }
                                        //printf("是不是%s空的", dest);
                                        fprintf(fp,"%s\t", dest);//写入指针fp，写入的东西就是刚才的用户输入的d,注意这里的fp和d没有引号
                                    //关闭文件
                                    fclose(fp);
                                    memset(dest, 0, sizeof(dest));
                                    memset(filename, 0, sizeof(filename));
                            }
                            memset(recvbuf, 0, sizeof(recvbuf));
                            memset(recvbufcpy, 0, sizeof(recvbufcpy));
                    }

                    memset(sendbuf, 0, sizeof(sendbuf));
                    //memset(num,0,10);//将输入数组归0
                    sendbuf[0]='D';
                    sendbuf[1]='E';
                    sendbuf[2]='L';
                    sendbuf[3]='E';
                    sendbuf[4]=' ';
                    sendbuf[5]=num[0];
                    sendbuf[6]='\n';

                    printf("%s\n",sendbuf);
                    send(sock_cli, (char*)sendbuf, strlen((char*)sendbuf),0); //发送DELE
                    memset(sendbuf, 0, sizeof(sendbuf));
                    sleep(1);
                    printf("The email has been deleted.\n");


                }
                /* 7.退出 */
                else if (num[0]=='7'){
                    char sendbuf[BUFFER_SIZE]="QUIT\n";
                    //fgets(sendbuf, sizeof(sendbuf), stdin);
                    printf("%s\n",sendbuf);
                    send(sock_cli, sendbuf, strlen(sendbuf),0); //发送
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

/* base64 */
unsigned char *base64_decode(unsigned char *code)  
{  
//根据base64表，以字符找到对应的十进制数据  
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
  
//计算解码后的字符串长度  
    len=strlen((const char*)code);  
//判断编码后的字符串后是否有=  
    if(strstr((const char*)code,"=="))  
        str_len=len/4*3-2;  
    else if(strstr((const char*)code,"="))  
        str_len=len/4*3-1;  
    else  
        str_len=len/4;  
  
    res=malloc(sizeof(unsigned char)*str_len+1);  
    res[str_len]='\0';  
  
//以4个字符为一位进行解码  
    for(i=0,j=0;i < len-2;j+=3,i+=4)  
    {  
        res[j]=((unsigned char)table[code[i]])<<2 | (((unsigned char)table[code[i+1]])>>4); //取出第一个字符对应base64表的十进制数的前6位与第二个字符对应base64表的十进制数的后2位进行组合  
        res[j+1]=(((unsigned char)table[code[i+1]])<<4) | (((unsigned char)table[code[i+2]])>>2); //取出第二个字符对应base64表的十进制数的后4位与第三个字符对应bas464表的十进制数的后4位进行组合  
        res[j+2]=(((unsigned char)table[code[i+2]])<<6) | ((unsigned char)table[code[i+3]]); //取出第三个字符对应base64表的十进制数的后2位与第4个字符进行组合  
    }  
  
    return res;  
  
}  

unsigned char *base64_encode(unsigned char *str)   
{  
    long len;  
    long str_len;  
    unsigned char *res;  
    int i,j;  
//定义base64编码表  
    char *base64_table="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";  
  
//计算经过base64编码后的字符串长度  
    str_len=strlen((const char*)str);  
    if(str_len % 3 == 0)  
        len=str_len/3*4;  
    else  
        len=(str_len/3+1)*4;  
  
    res=malloc(sizeof(unsigned char)*len+1);  
    res[len]='\0';  
  
//以3个8位字符为一组进行编码  
    for(i=0,j=0;i<len-2;j+=3,i+=4)  
    {  
        res[i]=base64_table[str[j]>>2]; //取出第一个字符的前6位并找出对应的结果字符  
        res[i+1]=base64_table[(str[j]&0x3)<<4 | (str[j+1]>>4)]; //将第一个字符的后位与第二个字符的前4位进行组合并找到对应的结果字符  
        res[i+2]=base64_table[(str[j+1]&0xf)<<2 | (str[j+2]>>6)]; //将第二个字符的后4位与第三个字符的前2位组合并找出对应的结果字符  
        res[i+3]=base64_table[str[j+2]&0x3f]; //取出第三个字符的后6位并找出结果字符  
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