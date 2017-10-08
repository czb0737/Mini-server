#include "../../include/handler/messageHandler.h"

#define getMsg 1
#define sendMsg 2

char *transfer_string_to_char(string s)
{
    char *c = new char[s.length() + 1];
    s.copy(c, s.length(), 0);
    c[s.length()] = '\0';
    return c;
}

string messageHandler(string &sql)
{
    sock = connect_to_server(port_low);

    int bytes = 0;
    cJSON *json;
    cJSON *json2;
    int size = sql.size();
    char *buffer = new char[size + 1];
    copy(sql.begin(), sql.end(), buffer);
    buffer[size] = '\0';
    json = cJSON_Parse(buffer);

    //验证用户token
    string str = (string)"select token from User where userName='" + cJSON_GetObjectItem(json, "userName")->valuestring + "'";
    char *res = transfer_string_to_char(str);
    str.clear();

    if(-1 == write(sock, res, 1024))
    {
        cerr << "Fail to write to storage!$" << endl;
        cerr << strerror(errno) << endl;
    }

    delete[] res;
    res = NULL;
    res = new char[1024];

    if(-1 == (bytes = read(sock, res, 1024)))
    {
        cerr << "Fail to read from storage!" << endl;
    }
    res[bytes] = '\0';
    // cout << "Response: " << res << endl;
    json2 = cJSON_Parse(res);

    delete[] res;   //指针res本次生命周期到此结束
    res = NULL;

    if(cJSON_GetObjectItem(json2, "result")->valueint)
    {
        cJSON_AddStringToObject(json2, "extra", "Fail to search for userName!");
        char *ret = cJSON_Print(json2);
        cJSON_Delete(json2);
        return ret;
    }

    int count = cJSON_GetObjectItem(json2, "count")->valueint;

    if(0 == count)
    {
        cJSON *tmpjs;
        tmpjs = cJSON_CreateObject();
        cJSON_AddNumberToObject(tmpjs, "result", 1);
        cJSON_AddStringToObject(tmpjs, "extra", "User does not exist!");
        char *ret = cJSON_Print(tmpjs);
        cJSON_Delete(tmpjs);
        return ret;
    }

    if(0 != strcmp(cJSON_GetObjectItem(json, "token")->valuestring, cJSON_GetObjectItem(cJSON_GetObjectItem(json2, "info")->child, "token")->valuestring))
    {
        cJSON *tmpjs;
        tmpjs = cJSON_CreateObject();
        cJSON_AddNumberToObject(tmpjs, "result", 1);
        cJSON_AddStringToObject(tmpjs, "extra", "Fail to verify user!");
        char *ret = cJSON_Print(tmpjs);
        cJSON_Delete(tmpjs);
        return ret;
    }

    cJSON_Delete(json2);

    //信息验证成功，开始处理请求
    int operating = cJSON_GetObjectItem(json, "operating")->valueint;

    if(getMsg == operating)
    {
        str = (string)"select * from Message where toUser='" + cJSON_GetObjectItem(json, "userName")->valuestring + "'";
        res = transfer_string_to_char(str);
        str.clear();

        if(-1 == write(sock, res, 1024))
        {
            cout << "Fail to write to storage!#" << endl;
            cerr << strerror(errno) << endl;
        }

        delete[] res;
        res = NULL;
        res = new char[1024];

        if(-1 == (bytes = read(sock, res, 1024)))
        {
            cout << "Fail to read from storage!" << endl;
        }
        res[bytes] = '\0';

        json2 = cJSON_Parse(res);

        delete[] res;   //指针res本次生命周期到此结束
        res = NULL;

        if(cJSON_GetObjectItem(json2, "result")->valueint)
        {
            cJSON_AddStringToObject(json2, "extra", "Fail to get your messages!");
            char *ret = cJSON_Print(json2);
            cJSON_Delete(json2);
            return ret;
        }

        count = cJSON_GetObjectItem(json2, "count")->valueint;

        if(0 == count)
        {
            cJSON_Delete(json2);

            cJSON *tmpjs;
            tmpjs = cJSON_CreateObject();
            cJSON_AddNumberToObject(tmpjs, "result", 1);
            cJSON_AddStringToObject(tmpjs, "extra", "You have no new message!");
            char *ret = cJSON_Print(tmpjs);
            cJSON_Delete(tmpjs);
            return ret;
        }
        else
        {
            cJSON_AddStringToObject(json2, "extra", "You have new messages!");
        }

    }
    else if(sendMsg == operating)
    {
        str = (string)"insert into Message(fromUser,toUser,message) values('" + cJSON_GetObjectItem(json, "userName")->valuestring + "','" + cJSON_GetObjectItem(json, "userName2")->valuestring + "','" + cJSON_GetObjectItem(json, "message")->valuestring + "')";
        res = transfer_string_to_char(str);
        str.clear();
        // cout << "After verification: " << res << endl;
        if(-1 == write(sock, res, 1024))
        {
            cout << "Fail to write to storage!@" << endl;
            cerr << strerror(errno) << endl;
        }
        // cout << "Write success!" << endl;
        delete[] res;
        res = NULL;
        res = new char[1024];

        if(-1 == (bytes = read(sock, res, 1024)))
        {
            cout << "Fail to read from storage!" << endl;
        }
        res[bytes] = '\0';
        // cout << "Response 2: " << res << endl;
        json2 = cJSON_Parse(res);

        delete[] res;   //指针res本次生命周期到此结束
        res = NULL;

        if(cJSON_GetObjectItem(json2, "result")->valueint)
        {
            cout << "Fail to run select!" << endl;
            cJSON_AddStringToObject(json2, "extra", "Fail to send your message your friend!");
            char *ret = cJSON_Print(json2);
            cJSON_Delete(json2);
            return ret;
        }

        cJSON_AddStringToObject(json2, "extra", "Send message succeed!");
    }
    else
    {
        json2 = cJSON_CreateObject();
        cJSON_AddNumberToObject(json2, "result", 1);
        cJSON_AddStringToObject(json2, "extra", "Wrong operating!");
    }
    char *ret = cJSON_Print(json2);
    cJSON_Delete(json2);
    return string(ret);

}

