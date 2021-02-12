#include<stdio.h>
#include<sys/socket.h>
#include<errno.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<strings.h>
#include<string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#define NMAX  1000
#define buffer_size 1024
typedef struct {
	int id;
	char nom[50];
	char prenom[50];
	char login[30];
	char password[30];
	int etat;
}utilisateur;

char*   menu(char  *FL)
{
   strcpy(FL,"\0");
   strcat(FL,"\t\t 1. Add user \n");
   strcat(FL,"\t\t 2. Show list_users \n");
   strcat(FL,"\t\t 3. Find user\n");
   strcat(FL,"\t\t 4. Delete user\n");
   strcat(FL,"\t\t 5. Save user\n");
   strcat(FL,"\t\t 6. Restore user  \n");
   strcat(FL,"\t\t 7. Run server\n");
   strcat(FL,"\t\t 8. Shut down server \n");
   strcat(FL,"\t\t 9. Exit\n\n");
   strcat(FL,"\0");
   return FL;
}

void   ADD(utilisateur *A,int *N)
{
	utilisateur nouveau;
	printf(" Insert your last name\n");
	scanf("%s",nouveau.nom);
	printf(" Insert your first name\n");
	scanf("%s",nouveau.prenom);
	printf(" Insert A Login\n");
	scanf("%s",nouveau.login);
	printf(" Insert A Password\n");
	scanf("%s",nouveau.password);
	nouveau.etat=1;
	if(*N==0)
	  nouveau.id=*N+1;
	else
	   nouveau.id=A[*N-1].id + 1;

	A[*N]=nouveau;
	(*N)++;
}

void SHOW(utilisateur *A,int N)
{
	int i;
  for(i=0;i<N;i++)
   printf("%d:%s:%s:%s:%s:%d\n",A[i].id,A[i].nom,A[i].prenom,A[i].login,A[i].password,A[i].etat);
}

int SEARCH(utilisateur *A,int N, char *logr)
{
	int i;
	for(i=0;i<N;i++)
	{
		if(strcmp(A[i].login,logr)==0)
		return i;
	}
	 return -1;
}
void DELETE(utilisateur *A,int *N, char *logr)
{
	int i,position;
	position=SEARCH(A,*N,logr);
	if(position==-1)
	  printf("Aucun Utilisateur avec ce login \n");
	else
	{
		for(i=position;i<*N-1;i++)
	    A[i]=A[i+1];
	  (*N)--;
	  printf("Utilisateur a ete supprime\n");
	}
}
void SAVE(utilisateur *A,int N,char *nomfichier)
{
	FILE *P;
	int i;
	P=fopen(nomfichier,"a");
	if(P==NULL)
	  {
	  	printf("Erreur d'ouverture du fichier  \n");
	  	return;
	  }
	for(i=0;i<N;i++)
     fprintf(P,"%d\t%s\t%s\t%s\t%s\t%d\n",A[i].id,A[i].nom,A[i].prenom,A[i].login,A[i].password,A[i].etat);
     printf("La liste est sauvegardee\n");
	 fclose(P);
}

void RESTORE(utilisateur  *A,int *N,char *nomfichier)
{
	FILE *P;
	int i=0;
	P=fopen(nomfichier,"r");
	if(P==NULL)
	  {
	  	printf("Erreur d'ouverture du fichier  \n");
	  	return ;
	  }
    else
    {
	while(!feof(P))
	 {
	 	fscanf(P,"%d\t%s\t%s\t%s\t%s\t%d\n",&A[i].id,A[i].nom,A[i].prenom,A[i].login,A[i].password,&A[i].etat);
		    if(!feof(P))
			i++;
	 }
	  *N=i;
    }
}

int CHECK(utilisateur *A,int N,char *FL,char *ch2)
{
   int i;
   for(i=0;i<N;i++)
   {
       if(strcmp(A[i].login,FL)==0 && strcmp(A[i].password,ch2)==0)
      return 1;
   }
 return 0;
}

