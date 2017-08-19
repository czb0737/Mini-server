#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<sys/select.h>

int main(void)
{
    int s, r, ret, len, error;
    char buf[20];
    struct sockaddr_in sock;
    fd_set rset,wset;
    struct timeval tv;
    tv.tv_sec=10;
    s=socket(AF_INET,SOCK_STREAM,0);
    if(-1==s)
    {
        printf("socket() failed/n"); return 1;
    }

    if(0>fcntl(s,F_SETFL,fcntl(s,F_GETFL,0)|/*O_NDELAY*/O_NONBLOCK))
    {
        printf("fcntl failed/n"); return 1;
    }

    sock.sin_family=PF_INET;
    sock.sin_port  =htons(10080);
    sock.sin_addr.s_addr=inet_addr("197.0.0.9");        /*htons(INADDR_ANY);,if a none exist ip, timeout takes effect*/

    if(-1==(ret=connect(s,(struct sockaddr*)&sock,sizeof(struct sockaddr_in))))
    {
        if(EINPROGRESS!=errno){/*if not fcntl->O_NDELAY, fall here*/
        printf("connect() failed:%s/n",strerror(errno)); return 1; }
    }
    if(ret==0)
    {
        printf("connection ok at once/n"); close(s); return 0;
    }
    FD_ZERO(&rset);
    FD_SET(s,&rset);
    wset=rset;
    if(-1==(ret=select(s+1,&rset,&wset,NULL,&tv)))
    {
        close(s);
        printf("select error/n");
        return 1;
    }
    if(0==ret)
    {
        close(s);
        printf("timeout/n");
        return 1;
    }
    if(FD_ISSET(s,&rset)||FD_ISSET(s,&rset))
    {
        len=sizeof(error);
        errno=0;
        if(0>(ret=getsockopt(s,SOL_SOCKET,SO_ERROR,(void*)&error,&len)))
        {
        printf("getsockopt error:%s/n",strerror(error));
        close(s);
        return 1;
        }
        if(error!=0)
        {
            printf("getsockopt set error:%s/n",strerror(error)); close(s);
            return 1;
        }
        printf("getsockopt success/n");
    }
    else
    {
        close(s);
        printf("FD_ISSET error/n");
        return 1;
    }
    if(-1==(ret=read(s,buf,sizeof(buf))))
    {
        printf("read() failed/n");
        return 1;
    }
    printf("ret=%d/n",ret);
    buf[ret-1]='/0';
    printf("read:%s/n",buf);
    close(s);
    return 0;
}










