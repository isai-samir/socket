#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>

int main(int argc, char *argv[]){
    int sock, length, n, resp;
    socklen_t tamCliente;
    struct sockaddr_in servidor;
    struct sockaddr_in cliente;
    char buffer[200],auxNombre[100],auxEdad[100],auxGenero[100],menu;
    if (argc < 2) {
       fprintf(stderr, "ERROR, no port provided\n");
       exit(0);
    }
    sock=socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) perror("Opening socket");
    length = sizeof(servidor);
    bzero(&servidor,length);
    servidor.sin_family=AF_INET;
    servidor.sin_addr.s_addr=INADDR_ANY;
    servidor.sin_port=htons(atoi(argv[1]));
    if (bind(sock,(struct sockaddr *)&servidor,length)<0) 
        perror("binding");
    tamCliente = sizeof(struct sockaddr_in);
    printf("Esperando cliente\n");
    while (1) {
        n = recvfrom(sock,buffer,200,0,(struct sockaddr *)&cliente,&tamCliente);
        if (n < 0) perror("recvfrom");
        write(1,"Recibi del cliente: @",21);
        write(1,"\nEnvie al cliente mensaje de bienvenida\n",40);
        n = sendto(sock,"Bienvenido al servidor",22,0,(struct sockaddr *)&cliente,tamCliente);
        if (n  < 0) perror("sendto");
        do{
            n = recvfrom(sock,buffer,200,0,(struct sockaddr *)&cliente,&tamCliente);
            menu = buffer[0];
            switch(menu){
                case 'a':
                    bzero(auxNombre,100);
                    n = recvfrom(sock,auxNombre,100,0,(struct sockaddr *)&cliente,&tamCliente);
                    write(1,"Recibido de cliente: ",21);
                    write(1,auxNombre,strlen(auxNombre));
                    break;
                case 'b':
                    bzero(auxEdad,100);
                    n = recvfrom(sock,auxEdad,100,0,(struct sockaddr *)&cliente,&tamCliente);
                    write(1,"Recibido de cliente: ",21);
                    write(1,auxEdad,strlen(auxEdad));
                    break;
                case 'c':
                    bzero(auxGenero,100);
                    n = recvfrom(sock,auxGenero,100,0,(struct sockaddr *)&cliente,&tamCliente);
                    write(1,"Recibido de cliente: ",21);
                    write(1,auxGenero,strlen(auxGenero));
                    break;
                case 'd':
                    bzero(buffer,200);
                    strcat(buffer,auxNombre);
                    strcat(buffer,auxEdad);
                    strcat(buffer,auxGenero);
                    write(1,"Enviando datos al cliente: ",27);
                    write(1,buffer,strlen(buffer));
                    n = sendto(sock,buffer,strlen(buffer),0,(struct sockaddr *)&cliente,tamCliente);
                    break;
            }
        }while(menu != 'e');
        bzero(buffer,200);
        strcat(buffer,auxNombre);
        strcat(buffer,auxEdad);
        strcat(buffer,auxGenero);
        write(1,"Enviando datos al cliente: ",27);
        write(1,buffer,strlen(buffer));
        n = sendto(sock,buffer,strlen(buffer),0,(struct sockaddr *)&cliente,tamCliente);
        bzero(buffer,200);
        n = recvfrom(sock,buffer,200,0,(struct sockaddr *)&cliente,&tamCliente);
        write(1,"Recibido de cliente: ",21);
        write(1,buffer,strlen(buffer));
        write(1,"\n",1);
        write(1,"Enviar al cliente Adios\n",24);
        n = sendto(sock,"Adios",5,0,(struct sockaddr *)&cliente,tamCliente);
    }
    return 0;
 }