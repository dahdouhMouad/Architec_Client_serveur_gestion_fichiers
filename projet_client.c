#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#define buffer_size 1024
int Menu()
{
      int choix;
       printf ("\t\t 1. Upload a file\n");
       printf ("\t\t 2. Download a file\n");
       printf ("\t\t 3. Delete a file\n");
       printf ("\t\t 4. List your files \n");
       printf ("\t\t 5. Quitter \n");
       printf ("\t\t\t  Votre choix : ");
       scanf("%d",&choix);
       return choix;
}
int main(int argc,char *argv[])
{
    struct  sockaddr_in  server;
    int sock,choix;
    char ch1[1024];
    char ch2[1024],file_name1[256],file_name2[256];
    int len;
    int n = 0;
    int size_f = 0;
    int size_s ;
    struct stat infos;
    int dhd;
    int hyt;
    char buf[buffer_size];
   if((sock=socket(AF_INET,SOCK_STREAM,0))==-1)
     {
       perror("socket : ");
       exit(-1);
     }
        bzero((char *) &server, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_port = htons(atoi(argv[2]));
        server.sin_addr.s_addr = inet_addr(argv[1]);
       bzero(&(server.sin_zero),8);
if (connect(sock,(struct sockaddr *)&server,sizeof(server)) < 0)
{
perror("ERROR connecting");
}
       while(1)
         {
            printf("Login : ");
            scanf("%s",ch1);
            send(sock,ch1,strlen(ch1),0);
            printf("Password : ");
            scanf("%s",ch1);
            send(sock,ch1,strlen(ch1),0);
            len=recv(sock,ch2,1024,0);
            ch2[len]='\0';
            printf("%s \n",ch2);
            if(strcmp(ch2,"1")==0)
               {
                 do
                  {
                     printf("Bienvenue\n");
                     choix=Menu();
                     switch(choix)
                      {
                         case 1 :
                                  size_s=0;
                                  send(sock,"put",strlen("put"),0);
                                  printf("choix 1 \n");
                                  printf("Donner le nom du fichier : ");
                                  scanf("%s",file_name2);
                                   if ((dhd = open(file_name2,O_RDONLY)) == -1)

                                         printf("Le fichier n existe pas\n");
                                  else
                                        {
			                  fstat(dhd,&infos);
	                                  size_f = infos.st_size;
	                                   printf("Taille totale du fichier transmis: %d \n",size_f);
                                           write(sock,&size_f,sizeof(int));
                                           write(sock,file_name2,sizeof(file_name2));
                                           while (size_s < size_f )
					    {

	 					    if ((n=read(dhd,buf,buffer_size)) < 0) {
	   						  perror("Read");
	    						 exit(5);
	  						 }
	   						size_s += n;
	   					if ((hyt = write(sock, buf, n)) != n){
	   					  perror("put write");
	     					  exit(6);
	   						}

				            }
                                          close(dhd);
                                         }
                                  getchar();
                                  break;
                         case 2: printf("choix 2 \n");
                                 break;
                         case 3 :
                                 send(sock,"del",strlen("del"),0);
                                 printf("DOnner le nom du fichier : ");
                                 scanf("%s",file_name1);
                                 send(sock,file_name1,256,0);
                                 recv(sock,ch1,1024,0);
                                 printf("%s",ch1);
                                  break;
                         case 4 :
                                 send(sock,"ls",strlen("ls"),0);
                                 recv(sock,ch1,1024,0);
                                 printf("%s",ch1);
                                  break;
                         case 5 :
                                 close(sock);
                                 exit(0);
                                 break;
                        default :
				  printf("choix incorrecte");
				  break;
                      }
                 }
               while(choix!=5);
              }
       }
        close(sock);
   return 0;
}
