#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/time.h>
#include <sys/ioctl.h>

#include <ctype.h>

#include "constant.h"
#include "struct.c"
/* port we're listening on */

void load_account(char* list[])
{
    FILE * file;
    char account[100];
    int i = 0;
    file = fopen("account.txt","r");
    if(file == NULL){
        printf ("# Error open account list file!!\n");
        exit(1);
    }
    
    list[0] = (char *)malloc(200);
    
    printf("+ Account list:\n");
    while(fscanf(file,"%s",account) == 1) {
        strcpy(list[i],account);
        i++;
        list[i]=(char *)malloc(200);
        printf ("\t(%d) %s\n",i, account);
        strcpy(list[i],"");
    }
    
    fclose(file);
}

int check(char * account,char* list[])
{
    int i = 0;
    
    while ((list[i] != NULL) && (strcmp(list[i],"") != 0)) {
        if (strcmp(account, list[i]) == 0) {
            printf ("- Account %s found!\n", account);
            return 1;
        }
        i++;
    }
    printf("# No account found!\n");
    return 0;
}

int main(int argc, char *argv[])
{
    char* list[200];
    FILE * file;
    time_t lt;
    char user_invite[50];
    Player p1,p2;
    Match g1;
    Player * plist = (Player *)malloc(sizeof(Player) * 100);

    Match * glist = (Match *)malloc(sizeof(Match) * 10);
    char username[50], passwd[50], buff[500];
    char address[100];
    int soc1 = 0,soc2 = 0;
    long bytes_recieved = 0;
    char recv_data[1024];
    
    /* master file descriptor list */
    fd_set master;
    /* temp file descriptor list for select() */
    fd_set read_fds;
    /* server address */
    struct sockaddr_in serveraddr;
    /* client address */
    struct sockaddr_in clientaddr;
    /* maximum file descriptor number */
    int fdmax;
    /* listening socket descriptor */
    int listener;
    /* newly accept()ed socket descriptor */
    int newfd;
    int nbytes;
    /* for setsockopt() SO_REUSEADDR, below */
    int yes = 1;
    unsigned int addrlen;
    int i, j;
    /* clear the master and temp sets */
    
    FD_ZERO(&master);
    FD_ZERO(&read_fds);
    /* get the listener */
    load_account(list);
    if((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Server-socket() error!!\n");
        exit(1);
    }
    printf("Server-socket() is OK...\n");
    
    /*"address already in use" error message */
    if(setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("# Server-setsockopt() error!!\n");
        exit(1);
    }
    
    printf("Server-setsockopt() is OK...\n");
    
    /* bind */
    
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(PORT);
    memset(&(serveraddr.sin_zero), '\0', 8);
    
    if(bind(listener, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1) {
        perror("# Server-bind() error!!\n");
        exit(1);
    }
    
    printf("Server-bind() is OK...\n");

    /* listen */
    
    if(listen(listener, 10) == -1) {
        perror("# Server-listen() error!!");
        exit(1);
    }
    
    printf("Server-listen() is OK...\n");
    
    printf("*START* Server start successfully!...\n");
    
    /* add the listener to the master set */
    FD_SET(listener, &master);
    /* keep track of the biggest file descriptor */
    fdmax = listener; /* so far, it's this one*/
    /* loop */
    
    for(;;)
    {
        /* copy it */
        read_fds = master;
        if(select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1)
        {
            perror("Server-select() error!!\n");
            exit(1);
        }
        printf("Server-select() is OK...\n");
        /*run through the existing connections looking for data to be read*/
        
        for(i = 0; i <= fdmax; i++)
        {
            if(FD_ISSET(i, &read_fds))
            { /* we got one... */
                if(i == listener)
                {
                    /* handle new connections */
                    addrlen = sizeof(clientaddr);
                    if((newfd = accept(listener, (struct sockaddr *)&clientaddr, &addrlen)) == -1) {
                        perror("# Server-accept() error!!\n");
                    }
                    else {
                        strcpy(address,(char *)inet_ntoa(clientaddr.sin_addr));
                        printf("Server-accept() is OK...\n");
                        FD_SET(newfd, &master); /* add to master set */
                        if(newfd > fdmax)
                        { /* keep track of the maximum */
                            fdmax = newfd;
                        }

                        printf("*CONNECT* New connection from %s on socket %d\n", inet_ntoa(clientaddr.sin_addr), newfd);
                    }
                }
                
                else
                {
                    /* handle data from a client */
                    ioctl(i, FIONREAD, &nbytes);
                    if(nbytes <= 0) {
                        /* got error or connection closed by client */
                        if(nbytes == 0)
                        /* connection closed */
                        {
                            printf("*DISCONNECT* Socket %d hung up\n", i);
                            p1 = find_player_by_socket(plist,i);
                            if (p1.name != NULL)
                                if (strcmp(p1.name,"") != 0)
                                    remove_player(plist, p1);
                            printf ("\t- Remove player success!\n");
                            
                            g1 = find_match_by_socket(glist,i);
                            if (strcmp(g1.log,"NA") != 0)
                                remove_match(glist, g1);
                            printf("\t- Remove game with player success!\n");
                        }
                        else
                            perror("# recv() error!!\n");
                        /* close it... */
                        close(i);
                        /* remove from master set */
                        FD_CLR(i, &master);
                    }
                    else {
                        /* we got some data from a client*/
                        bytes_recieved = recv(i, recv_data, 1024, 0);
                        recv_data[bytes_recieved] = '\0';
                        printf ("\t#### recvdata = %s\n",recv_data);
                        
                        if (strcmp(recv_data, "login") == 0) { // LOGIN
                            printf("*LOGIN* Login process\n");
                            bytes_recieved = recv(i,recv_data,1024,0);
                            recv_data[bytes_recieved] = '\0';
                            printf("\tUser: %s\n", recv_data);
                            strcpy(username, recv_data);
                            strcpy(passwd, recv_data);
                            
                            if (check(username,list))
                            {
                                send(i,"ok",2,0);
                                
                                printf("Current player list:\n");
                                j=0;
                                while(1)
                                {
                                    if (strcmp(plist[j].name,"")==0) break;
                                    
                                    printf ("(%d) %s\n", j, plist[j].name);
                                    j++;
                                }
                                bytes_recieved=recv(i,recv_data,1024,0);
                                recv_data[bytes_recieved] = '\0';
                                if (strcmp(recv_data,"loginok")==0)
                                    send (i,plist,sizeof(Player)*100,0);
                                
                                p1 = new_player(i, 0, 0, 1, username, address);
                                add_player(plist,p1);
                            }
                            else
                            {
                                send(i,"not",3,0);
                            }
                        }
                        /*else if (strcmp(recv_data, "register")==0) // REGISTER
                        {
                            printf("register process\n");
                            send(i,"continue",8,0);
                            bytes_recieved=recv(i,recv_data,1024,0);
                            recv_data[bytes_recieved] = '\0';
                            if(check(recv_data,list))
                            {
                                send(i,"not",3,0);
                            }
                            else
                            {
                                f=fopen("acclist.txt","a");
                                if(f==NULL){
                                    printf ("error open list acc file to write\n");
                                    exit(1);
                                }
                                
                                fputs(recv_data,f);
                                fclose(f);
                                send(i,"ok",2,0);
                            }
                        }*/
                        
                        else if(strcmp(recv_data, "invite") == 0) {
                            printf("*INVITE* Invite process\n");
                            bytes_recieved = recv(i,recv_data,1024,0);
                            recv_data[bytes_recieved] = '\0';
                            strcpy(user_invite,recv_data);
                            soc2 = find_sock_by_player_name(plist,user_invite);
                            
                            soc1 = i;
                            
                            p1 = find_player_by_socket(plist,soc1);
                            p2 = find_player_by_socket(plist,soc2);
                            
                            g1 = new_match(p1,p2);
                            add_match(glist,g1);
                            
                            file = fopen(g1.log,"w");
                            if (file == NULL) {
                                printf ("1 error open log file to write\n");
                                exit(1);
                            }
                            
                            strcpy(buff,"");
                            lt = time(NULL);
                            strcat(buff,ctime(&lt));
                            strcat(buff,p1.name);
                            strcat(buff," play with ");
                            strcat(buff,p2.name);
                            strcat(buff,"\n");
                            
                            fputs(buff, file);
                            fclose(file);
                            printf("-+ found sock = %d\n", soc2);
                            send(soc2,"invitation",10,0);
                        }
                        
                        else if (strcmp(recv_data, "y")==0)
                        {
                            printf("connect %d with %d\n", soc1, soc2);
                            send(soc1,"accept",6,0);
                        }
                        else if (strcmp(recv_data, "n")==0)
                        {
                            printf ("not accept connect!!\n");
                            send(soc1,"refuse",6,0);
                            g1 = find_match_by_socket(glist,soc1);
                            if (strcmp(g1.log, "NA") != 0) {
                                remove_match(glist,g1);
                            }
                        }
                        else if(strcmp(recv_data, "win")==0)
                        {
                            send(i,"continue",8,0);
                            bytes_recieved=recv(i,recv_data,1024,0);
                            recv_data[bytes_recieved] = '\0';
                            send(find_player_socket(glist,i), recv_data, strlen(recv_data), 0);
                            
                            p1 = find_player_by_socket(plist,i);
                            g1 = find_match_by_socket(glist,i);
                            
                            file = fopen(g1.log ,"a");
                            if(file == NULL) {
                                printf ("Error(2) open log file to write!\n");
                                exit(1);
                            }
                            
                            strcpy(buff,p1.name);
                            strcat(buff," : ");
                            strcat(buff,recv_data);
                            strcat(buff,"\n");
                            fputs(buff, file);
                            
                            fclose(file);
                            
                            soc1 = i;
                            soc2 = find_player_socket(glist,i);
                            
                            g1 = find_match_by_socket(glist,i);
                            if (strcmp(g1.log,"")!=0) {
                                file = fopen(g1.log,"r");
                                while(!feof(file)){
                                    fgets(recv_data,200,file);
                                    puts(recv_data);
                                    send(soc1,recv_data,strlen(recv_data),0);
                                    send(soc2,recv_data,strlen(recv_data),0);
                                }
                                
                                send(soc1,"ENDFILE",7,0);
                                send(soc2,"ENDFILE",7,0);
                                
                                fclose(file);
                            }
                        }
                        else
                        {
                            printf("recv_data = %s\n",recv_data);
                            send(find_player_socket(glist,i),recv_data,strlen(recv_data),0);
                            p1 = find_player_by_socket(plist,i);
                            g1 = find_match_by_socket(glist,i);
                            file = fopen(g1.log,"a");
                            if(file == NULL){
                                printf ("Error(3) open log file to write!\n");
                                exit(1);
                            }
                            
                            strcpy(buff,p1.name);
                            strcat(buff," : ");
                            strcat(buff,recv_data);
                            strcat(buff,"\n");
                            fputs(buff,file);
                            
                            fclose(file);
                        }
                    }
                    
                }
            }
        }
    }
    
    return 0;
}
