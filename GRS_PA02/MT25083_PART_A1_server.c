#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>

#define FC 8
int FS, PORT;

typedef struct {
    char *f[FC];
} Msg;

Msg mkmsg()
{
    Msg m;
    for(int i=0;i<FC;i++){
        m.f[i]=malloc(FS);
        memset(m.f[i],'A'+i,FS-1);
        m.f[i][FS-1]='\0';
    }
    return m;
}

void freemsg(Msg *m)
{
    for(int i=0;i<FC;i++)
        free(m->f[i]);
}

void sendb(int fd, Msg *m)
{
    for(int i=0;i<FC;i++){
        int len=strlen(m->f[i])+1;
        int nlen=htonl(len);

        send(fd,&nlen,sizeof(nlen),0);
        send(fd,m->f[i],len,0);
    }
}

void *thr(void *arg)
{
    int cfd=*(int*)arg;
    free(arg);

    while(1){
        Msg m=mkmsg();
        sendb(cfd,&m);
        freemsg(&m);
    }

    close(cfd);
    return NULL;
}

int main(int argc, char *argv[])
{
    if(argc < 3){
        printf("Usage: %s <port> <msgsize>\n", argv[0]);
        return 0;
    }

    PORT = atoi(argv[1]);
    FS = atoi(argv[2]);

    signal(SIGPIPE, SIG_IGN);

    int sfd=socket(AF_INET,SOCK_STREAM,0);

    int opt=1;
    setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(PORT);
    addr.sin_addr.s_addr=INADDR_ANY;

    if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0){
        perror("bind");
        exit(1);
    }

    listen(sfd,20);

    while(1){
        int *cfd=malloc(sizeof(int));
        *cfd=accept(sfd,NULL,NULL);

        if(*cfd<0){
            free(cfd);
            continue;
        }

        pthread_t t;
        pthread_create(&t,NULL,thr,cfd);
        pthread_detach(t);
    }
}
