# POP3-email-server

## 1. Overview
The requirement and target of the project should be introduced.

The target of the project is to develop a POP3 client.
1.	Receive Email from pop3 server (pop.163.com) and store it as *.eml file.
2.	Friendly User Interface and good interactive User Experience (no GUI required).
Requirements: 
![image](https://user-images.githubusercontent.com/58174623/192473283-d4318851-afbf-46e5-89cf-30323bd1e662.png)

## 2. Requirements Analysis
The requirement of the project is analyzed here, including, 
- Development Environment, such as OS and programming language
-	Functional requirements in details, including task decomposition and analysis

Development environment: Linux OS with “gcc” complier using C language.

Functional requirements: 
### Connect POP3 server on port 110: 
Construct a socket which domain name is pop.163.com and port number is 110.
Using “connect” to connect the pop3 server.
Using “select” to listen the socket buffer, and then use FD_ZERO, FD_SET, FD_ISSET operation to achieve write user input to socket buffer or read server message from the socket buffer and use “send” and “recv” to send message to server and receive message from server.
### Guide user to login and show information about their mails:
Read user input username and use strcat() to construct the “USER” message send to server.
Read user input password and use strcat() to construct the “PASS” message send to server.
After successfully login, user can send “LIST” and “STAT” message to server to check information about their emails.

### Can display content of mails in terminal:
Send “RETR” message, and sleep 1s before using recv() to receive server feedback in order to receive all the content in one time. Then use printf() to display the buffer content.
Using “strstr()” to find the content plain text encode in base64, and then use base64_decode function to display the content in ASCII.

### The communication procedure can be captured by wireshark:
Using “$sudo wireshark” command to open wirshark process and in the wireshark interface set the recrive filter as “tcp port 110”. And then can captured POP packet from server.

### Login with implicit password:
Using “termio.h” to replace user input password as *.

### Can download mails and save them only on local machine (remove from remote server):
Using “fopen()” to open an local file and write the recv content from server to the file.
Then send “DELE” to server to delete the corresponding mail.

### Display by subject:
Using “strstr()” to find the Begin and End pointer for the subject and print it into terminal.
Using for to check each mail by sequence.

### Search text in all mails:
Using for to check whether user input is in the email by comparing each char.
Using for to check each mail by sequence.
![image](https://user-images.githubusercontent.com/58174623/192473446-e0290da5-2d0c-448d-a14b-66245cd75e50.png)

## 3. Detailed Design
Detailed design includes 
- Design analysis of each module 
- Flow chart of each module

Design analysis of each module:
### Client socket module:
Construct a socket using tcp which domain name is pop.163.com and port number is 110.
Using “connect” to connect the pop3 server.
Using “select” to listen the socket buffer, and then use FD_ZERO, FD_SET, FD_ISSET operation to achieve write user input to socket buffer or read server message from the socket buffer and use “send” and “recv” to send message to server and receive message from server.
 
### User Login Module:
Read user input username and use strcat() to construct the “USER” message then send to server.
Read user input password and use strcat() to construct the “PASS” message then send to server.
 
### Mail State Module:
After successfully login, user can send “LIST” and “STAT” message to server to check information about their emails.
 
### Content display module:
Send “RETR” message, and sleep 1s before using recv() to receive server feedback in order to receive all the content in one time. Then use printf() to display the buffer content.
Using “strstr()” to find the content plain text encode in base64, and then use base64_decode function to display the content in ASCII.
 
### Search text module:
Using for to check whether user input is in the email by comparing each char.
Using for to check each mail by sequence.
 
### Subject display module:
Using “strstr()” to find the Begin and End pointer for the subject and print it into terminal.
Using for to check each mail by sequence.
 
### Mail download and delete module:
Can download mails and save them only on local machine (remove from remote server):
Using “fopen()” to open an local file and write the recv content from server to the file.
Then send “DELE” to server to delete the corresponding mail.
 
![image](https://user-images.githubusercontent.com/58174623/192474010-c01699d7-3c3f-4868-99a2-ba9f86b8d164.png)

