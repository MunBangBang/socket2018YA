 #include <stdio.h> 
  #include <netinet/in.h> 
  #include <sys/socket.h> 
  #include <string.h> 
#include<signal.h>
#include<sys/wait.h>
  #define PORT 9000 
    
  char buffer[BUFSIZ] = "Hi, I'm server\n"; 
   void do_service(int c_socket);
void sig_handler(int signo);
  main( ) 
  { 
  	int   c_socket, s_socket; 
  	struct sockaddr_in s_addr, c_addr; 
  	int   len; 
  	int   n; 
  	int rcvLen; 
signal[SIGCHLD,sig_handler];
  	char rcvBuffer[BUFSIZ], *ptr, *ptr2; 
   	s_socket = socket(PF_INET, SOCK_STREAM, 0); 
  	 
  	memset(&s_addr, 0, sizeof(s_addr)); 
  	s_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
  	s_addr.sin_family = AF_INET; 
  	s_addr.sin_port = htons(PORT); 
    
  	if(bind(s_socket, (struct sockaddr *) &s_addr, sizeof(s_addr)) == -1) { 
  		printf("Can not Bind\n"); 
  		return -1; 
  	} 
    
  	if(listen(s_socket, 5) == -1) { 
  		printf("listen Fail\n"); 
  		return -1; 
  	} 
   	 
  	while(1) { 
  		len = sizeof(c_addr); 
  		c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len); 
  		printf("Client is connected\n"); 
  		pid=fork();
		if(pid>0){//부모프로세스
			clos(c_socket);
		}else if(pid==0){//자식 프로세스
			close(s_socket);
			do_servise(c_socket);
		}else{
		printf("[ERROR]fork failed\n");
		exit(0);
		}
  		if(!strncasecmp(rcvBuffer, "kill server", 11)) 
  			break; 
  	}	 
  	close(s_socket); 
 } 
void do_service(int c_socket){
while(1){ 
  			rcvLen = read(c_socket, rcvBuffer, sizeof(rcvBuffer)); 
  			rcvBuffer[rcvLen] = '\0'; 
  			printf("[%s] received\n", rcvBuffer); 
  		 
  			if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0) 
  				break; 
  			 
  			if(strncmp(rcvBuffer, "안녕하세요", strlen("안녕하세요")) == 0){ 
  				strcpy(buffer, "안녕, 만나서 반가워."); 
  			} else if(strncmp(rcvBuffer, "이름이 머야?", strlen("이름이 머야?"))==0){ 
  				strcpy(buffer, "나 는 배현지야"); 
  			} else if(strncmp(rcvBuffer, "몇 살이야?", strlen("몇 살이야?"))==0){ 
  				strcpy(buffer, "나는 19살이야"); 
  			} else{ 
  				strcpy(buffer, "No data"); 
  			} 
  
 
  			if(strncmp(rcvBuffer, "strlen ", 7) == 0){ 
  				n = strlen(rcvBuffer)-7; 
  				sprintf(buffer, "%d", n); 
  			} else if(strncmp(rcvBuffer, "strcmp ", 7) == 0){ 
  				ptr = strtok(rcvBuffer, " "); 
  
 
  				ptr = strtok(NULL, " "); 
  				ptr2 = strtok(NULL, " "); 
  
 
  				if(strcmp(ptr, ptr2) == 0){ 
  				    	n = 0; 
  					sprintf(buffer, "%d", n); 
  				} else { 
  					n = -1; 
  					sprintf(buffer, "%d", n); 
  				} 
  			} 
  
 
  			write(c_socket, buffer, strlen(buffer)); 
  		} 
  

  		close(c_socket); 
}
void sig_handler(int signo){
	int pid;
		int status;
	pid=wait();//자식 프로세스가 종료될때까지 기다려주는 함수
	printf("pid[%d] process terminated. status[%d]\n",pid,status); 

}
