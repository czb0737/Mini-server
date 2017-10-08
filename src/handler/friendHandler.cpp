#include "../../include/handler/friendHandler.h"

#define getFriends 1
#define addFriend 2
#define deleteFriend 3
#define searchFriend 4

char *transfer_string_to_char(string s)
{
    char *c = new char[s.length() + 1];
    s.copy(c, s.length(), 0);
    c[s.length()] = '\0';
    return c;
}

string friendHandle(string &sql)
{
    int bytes = 0;
    cJSON *json;
    cJSON *json2;
    int size = sql.size();
    char *buffer = new char[size + 1];
    copy(sql.begin(), sql.end(), buffer);
    buffer[size] = '\0';
    json = cJSON_Parse(buffer);

    //验证用户token
    int sock = connect_to_server(port_low);
    string str = (string)"select token from User where userName='" + cJSON_GetObjectItem(json, "userName")->valuestring + "'";
    char *res = transfer_string_to_char(str);
    str.clear();

    if(-1 == write(sock, res, 1024))
    {
        cout << "Fail to write to storage!" << endl;
    }

    delete[] res;
    res = NULL;
    res = new char[1024];

    if(-1 == (bytes = read(sock, res, 1024)))
    {
        cout << "Fail to read from storage!" << endl;
    }
    close(sock);
    res[bytes] = '\0';

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

    if(getFriends == operating)
    {
        sock = connect_to_server(port_low);
        str = (string)"select userName1,userName2 from Friend where (userName1='" + cJSON_GetObjectItem(json, "userName")->valuestring + "' or userName2='" + cJSON_GetObjectItem(json, "userName")->valuestring + "') and confirm1=1 and confirm2=1";
        res = transfer_string_to_char(str);
        str.clear();

        if(-1 == write(sock, res, 1024))
        {
            cout << "Fail to write to storage!" << endl;
        }

        delete[] res;
        res = NULL;
        res = new char[1024];

        if(-1 == (bytes = read(sock, res, 1024)))
        {
            cout << "Fail to read from storage!" << endl;
        }
        close(sock);
        res[bytes] = '\0';

        json2 = cJSON_Parse(res);

        delete[] res;   //指针res本次生命周期到此结束
        res = NULL;

        if(cJSON_GetObjectItem(json2, "result")->valueint)
        {
            cJSON_AddStringToObject(json2, "extra", "Fail to get your friends!");
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
            cJSON_AddStringToObject(tmpjs, "extra", "You have no friend!");
            char *ret = cJSON_Print(tmpjs);
            cJSON_Delete(tmpjs);
            return ret;
        }
        else
        {
            cJSON_AddStringToObject(json2, "extra", "Here are your friends!");
        }

    }
    else if(addFriend == operating)
    {
        sock = connect_to_server(port_low);
        str = (string)"select * from Friend where (userName1='" + cJSON_GetObjectItem(json, "userName")->valuestring + "' or userName2='" + cJSON_GetObjectItem(json, "userName")->valuestring + "') and (userName1='" + cJSON_GetObjectItem(json, "userName2")->valuestring + "' or userName2='" + cJSON_GetObjectItem(json, "userName2")->valuestring + "')";
        res = transfer_string_to_char(str);
        str.clear();

        if(-1 == write(sock, res, 1024))
        {
            cout << "Fail to write to storage!" << endl;
        }

        delete[] res;
        res = NULL;
        res = new char[1024];

        if(-1 == (bytes = read(sock, res, 1024)))
        {
            cout << "Fail to read from storage!" << endl;
        }
        close(sock);
        res[bytes] = '\0';

        json2 = cJSON_Parse(res);

        delete[] res;   //指针res本次生命周期到此结束
        res = NULL;

        if(cJSON_GetObjectItem(json2, "result")->valueint)
        {
            cout << "Fail to run select!" << endl;
            cJSON_AddStringToObject(json2, "extra", "Fail to search for your friendship!");
            char *ret = cJSON_Print(json2);
            cJSON_Delete(json2);
            return ret;
        }

        count = cJSON_GetObjectItem(json2, "count")->valueint;

        if(0 == count)
        {
            sock = connect_to_server(port_low);
            str = (string)"insert into Friend values('" + cJSON_GetObjectItem(json, "userName")->valuestring + "','" + cJSON_GetObjectItem(json, "userName2")->valuestring + "',1,0)";
            res = transfer_string_to_char(str);
            str.clear();

            if(-1 == write(sock, res, 1024))
            {
                cout << "Fail to write to storage!" << endl;
            }

            delete[] res;
            res = NULL;
            res = new char[1024];

            if(-1 == (bytes = read(sock, res, 1024)))
            {
                cout << "Fail to read from storage!" << endl;
            }
            close(sock);
            res[bytes] = '\0';

            cJSON_Delete(json2);
            json2 = cJSON_Parse(res);

            delete[] res;   //指针res本次生命周期到此结束
            res = NULL;

            if(cJSON_GetObjectItem(json2, "result")->valueint)
            {
                cJSON_AddStringToObject(json2, "extra", "Fail to add your friend!");
                char *ret = cJSON_Print(json2);
                cJSON_Delete(json2);
                return ret;
            }

            cJSON_AddStringToObject(json2, "extra", "Add friend succeed. Please wait for his/her reply!");

        }
        else if(1 == count)
        {
            cJSON *tmpjs2 = json2;
            json2 = cJSON_GetObjectItem(tmpjs2, "info")->child;
            int confirm1 = cJSON_GetObjectItem(json2, "confirm1")->valuestring[0] - '0';
            int confirm2 = cJSON_GetObjectItem(json2, "confirm2")->valuestring[0] - '0';

            if(1 == confirm1 && 1 == confirm2)
            {
                cJSON_Delete(tmpjs2);
                //cJSON_Delete(json2);

                cJSON *tmpjs;
                tmpjs = cJSON_CreateObject();
                cJSON_AddNumberToObject(tmpjs, "result", 1);
                cJSON_AddStringToObject(tmpjs, "extra", "He/She is already your friend!");
                char *ret = cJSON_Print(tmpjs);
                cJSON_Delete(tmpjs);
                return ret;
            }
            if((1 == confirm1 && 0 == strcmp(cJSON_GetObjectItem(json, "userName")->valuestring, cJSON_GetObjectItem(json2, "userName1")->valuestring)) || (1 == confirm2 && 0 == strcmp(cJSON_GetObjectItem(json, "userName")->valuestring, cJSON_GetObjectItem(json2, "userName2")->valuestring)))
            {
                cJSON_Delete(tmpjs2);
                //cJSON_Delete(json2);

                cJSON *tmpjs;
                tmpjs = cJSON_CreateObject();
                cJSON_AddNumberToObject(tmpjs, "result", 1);
                cJSON_AddStringToObject(tmpjs, "extra", "You have already add he/she. Please wait for his/her reply!");
                char *ret = cJSON_Print(tmpjs);
                cJSON_Delete(tmpjs);
                return ret;
            }

            sock = connect_to_server(port_low);
            str = (string)"update Friend set confirm1=1,confirm2=1 where userName1='" + cJSON_GetObjectItem(json2, "userName1")->valuestring + "' and userName2='" + cJSON_GetObjectItem(json2, "userName2")->valuestring + "'";
            res = transfer_string_to_char(str);
            str.clear();

            if(-1 == write(sock, res, 1024))
            {
                cout << "Fail to write to storage!" << endl;
            }

            delete[] res;
            res = NULL;
            res = new char[1024];

            if(-1 == (bytes = read(sock, res, 1024)))
            {
                cout << "Fail to read from storage!" << endl;
            }
            close(sock);
            res[bytes] = '\0';

            json2 = cJSON_Parse(res);

            delete[] res;   //指针res本次生命周期到此结束
            res = NULL;

            if(1 == cJSON_GetObjectItem(json2, "result")->valueint)
            {
                cJSON_AddStringToObject(json2, "extra", "Error ocurred in server!");
                return cJSON_Print(json2);
            }

            cJSON_Delete(tmpjs2);
            cJSON_AddStringToObject(json2, "extra", "Add friend succeed!");

        }
        else
        {
            cJSON_Delete(json2);
            cJSON *tmpjs;
            tmpjs = cJSON_CreateObject();
            cJSON_AddNumberToObject(tmpjs, "result", 1);
            cJSON_AddStringToObject(tmpjs, "extra", "Error ocurred in server!");
            return cJSON_Print(tmpjs);
        }
    }
    else if(deleteFriend == operating)
    {
        sock = connect_to_server(port_low);
        str = (string)"select * from Friend where (userName1='" + cJSON_GetObjectItem(json, "userName")->valuestring + "' or userName2='" + cJSON_GetObjectItem(json, "userName")->valuestring + "') and (userName1='" + cJSON_GetObjectItem(json, "userName2")->valuestring + "' or userName2='" + cJSON_GetObjectItem(json, "userName2")->valuestring + "')";
        res = transfer_string_to_char(str);
        str.clear();

        if(-1 == write(sock, res, 1024))
        {
            cout << "Fail to write to storage!" << endl;
        }

        delete[] res;
        res = NULL;
        res = new char[1024];

        if(-1 == (bytes = read(sock, res, 1024)))
        {
            cout << "Fail to read from storage!" << endl;
        }
        close(sock);
        res[bytes] = '\0';

        json2 = cJSON_Parse(res);

        delete[] res;   //指针res本次生命周期到此结束
        res = NULL;

        if(cJSON_GetObjectItem(json2, "result")->valueint)
        {
            cout << "Fail to run select!" << endl;
            cJSON_AddStringToObject(json2, "extra", "Fail to search for your friendship!");
            char *ret = cJSON_Print(json2);
            cJSON_Delete(json2);
            return ret;
        }

        count = cJSON_GetObjectItem(json2, "count")->valueint;

        if(0 == count)
        {
            cJSON_AddStringToObject(json2, "extra", "He is not your friend!");
        }
        else if(1 == count)
        {
            cJSON *tmpjs2 = json2;
            json2 = cJSON_GetObjectItem(tmpjs2, "info")->child;

            sock = connect_to_server(port_low);
            str = (string)"delete from Friend where (userName1='" + cJSON_GetObjectItem(json2, "userName1")->valuestring + "' and userName2='" + cJSON_GetObjectItem(json2, "userName2")->valuestring + "') or (userName1='" + cJSON_GetObjectItem(json2, "userName2")->valuestring + "' and userName2='" + cJSON_GetObjectItem(json2, "userName1")->valuestring + "')";
            res = transfer_string_to_char(str);
            str.clear();

            if(-1 == write(sock, res, 1024))
            {
                cout << "Fail to write to storage!" << endl;
            }

            delete[] res;
            res = NULL;
            res = new char[1024];

            if(-1 == (bytes = read(sock, res, 1024)))
            {
                cout << "Fail to read from storage!" << endl;
            }
            close(sock);
            res[bytes] = '\0';

            delete[] tmpjs2;
            json2 = cJSON_Parse(res);

            delete[] res;   //指针res本次生命周期到此结束
            res = NULL;

            if(1 == cJSON_GetObjectItem(json2, "result")->valueint)
            {
                cJSON_AddStringToObject(json2, "extra", "Error ocurred in server!");
                return cJSON_Print(json2);
            }

            cJSON_AddStringToObject(json2, "extra", "Delete friend succeed!");

        }
        else
        {
            cJSON_Delete(json2);
            cJSON *tmpjs;
            tmpjs = cJSON_CreateObject();
            cJSON_AddNumberToObject(tmpjs, "result", 1);
            cJSON_AddStringToObject(tmpjs, "extra", "Error ocurred in server!");
            return cJSON_Print(tmpjs);
        }
    }
    else if(searchFriend == operating)
    {
        sock = connect_to_server(port_low);
        str = (string)"select userName from User where userName like '%" + cJSON_GetObjectItem(json, "userName2")->valuestring + "%'";
        res = transfer_string_to_char(str);
        str.clear();

        if(-1 == write(sock, res, 1024))
        {
            cout << "Fail to write to storage!" << endl;
        }

        delete[] res;
        res = NULL;
        res = new char[1024];

        if(-1 == (bytes = read(sock, res, 1024)))
        {
            cout << "Fail to read from storage!" << endl;
        }
        close(sock);
        res[bytes] = '\0';

        json2 = cJSON_Parse(res);

        delete[] res;   //指针res本次生命周期到此结束
        res = NULL;

        if(cJSON_GetObjectItem(json2, "result")->valueint)
        {
            cout << "Fail to run select!" << endl;
            cJSON_AddStringToObject(json2, "extra", "Fail to search for your friends!");
            char *ret = cJSON_Print(json2);
            cJSON_Delete(json2);
            return ret;
        }

        count = cJSON_GetObjectItem(json2, "count")->valueint;
        char c[10];
        sprintf(c, "%d", count);
        str = (string)"There are " + c + " users found!";
        res = transfer_string_to_char(str);
        str.clear();

        cJSON_AddStringToObject(json2, "extra", res);

        delete[] res;
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


















