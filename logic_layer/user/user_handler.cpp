#include <string.h>
#include <time.h>
#include "cJSON.c"
#include "md5.h"
#include "connect.cpp"


#define reg 1
#define login 2

char *transfer_string_to_char(string s)
{
    char *c = new char[s.length() + 1];
    s.copy(c, s.length(), 0);
    c[s.length()] = '\0';
    return c;
}

char *user_handle(char *buffer)
{
    int bytes = 0;
    cJSON *json;
    cJSON *json2;
    json = cJSON_Parse(buffer);

    int operating = cJSON_GetObjectItem(json, "operating")->valueint;

    if(reg == operating)
    {
        int sock = connect_to_server(port_low);
        string str = (string)"select userName from User where userName='" + cJSON_GetObjectItem(json, "userName")->valuestring + "'";
        cout << str << endl;
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

        cJSON_Delete(json2);

        if(0 != count)
        {
            cJSON *tmpjs;
            tmpjs = cJSON_CreateObject();
            cJSON_AddNumberToObject(tmpjs, "result", 1);
            cJSON_AddStringToObject(tmpjs, "extra", "Already exist!");
            char *ret = cJSON_Print(tmpjs);
            cJSON_Delete(tmpjs);
            return ret;
        }
        else
        {
            str = (string)"insert into User values('" + cJSON_GetObjectItem(json, "userName")->valuestring + "','" + cJSON_GetObjectItem(json, "password")->valuestring + "','')";
            res = transfer_string_to_char(str);
            str.clear();
            sock = connect_to_server(port_low);

            if(-1 == write(sock, res, 1024))
            {
                cout << "Fail to write to storage!(Insert)" << endl;
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

            int result = cJSON_GetObjectItem(json2, "result")->valueint;

            if(1 == result)
            {
                cJSON_AddStringToObject(json2, "extra", "Fail to insert user!");
                char *ret = cJSON_Print(json2);
                cJSON_Delete(json2);
                return ret;
            }

            cJSON_AddStringToObject(json2, "extra", "Register succeed!");

        }

    }
    else if(login == operating)
    {
        int sock = connect_to_server(port_low);
        string str = (string)"select userName,password from User where userName='" + cJSON_GetObjectItem(json, "userName")->valuestring + "'";
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
            cout << "Fail to run select!" << endl;
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
            cJSON_AddStringToObject(tmpjs, "extra", "User dose not exist!");
            return cJSON_Print(tmpjs);
        }
        else if(0 == strcmp(cJSON_GetObjectItem(json, "password")->valuestring, cJSON_GetObjectItem(cJSON_GetObjectItem(json2, "info")->child, "password")->valuestring))
        {
            time_t t;
            t = time(NULL);
            char *tmp = new char[11];
            sprintf(tmp, "%d", (int)t);
            tmp[10] = '\0';
            char *tmp2 = MDString(tmp);

            sock = connect_to_server(port_low);
            string str = (string)"update User set token='" + tmp2 + "' where userName='" + cJSON_GetObjectItem(json, "userName")->valuestring + "'";
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

            if(1 == cJSON_GetObjectItem(json2, "result")->valueint)
            {
                cJSON_AddStringToObject(json2, "extra", "Fail to get token!");
                char *ret = cJSON_Print(json2);
                cJSON_Delete(json2);
                return ret;
            }

            cJSON_AddStringToObject(json2, "token", tmp2);
            cJSON_AddStringToObject(json2, "extra", "Login succeed!");
            //delete[] tmp;
            //delete[] tmp2;
        }
        else
        {
            cJSON_Delete(json2);
            cJSON *tmpjs;
            tmpjs = cJSON_CreateObject();
            cJSON_AddNumberToObject(tmpjs, "result", 1);
            cJSON_AddStringToObject(tmpjs, "extra", "Password is wrong!");

            char *ret = cJSON_Print(tmpjs);
            cJSON_Delete(tmpjs);
            return ret;
        }
    }
    else
    {
        json2 = cJSON_CreateObject();
        cJSON_AddNumberToObject(json2, "result", 1);
        cJSON_AddStringToObject(json2, "extra", "Wrong operating!");
    }

    char *ret = cJSON_Print(json2);
    cJSON_Delete(json2);
    return ret;

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


