int demarrer(int port)
{
 struct sockaddr_in serveur;
  int s_serveur , s_client , len;
   s_serveur=socket(AF_INET,SOCK_STREAM,0);
   if(s_serveur==-1)
 {
     perror("probleme socket");
      exit(-1);
 }
     serveur.sin_family=AF_INET;
     serveur.sin_port=htons(port);
     serveur.sin_addr.s_addr=INADDR_ANY;
len=sizeof(serveur);
     if(bind(s_serveur,( struct sockaddr*)&serveur,sizeof(serveur))<0)

   {
       perror("erreur  de la fonction bind");
       exit(-1);
   }
     if (listen(s_serveur,4)==-1)
   {
       perror("probleme listen");
       exit(-1);
   }
  return s_serveur;
}
int main(int argc,char *argv[])
{
   FILE *F;
   utilisateur   T[NMAX],*X;
   int j=0,b;
   int i,choix,ind,Number=0, pro_socket, new_socket,pid,a;
   int s_client, client_socket[30],len, max_clients = 30,max_sd,fichiersize;
   char FL[30],Login[30],Password[30],Road[256],commande[256],cmd[256],buffer[256],path[256],nom_fichier[256];
   struct sockaddr_in client ;
   int fichier;
   int n = 0;
   int rcvt;
   char buf[buffer_size];
   fd_set readfds;

   for (i = 0; i < max_clients; i++)
        {
          client_socket[i] = 0;
        }
 do
    {
       printf ("\t\t 1. Add user\n");
       printf ("\t\t 2. Show list_users \n");
       printf ("\t\t 3. Find user \n");
       printf ("\t\t 4. Delete user\n");
       printf ("\t\t 5. Save user\n");
       printf ("\t\t 6. Restore user\n");
       printf ("\t\t 7. Run server \n");
       printf ("\t\t 8. Shut down server \n");
       printf ("\t\t 9. Exit \n\n");
       printf ("\t\t\t  Choose an option : ");
	   scanf("%d",&choix);
     switch(choix)
      {
      	case 1 : ADD(T,&a);
      	         break;
      	case 2 : SHOW(T,a);
      	         break;
        case 3 : printf("Insert Your Login");
                 scanf("%s",FL);
                 ind=SEARCH(T,a,FL);
		        if(ind==-1)
		          printf("This Login  doesnt existe");
		        else
		           printf("%d:%s:%s:%s:%s:%d\n",T[ind].id,T[ind].nom,T[ind].prenom,T[ind].login,T[ind].password,T[ind].etat);
      	         break;
      	case 4 :
                 printf("Insert Your Login ");
                 scanf("%s",FL);
                 DELETE(T,&a,FL);
                 break;
        case 5 :
                 SAVE(T,a,"utilisateur.txt");
                 break;
        case 6 :
                 RESTORE(T,&a,"utilisateur.txt");
                 printf("size %d",a);
                 break;
        case 7 :
                 pro_socket=demarrer(70000);
                 while (1)
	   {
                s_client=accept(pro_socket,(struct sockaddr*)&client,&len);
               printf("desc : %d, machine : %s, port : %d \n",s_client,inet_ntoa(client.sin_addr.s_addr),ntohs(client.sin_port));

                 pid=fork();
                  if(pid==0)
                     {
                     while(Number<3)
			{
                     recv(s_client,Login,256,0);
                     recv(s_client,Password,256,0);
                   if(CHECK(T,a,Login,Password)==1)
    		 {
                         send(s_client,"1",strlen("1"),0);
                         strcpy(Road,"/home/Projet/");
        		 strcat(Road,Login);
    			 strcat(Road,"\0");
                         mkdir(Road,0755);
                         printf("%s\n",Road);
                       while(1)
                         {
                            j++;
                         printf("%d\n",j);
                         strcpy(cmd,"");
                         printf("%s \n",cmd);
                         recv(s_client,cmd,256,0);
                         printf(" commande client %s\n",cmd);
                        if(strcmp(cmd,"put")==0)
                         {
                            read(s_client,&fichiersize,sizeof(int));
                            read(s_client,nom_fichier,sizeof(nom_fichier));
                            printf("Size of the file : %d\n",fichiersize);
			                printf("name of the file: %s\n",nom_fichier);
                            fflush(stdout);
                            strcpy(path,"/home/Projet/");
                            strcat(path,Login);
                            strcat(path,"/");
                            strcat(path,nom_fichier);
                            fichier = open(path, O_WRONLY | O_CREAT | O_TRUNC,0666);
                            rcvt=0;
                             while(rcvt < fichiersize )
                             {
	                         if((n=read(s_client,buf,buffer_size)) < 0)
                                   {
	                             perror(" Probleme Read");
	                             exit(7);
	                           }
	                       rcvt += n;
	                      if (write(fichier,buf,n) != n)
                                 {
	                           perror(" Probleme write");
	                          exit(7);
	                          }
                            }
                          close(fichier);
                        }
                         else if(strcmp(cmd,"ls")==0)
                           {
                         strcpy(commande,"ls -l /home/Projet/");
                         strcat(commande,Login);
                         strcat(commande,"  > tmp");
                         system(commande);
                         F=fopen("tmp","r");
                         i=0;
                          while(fgets(buffer,256,F)!=NULL)
                            {
                             puts(buffer);
                             send(s_client,buffer,strlen(buffer),0);
                            }
                          }
                        else if(strcmp(cmd,"del")==0)
                          {
                            strcpy(path,"/home/Projet/");
                            strcat(path,Login);
                            strcat(path,"/");
                            recv(s_client,nom_fichier,256,0);
                            strcat(path,nom_fichier);
                       b=remove(path);
                            if(b==0)
                               printf("%s \n",path);
                            else
                              printf("Probleme de suppression");
                          }
                        }
                     Number=0;
                   break;
              }
		else if(Number<3)
   		   {
  		       send(s_client,"2",strlen("3"),0);
                       Number++;
                   }
               if(Number==3)
                {
                  send(s_client,"4",strlen("4"),0);
                  close(s_client);
                 }
                }
               }
              }
                 break;

       	case 8 : exit(0);
       	case 9 : exit(0);
      	default : printf("Choix incorrecte \n");
      	        break;
	  }
    }
  while(choix !=9);
  system("pause");
  return 0;
}
