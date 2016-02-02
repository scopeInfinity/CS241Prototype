#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

char outputBuffer[256];
void execute(char *);
int main( int argc, char *argv[] ) {
   int sockfd, newsockfd, portno, clilen;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;
   int  n;
   
   /* First call to socket() function */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
   
   /* Initialize socket structure */
   bzero((char *) &serv_addr, sizeof(serv_addr));
   portno = 8080;
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);
   
   /* Now bind the host address using bind() call.*/
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }
      
   /* Now start listening for the clients, here process will
      * go in sleep mode and will wait for the incoming connection
   */
   
   listen(sockfd,5);
   clilen = sizeof(cli_addr);
   
   /* Accept actual connection from the client */
   newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
	
   if (newsockfd < 0) {
      perror("ERROR on accept");
      exit(1);
   }
   
   /* If connection is established then start communicating */
   while(1)
   {
      bzero(buffer,256);
      n = read( newsockfd,buffer,255 );
      
      if (n < 0) {
         perror("ERROR reading from socket");
         exit(1);
      }
      execute(buffer);
      //printf("Here is the message: %s\n",outputBuffer);
      
      /* Write a response to the client */
      n = write(newsockfd,outputBuffer,strlen(outputBuffer));
      
      if (n < 0) {
         perror("ERROR writing to socket");
         exit(1);
      }
   }   
   return 0;
}
int executing(char *cmd)
{
       FILE *fp;
      int status;
      char path[256];

      printf("Executing %s",cmd);
      fp = popen(cmd, "r");
      if (fp == NULL)
          /* Handle error */;

      outputBuffer[0]='\0';
      strcpy(outputBuffer,"Executing `");
      strcat(outputBuffer,cmd);
      strcat(outputBuffer,"`\n");

      while (fgets(path, 256, fp) != NULL)
         strcat(outputBuffer,path);


      status = pclose(fp);
      if (status == -1) {
          /* Error reported by pclose() */
         return -1;
         
      } else {
          /* Use macros described under wait() to inspect `status' in order
             to determine success/failure of command executed by popen() */
          return status;
      }
}
void execute(char *cmd)
{
   int status = -2;
   int index=strlen(cmd)-1;
   while(index>=0)
   {
      if(cmd[index]==' ' || cmd[index]=='\n')
         index--;
      else break;
   }
   cmd[index+1]='\0';
   int toallow = 0;
   if(strcmp(cmd,"ls")==0)
      toallow = 1;
   else if(cmd[0]=='c' && cmd[1]=='p')
      toallow = 1;
   else if(cmd[0]=='m' && cmd[1]=='v')
      toallow = 1;
   else if(cmd[0]=='r' && cmd[1]=='m')
      toallow = 1;
   //else if(cmd[0]=='s' && cmd[1]=='h')  //Can be handled later,, for complete shell access
   //   toallow = 1;

   if(toallow)
         status = executing(cmd);

   if(status==-2)
      strcpy(outputBuffer,"Access Denied!");
   else if(status==-1)
      strcpy(outputBuffer,"Error in executing Command");
   else ;//Done   
   

}
