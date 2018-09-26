

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<pthread.h>
#include <openssl/ssl.h>

char key[100000];
char *key_word = "data-currency-value";
int kill=-1;
void * key_search()
{
for(int i=0;i<strlen(key);i++)
if(key[i]=='d')
{
int z=0;
for(;z<strlen(key_word);z++)
if(key[i+z]!=key_word[z])
break;
if(z==strlen(key_word))
{
i+=z+1;
double val;
char buf[20];
int op=0;
for(int l=i;key[l]!='<';l++)
buf[op++]=key[l];
buf[op]='\0';
val = atof(buf);
system("date");
printf("bitcoin : %lf  \n",val);
kill++;
}
}


}



int main(int argc, char** argv)
{
	

	pthread_t th;
	pthread_t tk;
	struct sockaddr_in servaddr;
	struct hostent *hp;
	int sock_id;
	char request[] = "GET /currencies/bitcoin/ HTTP/1.1\r\n"
		"Host: coinmarketcap.com \r\n"
		"User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:61.0) Gecko/20100101 Firefox/61.0 \r\n"
		"\r\n";
               

         

          sock_id = socket(AF_INET,SOCK_STREAM,0);
	
	memset(&servaddr,0,sizeof(servaddr));
	
	
	hp = gethostbyname("coinmarketcap.com");
		
	memcpy((char *)&servaddr.sin_addr.s_addr, (char *)hp->h_addr, hp->h_length);
	
	
	servaddr.sin_port = htons(443);
	servaddr.sin_family = AF_INET;



SSL_load_error_strings ();
SSL_library_init ();
SSL_CTX *ssl = SSL_CTX_new (SSLv23_client_method ());


SSL *sslcon = SSL_new(ssl);
SSL_set_fd(sslcon, sock_id);
	
	
	connect(sock_id, (struct sockaddr *)&servaddr, sizeof(servaddr));
	
	
SSL_connect(sslcon);
	
SSL_write(sslcon,request,sizeof(request));
	int stat = 0;
	char buf;
	int count =0;
	int f=0;
	char *str="data-currency-value";
	while(1)
	{
	SSL_read(sslcon,&buf,1);
//	putchar(buf);
        key[f++] = buf;	
       if((buf==13) || (buf=='\n'))
       {
       key[f]='\0';
       f=0;
       pthread_create(&tk,NULL,(void *)&key_search,NULL);     
       pthread_join(th,NULL);
 //      puts("recieved thread");
       if(kill==0)
	     exit(0);
       }	       
	if(buf=='\n')
		count++;
	else
		count=0;
	if(count==10)
	{
		exit(0);
		
	}
	}

	return 0;


}
