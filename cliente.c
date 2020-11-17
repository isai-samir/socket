#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){
    int sock, send;
    unsigned int length;
    struct sockaddr_in server, from;
    struct hostent *hp;
    char buffer[200], menu,resp;
    if (argc != 3) { 
        printf("Usage: server port\n");
        exit(1);
    }
    sock= socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) perror("socket");
    server.sin_family = AF_INET;
    hp = gethostbyname(argv[1]);
    if (hp==0) perror("Unknown host");
    bcopy((char *)hp->h_addr, (char *)&server.sin_addr,hp->h_length);
    server.sin_port = htons(atoi(argv[2]));
    length=sizeof(struct sockaddr_in);
    //comunicacion con el servidor
    send = sendto(sock,"@",1,0,(const struct sockaddr *)&server,length);
    if (send < 0) perror("Sendto");
    bzero(buffer,200);
    send = recvfrom(sock,buffer,200,0,(struct sockaddr *)&from, &length);
    if (send < 0) perror("Sendto");
    printf("Mensaje recibido: %s \n",buffer);
    bzero(buffer,200);
    do{
        printf("a.- Nombre\nb.- Edad\nc.- Genero\nd.- Ver resultado ");
        fgets(buffer,200,stdin);
        menu = buffer[0];
        printf("<%c>",menu);
        bzero(buffer,200);
        switch (menu){
        case 'a':
            send = sendto(sock,"a",1,0,(const struct sockaddr *)&server,length);
            printf("Ingresa tu nombre ");
            fgets(buffer,200,stdin);
            printf("hay en buffer<%s>",buffer);
            send = sendto(sock,buffer,strlen(buffer),0,(const struct sockaddr *)&server,length);
            break;
        case 'b':
            send = sendto(sock,"b",1,0,(const struct sockaddr *)&server,length);
            printf("Ingresa tu edad ");
            fgets(buffer,200,stdin);
            send = sendto(sock,buffer,strlen(buffer),0,(const struct sockaddr *)&server,length);
            break;
        case 'c':
            send = sendto(sock,"c",1,0,(const struct sockaddr *)&server,length);
            printf("Ingresa tu genero ");
            fgets(buffer,200,stdin);
            send = sendto(sock,buffer,strlen(buffer),0,(const struct sockaddr *)&server,length);
            break;
        case 'd':
            send = sendto(sock,"d",1,0,(const struct sockaddr *)&server,length);
            send = recvfrom(sock,buffer,200,0,(struct sockaddr *)&from, &length);
            printf("Recibi el texto: %s\n",buffer);
            break;
        default:
            printf("Opcion incorrecta\n");
            break;
        }
        printf("Vas a ingresar otro dato\na.- Si\nb.- No ");
        fgets(buffer,200,stdin);
        menu = buffer[0];
        bzero(buffer,200);
    }while(menu != 'b');
    send = sendto(sock,"e",1,0,(const struct sockaddr *)&server,length);
    send = recvfrom(sock,buffer,200,0,(struct sockaddr *)&from, &length);
    printf("Recibi el texto: %s",buffer);
    bzero(buffer,200);
    printf("Se envio al servidor Salir\n");
    send = sendto(sock,"Salir\n",5,0,(const struct sockaddr *)&server,length);
    printf("Esperando palabra Adios\n");
    bzero(buffer,200);
    send = recvfrom(sock,buffer,200,0,(struct sockaddr *)&from, &length);
    printf("Recibido %s",buffer);
    /*send = sendto(sock,buffer,strlen(buffer),0,(const struct sockaddr *)&server,length);
    fgets(buffer,200,stdin);
    if (send < 0) perror("recvfrom");
    write(1,"Got an ack: ",12);
    write(1,buffer,send);
*/
    close(sock);
    return 0;
}