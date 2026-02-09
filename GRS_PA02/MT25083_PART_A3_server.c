#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <linux/errqueue.h>
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
        m.f[i] = malloc(FS);
        memset(m.f[i], 'A'+i, FS-1);
        m.f[i][FS-1] = '\0';
    }
    return m;
}

void freemsg(Msg *m)
{
    for(int i=0;i<FC;i++)
        free(m->f[i]);
}

void waitzc(int fd)
{
    struct msghdr msg;
    struct iovec iov;
    char d;
    char cbuf[512];

    memset(&msg,0,sizeof(msg));

    iov.iov_base=&d;
    iov.iov_len=sizeof(d);

    msg.msg_iov=&iov;
    msg.msg_iovlen=1;
    msg.msg_control=cbuf;
    msg.msg_controllen=sizeof(cbuf);

    /* wait until at least one completion arrives */
    recvmsg(fd,&msg,MSG_ERRQUEUE);

    /* clear remaining completions */
    while(recvmsg(fd,&msg,MSG_ERRQUEUE|MSG_DONTWAIT)>=0);
}

void sendzc(int fd, Msg *m)
{
    struct iovec iov[FC*2];

    for(int i=0;i<FC;i++){
        int len=strlen(m->f[i])+1;
        int nlen=htonl(len);

        int *lb=malloc(sizeof(int));
        *lb=nlen;

        iov[2*i].iov_base=lb;
        iov[2*i].iov_len=sizeof(int);

        iov[2*i+1].iov_base=m->f[i];
        iov[2*i+1].iov_len=len;
    }

    struct msghdr msg;
    memset(&msg,0,sizeof(msg));
    msg.msg_iov=iov;
    msg.msg_iovlen=FC*2;

    sendmsg(fd,&msg,MSG_ZEROCOPY);
}

void *thr(void *arg)
{
    int cfd=*(int*)arg;
    free(arg);

    int one=1;
    setsockopt(cfd,SOL_SOCKET,SO_ZEROCOPY,&one,sizeof(one));

    while(1){
        Msg m=mkmsg();
        sendzc(cfd,&m);
        waitzc(cfd);
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

