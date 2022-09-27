#include <fcntl.h> /* For open(), O_APPEND */
#include <unistd.h> /* For close() */
#include <stdio.h> /* For puts() */
#include <stdlib.h> /* For exit() */
 
const int BUFFER_SIZE = 256; //the buffer size of the appender.
 
int main(int argc, char* argv[])
{
   int src_fd;                        //file descriptor for the source file
   int des_fd;                        //file descriptor for the destination file
   int read_count;                    //Bytes actually read each time
   mode_t f_attrib;                   //file permission
   unsigned char buffer[BUFFER_SIZE]; //Buffer holds the stream
 
   if(argc != 3) //Usage
       printf("Usage: %s <SOURCE FILE> <DESTINATION FILE>\n", argv[0]);
 
   const char *src_fd_path = argv[1];
   const char *des_fd_path = argv[2];
   src_fd = open(src_fd_path, O_RDONLY); //open the source file in read only mode
   f_attrib = S_IRWXU | S_IRWXG | S_IRWXO; //Set permission for the not exist destination file
   des_fd = open(des_fd_path, O_WRONLY | O_APPEND | O_CREAT, f_attrib); //open the destination file
   if (src_fd == -1) {                                                  //Check failure
      puts("Source file open failed.");
      close(src_fd);
      exit(1);
   }
   if (des_fd == -1) {                                                  
	puts("Destination file open failed.");
      	close(des_fd);
      	exit(1);
   }
//	Task3	
//
//	if (lseek(src_fd,10,SEEK_SET)==-1){
//		printf("lseek error\n");
//		exit(1);
//	}
//	if (lseek(des_fd,10,SEEK_SET)==-1){
//		printf("lseek error\n");
//		exit(1);
//	}
   while((read_count = read(src_fd, buffer, BUFFER_SIZE)) != 0) write(des_fd, buffer, 	read_count); //Append the file
   puts("Append complete.");
 
   close(src_fd); //close the file descriptor
   close(des_fd);
   return 0;

}
