#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
// 2-1. 서버 프로그램이 사용하는 포트를 9000 --> 10000으로 수정 
#define PORT 9000
//#define PORT 10000
 
// 2-2. 클라이언트가 접속했을 때 보내는 메세지를 변경하려면 buffer을 수정
//char buffer[100] = "hello, world\n";
char buffer[100] = "ㅇㅇㅇ"; 
main( )
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;
	int   n;
	int rcvLen;
	char rcvBuffer[100], *ptr1, *ptr2;
 	s_socket = socket(PF_INET, SOCK_STREAM, 0); //소켓을 생성
	
	memset(&s_addr, 0, sizeof(s_addr));
	//s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT);  // 연결 요청을 수신할 주소 설정
 
	if(bind(s_socket, (struct sockaddr *) &s_addr, sizeof(s_addr)) == -1) { //소켓을 포트에 연결
		printf("Can not Bind\n");
		return -1;
	}
 
	if(listen(s_socket, 5) == -1) { //커널에 개통을 요청
		printf("listen Fail\n");
		return -1;
	}
 	
	while(1) {
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);//연결 요청을 수신
		//3-3.클라이언트가 접속했을 때 "Client is connected" 출력
		printf("Client is connected\n");
		while(1){
			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer)); //클라이언트에게 서비스를 제공
			rcvBuffer[rcvLen] = '\0';
			if(strncmp(rcvBuffer, "이름이 머야?", strlen("이름이 머야?")) == 0)
			{
					strcpy(buffer, "내 이름은 지수현이야");
			}
			else if(strncmp(rcvBuffer, "몇 살이야?", strlen("몇 살이야?")) == 0)
			{
				strcpy(buffer,"나는 23살이야");
			}
			else if(strncmp(rcvBuffer, "안녕하세요", strlen("안녕하세요"))==0)
			{
				strcpy(buffer, "안녕하세요. 만나서 반가워요");
			}
	
			else if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
				break;
			else
			{
				strcpy(buffer, "no");
			}

		
			if(strncmp(rcvBuffer, "strlen ", 7)==0)
			{
				n = strlen(rcvBuffer)-7;
				sprintf(buffer, "%d", n);
				
			}
			else if(strncmp(rcvBuffer, "strcmp", 7)==0)
			{
				ptr1 = strtok(rcvBuffer, " ");
			
				ptr1 = strtok(NULL, " ");
				ptr2 = strtok(NULL, " ");

				if(strcmp(ptr1, ptr2)==0) 
				{
					n=0;
					sprintf(buffer, "%d", n);
				}
				else
				{
					n=1;
					sprintf(buffer, "%d", n);
				}
			}
			write(c_socket, buffer, strlen(buffer));
				
		}
		
		
		close(c_socket);
		if(!strncasecmp(rcvBuffer, "kill server", 11))
			break;
	}	
	close(s_socket);  //클라이언트와의 연결을 종료
}
