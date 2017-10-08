#include <mysql/mysql.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "cJSON.c"

using namespace std;

char *get_data_from_db(char sql[])
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char server[] = "localhost";
    char user[] = "root";
    char password[] = "czb";
    char database[] = "chat";
    int result = 0; //sql语句运行成败
    char *ret;

    cJSON *json;
    json = cJSON_CreateObject();

    conn = mysql_init(NULL);

    /* Connect to database */
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    /* send SQL query */
    if (result = mysql_query(conn, sql))
    {
        cerr << "Fail to read it!" << endl;
        cJSON_AddNumberToObject(json, "result", result);
        ret = cJSON_Print(json);
        return ret;
    }

    cJSON_AddNumberToObject(json, "result", result);
    res = mysql_use_result(conn);

    if(NULL != res)
    {
        int num_fields = mysql_num_fields(res);

        char *name[100];
        int k = 0;
        MYSQL_FIELD *field;

        while ((field = mysql_fetch_field(res)))
        {
            name[k] = field->name;
            ++k;
        }

        /*构造数组存储数据*/
        cJSON_AddItemToObject(json, "info", cJSON_CreateArray());

        while ((row = mysql_fetch_row(res)) != NULL)
        {
            cJSON *tmpjs;
            tmpjs = cJSON_CreateObject();
            for(int i = 0; i < num_fields; ++i)
            {
                cJSON_AddStringToObject(tmpjs, name[i], row[i]);
            }
            cJSON_AddItemToArray(cJSON_GetObjectItem(json, "info"), tmpjs);
        }

        cJSON_AddNumberToObject(json, "count", res->row_count);

    }

    /* close connection */
    mysql_free_result(res);
    mysql_close(conn);
    /* Use any MySQL API functions here */
    mysql_server_end();

    ret = cJSON_Print(json);
    return ret;
}

/*
int main()
{
    //char sql[] = "insert into User values('czb4','czb4','')";
    char sql[] = "delete from User where userName='czb4'";
    //char sql[] = "select * from User";

    char *s;
    cout << (s = get_data_from_db(sql)) << endl;
/*    cJSON *json;
    json = cJSON_Parse(sql);
    json = cJSON_GetObjectItem(json, "info");
    json = json->child;
    while(json != NULL)
    {
        cout << cJSON_GetObjectItem(json, "userName")->valuestring << "\t" << cJSON_GetObjectItem(json, "password")->valuestring << endl;
        json = json->next;
    }
    //cout << cJSON_Print(json) << endl;
    free(s);
}*/




























