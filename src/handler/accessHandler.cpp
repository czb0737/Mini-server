#include "../../include/handler/accessHandler.h"

string accessHandle(string &sql)
{
    cJSON *json;
    int size = sql.size();
    char *buffer = new char[size + 1];
    copy(sql.begin(), sql.end(), buffer);
    buffer[size] = '\0';
    json = cJSON_Parse(buffer);

    int num = cJSON_GetObjectItem(json, "type")->valueint;
    int port_low = 0;
    int recbytes = 0;

    switch(num)
    {
        case 1:
            port_low = USER_PORT;
            break;

        case 2:
            port_low = FRIEND_PORT;
            break;

        case 3:
            port_low = MESSAGE_PORT;
            break;

        default:
            port_low = MESSAGE_PORT;
    }

    int sock = connect_to_server(port_low);

    if(-1 == write(sock, buffer, 1024))
    {
        cerr << "Fail to write to client!" << endl;
    }

    char buf[1025];

    if(-1 == (recbytes = read(sock,buf,1024)))
    {
        cerr << "Fail to read from client!" << endl;
    }
    buf[recbytes]='\0';

    return string(buf);
}
