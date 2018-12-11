#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

#define MAX_CLIENT 2
#define CHATDATA 1024
#define INVALID_SOCK -1
#define PORT 9000
#define USERNAME 256

struct user{
	char name[USERNAME];
	int sock;
};

void *do_chat(void *); //채팅 메세지를 보내는 함수
int pushClient(int); //새로운 클라이언트가 접속했을 때 클라이언트 정보 추가
int popClient(int); //클라이언트가 종료했을 때 클라이언트 정보 삭제

//int    list_c[MAX_CLIENT]; //접속한 클라이언트를 관리하는 배열
struct user list_c[MAX_CLIENT];
char    escape[ ] = "exit";
char    greeting[ ] = "Welcome to chatting room\n";
char    CODE200[ ] = "Sorry No More Connection\n";

pthread_t thread;
pthread_mutex_t mutex;

int main(int argc, char *argv[ ])
{
    int c_socket, s_socket;
    struct sockaddr_in s_addr, c_addr;
    int    len;
    int    i, j, n;
    int    res;

    if(pthread_mutex_init(&mutex, NULL) != 0) {
        printf("Can not create mutex\n");
        return -1;
    }

    s_socket = socket(PF_INET, SOCK_STREAM, 0);
    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT);

    if(bind(s_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1) {
        printf("Can not Bind\n");
        return -1;
    }

    if(listen(s_socket, MAX_CLIENT) == -1) {
        printf("listen Fail\n");
        return -1;
    }

    for(i = 0; i < MAX_CLIENT; i++)
        list_c[i].sock = INVALID_SOCK;

    while(1) {
        len = sizeof(c_addr);
        c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
        res = pushClient(c_socket); //접속한 클라이언트를 list_c에 추가
        if(res < 0) { //MAX_CLIENT만큼 이미 클라이언트가 접속해 있다면,
            write(c_socket, CODE200, strlen(CODE200));
            close(c_socket);
        } else {
            write(c_socket, greeting, strlen(greeting));
            //pthread_create with do_chat function.
            pthread_mutex_lock(&mutex);
            pthread_create(&thread, NULL, do_chat, (void*)&c_socket);
	    pthread_mutex_unlock(&mutex);
        }
    }
}

void *do_chat(void *arg)
{
    int c_socket = *((int *)arg);
    char chatData[CHATDATA];
    int i, n;
    char *ptr;

    while(1) {
        memset(chatData, 0, sizeof(chatData));
        if((n = read(c_socket, chatData, sizeof(chatData))) > 0) {

	    if(chatData[0]=="/"){
		
            }else if(strstr(chatData, escape) != NULL) {
                popClient(c_socket);
                break;
            }else{
		//write chatData to all clients
            	for(i=0; i<MAX_CLIENT; i++){
			if(list_c[i].sock==INVALID_SOCK)	break;
			write(list_c[i].sock, chatData, n);
	    	}
	    }
        }

    }


}

int pushClient(int c_socket) {
	int i, n;
	char nickname[USERNAME];

	n = read(c_socket, nickname, sizeof(nickname));
	nickname[n] = '\0';

	for(i=0; i<MAX_CLIENT; i++){
		if(list_c[i].sock==INVALID_SOCK){
			list_c[i].sock = c_socket;
			strcpy(list_c[i].name, nickname);
			printf("[%d] %s 님이 접속하였습니다.\n", i+1, list_c[i].name);
			return i;
		}
	}

	if(i==MAX_CLIENT)	return -1;
}

int popClient(int c_socket)
{
	int i, j;

	close(c_socket);

	for(i=0; i<MAX_CLIENT; i++){
		if(list_c[i].sock==c_socket){
			printf("%s님이 퇴장하셨습니다.\n", list_c[i].name);
			for(j=i; j<MAX_CLIENT; j++){
				list_c[j].sock=list_c[j+1].sock;
				if(list_c[j].sock==INVALID_SOCK)	break;
			}
		}
	}

	return j;
}
