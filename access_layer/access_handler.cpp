#include "connect.cpp"
#include "cJSON.c"

char *access_handle(char *buffer)
{
    cJSON *json;
    json = cJSON_Parse(buffer);

    int num = cJSON_GetObjectItem(json, "type")->valueint;
    int port_low = 0;
    int recbytes = 0;

    switch(num)
    {
        case 1:
            port_low = user_port;
            break;

        case 2:
            port_low = friend_port;
            break;

        case 3:
            port_low = message_port;
            break;

        default:
            port_low = message_port;
    }

    int sock = connect_to_server(port_low);

    if(-1 == write(sock, buffer, 1024))
    {
        cout << "Fail to write to client!" << endl;
    }

    char *buf = new char[1024];

    if(-1 == (recbytes = read(sock,buf,1024)))
    {
        cout << "Fail to read from client!" << endl;
    }
    buffer[recbytes]='\0';

    return buf;
}
