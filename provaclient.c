#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

#define MSG_LOGIN 'L'
#define MSG_REGLOG 'R'
#define MSG_OK 'O'
#define MSG_ERROR 'E'
#define MSG_SINGLE 'S'
#define MSG_BRDCAST 'B'
#define MSG_LIST 'I'
#define MSG_LOGOUT 'X'

typedef struct {
	char type;
	char *sender;
	char *receiver;
	char msglen;
	char *msg;
} msg_t;

void lettura_da_file(){
	 FILE *fd;
  char buf[200];
  


/* apre il file */
  fd=fopen("righe.txt", "r");
  if( fd==NULL ) {
    fprintf(stderr,"Errore in apertura del file");
    exit(1);
  }


/* legge e stampa ogni riga */
  while(fgets(buf, 200, fd)) {
	  
	  printf("%s", buf);
  }


/* chiude il file */
fclose(fd);
	
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n, c=-1;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int count=0;

    char buffer[256];

	//da ciao.c
	char *username, a[3]={'-', 'h', '\0'}, b[5]={'-', 'r', '\0'};
	
	//buffer che conterrà il messaggio di registrazione
	char buffer_message[256];
	//pulisco il buffer
	bzero( (char*)&buffer_message, sizeof(buffer_message));
	//array per convertire l'intero in stringa
	char arr[50];
	//puntatore alla struct
	msg_t *t;
	//dichiaro una variabile di tipo struct msg_t per stampare il contenuto dei campi puntati da t
	msg_t p;
	//prende l'indirizzo della struttura
	t=&p;
	//variabile che contiene la lunghezza dell'username
	int lunghezza;

		if(strcmp(argv[1], a) == 0){
		if(argc==3){
			lettura_da_file();
			username=argv[argc-1];			
			printf(" username:%s\n",username);
		
		}else{
			fprintf(stderr, "ERROR arguments\n");
		}
	} 
	else if (strcmp(argv[1], b) == 0){
			if(argc==6){
				//anche qua mi interessa sapere l'username
				username=argv[argc-1];

				lunghezza=strlen(username);
				//printf("%s", username);
				/*FASE DI REGISTRAZIONE*/
				
				//riempio i campi della struct per la registrazione
				t->type=MSG_REGLOG;
				t->sender=NULL;
				t->receiver=NULL;
				t->msglen=lunghezza;
				t->msg=username;
				//stampe a video dei campi della struttura
				//printf("%d\n", t->type);
				//printf("%s\n", t->sender);		//è NULL
				//printf("%s\n", t->receiver);	//è NULL
				//printf("%d\n", t->msglen);						
				//printf("%s\n", t->msg);

				//copio la struct sul buffer
				*buffer_message=t->type;
				sprintf(arr, "%d", t->msglen);
				strcat(buffer_message, arr);
				strcat(buffer_message, t->msg);

				printf("%s\n", buffer_message);
				printf("\n@--Ti sei registrato come %s--@\n", username);			
		}else{
			fprintf(stderr, "ERROR arguments\n");
		}
	}
	else{
		fprintf(stderr, "ERROR INPUT ARGUMENTS\n");
	}
	//FINE ciao.c


    //if (argc < 3) {
    //    fprintf(stderr,"usage %s hostname port\n", argv[0]);
    //    exit(0);
    //}
    //portno = atoi(argv[2]);


    /* Create a socket point */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        perror("ERROR opening socket");
        exit(1);
    }
    //server = gethostbyname(argv[1]);
    ////if (server == NULL) {
    ////    fprintf(stderr,"ERROR, no such host\n");
    ////    exit(0);
    ////}

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    //bcopy((char *)server->h_addr, 
    //       (char *)&serv_addr.sin_addr.s_addr,
    //            server->h_length);
    serv_addr.sin_port = htons(5001);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    /* Now connect to the server */
    while (count<10 && c==-1){
		c=connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
			if (c==-1){
				count++;
				sleep(1);
			//if(errno==ENOENT)
		
	//		else
	//			exit(EXIT_FAILURE);
		}
	}
	



	printf("count:%d\tc:%d\n",count,c);


	
    //}
    //if (connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0) 
    //{
    //     perror("ERROR connecting");
    //     exit(1);
    //}	

	if(count==10){
		fprintf(stderr, "ERROR connecting\n");
		exit(0);
	}



    /* Now ask for a message from the user, this message
    * will be read by server
    */
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    /* Send message to the server */
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
    {
         perror("ERROR writing to socket");
         exit(1);
    }
    /* Now read server response */
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
    {
         perror("ERROR reading from socket");
         exit(1);
    }
    printf("%s\n",buffer);
    return 0;
}