// bool deleteMsg(char *buf, int sock)
// {
//     int bytes = 0;
//     cJSON *json;
//     cJSON *json2;
//     json = cJSON_Parse(buf);
//     cJSON *tmpjs = cJSON_GetObjectItem(json, "info")->child;
//     //int arrSize = cJSON_GetArraySize(tmpjs);

//     while(tmpjs != NULL)
//     {
//         //cJSON *tmpjs2 = cJSON_GetArrayItem(tmpjs, i);
//         string str = (string)"delete from Message where nowTime='" + cJSON_GetObjectItem(tmpjs, "nowTime")->valuestring + "'";
//         char *res = transfer_string_to_char(str);
//         str.clear();

//         tmpjs = tmpjs->next;

//         if(-1 == write(sock, res, 1024))
//         {
//             cout << "Fail to write to storage!" << endl;
//         }

//         delete[] res;
//         res = NULL;
//         res = new char[1024];

//         if(-1 == (bytes = read(sock, res, 1024)))
//         {
//             cout << "Fail to read from storage!" << endl;
//         }
//         res[bytes] = '\0';

//         json2 = cJSON_Parse(res);

//         delete[] res;   //指针res本次生命周期到此结束
//         res = NULL;

//         if(cJSON_GetObjectItem(json2, "result")->valueint)
//         {
//             cJSON_Delete(json2);
//             return false;
//         }

//         cJSON_Delete(json2);
//     }
//     cJSON_Delete(json);
//     return true;
// }
/*
int main()
{
    cJSON *json;
    json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "operating", 2);
    cJSON_AddStringToObject(json, "userName", "czb4");
    cJSON_AddStringToObject(json, "password", "czb4");
    char *buf = user_handle(cJSON_Print(json));
    cout << buf << endl;
}

*/


















