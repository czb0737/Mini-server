int set_server_socket(int port)
{
    struct sockaddr_in s_add,c_add;
    socklen_t sin_size = sizeof(struct sockaddr_in);
    int sfp;   //sfp为 server用于监听的socket

    printf("Hello,welcome to my server !\r\n");

    //建立socket
    sfp = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sfp)
    {
        printf("socket fail ! \r\n");
        return -1;
    }
    printf("socket ok !\r\n");

    bzero(&s_add,sizeof(struct sockaddr_in));
    s_add.sin_family=AF_INET;
    s_add.sin_addr.s_addr=htonl(INADDR_ANY);
    s_add.sin_port=htons(port);

    //绑定端口
    if(-1 == bind(sfp,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
    {
        printf("bind fail !\r\n");
        return -1;
    }
    printf("bind ok !\r\n");

    //监听socket
    if(-1 == listen(sfp, 10))
    {
        printf("listen fail !\r\n");
        return -1;
    }
    printf("listen ok\r\n");

    return sfp;

}
