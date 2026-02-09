#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#define FC 8

volatile int stop = 0;
int duration;
char *ip;
int port;

long long total_bytes = 0;
long long total_msgs = 0;

/* receive exact bytes */
ssize_t recvall(int fd, void *buf, size_t len)
{
    size_t got = 0;
    while(got < len){
        ssize_t n = recv(fd, (char*)buf + got, len - got, 0);
        if(n <= 0) return n;
        got += n;
    }
    return got;
}

/* timer thread */
void *timer_fn(void *arg)
{
    sleep(duration);
    stop = 1;
    return NULL;
}

/* worker thread */
void *worker(void *arg)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    serv.sin_addr.s_addr = inet_addr(ip);

    if(connect(sock, (struct sockaddr*)&serv, sizeof(serv)) < 0){
        perror("connect");
        return NULL;
    }

    while(!stop){

        for(int i=0;i<FC;i++){
            int nlen;

            if(recvall(sock, &nlen, sizeof(nlen)) <= 0)
                goto end;

            int len = ntohl(nlen);

            char *buf = malloc(len);
            if(recvall(sock, buf, len) <= 0){
                free(buf);
                goto end;
            }

            __sync_fetch_and_add(&total_bytes, len);
            free(buf);
        }

        __sync_fetch_and_add(&total_msgs, 1);
    }

end:
    close(sock);
    return NULL;
}

int main(int argc, char *argv[])
{
    if(argc != 6){
        printf("Usage: %s <ip> <port> <threads> <msgsize> <time>\n", argv[0]);
        return 0;
    }

    ip = argv[1];
    port = atoi(argv[2]);
    int threads = atoi(argv[3]);
    duration = atoi(argv[5]);

    pthread_t workers[threads];
    pthread_t timer;

    /* start global timer */
    pthread_create(&timer, NULL, timer_fn, NULL);

    /* start workers */
    for(int i=0;i<threads;i++)
        pthread_create(&workers[i], NULL, worker, NULL);

    /* wait workers */
    for(int i=0;i<threads;i++)
        pthread_join(workers[i], NULL);

    pthread_join(timer, NULL);

    double total_time = duration;

    double throughput = (total_bytes * 8.0) / (total_time * 1e9);
    double latency = (total_time / total_msgs) * 1e6;

    printf("BYTES=%lld\n", total_bytes);
    printf("TIME=%.3f\n", total_time);
    printf("THROUGHPUT=%.6f\n", throughput);
    printf("LATENCY=%.3f\n", latency);

    return 0;
}
