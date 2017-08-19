#include <iostream>
#include <string.h>
#include "cJSON.c"

using namespace std;

int main()
{
    cJSON *tmp;
    tmp = cJSON_CreateObject();
 /*   string s;
    cin >> s;*/
    char c[100];
    cin >> c;
    cJSON_AddStringToObject(tmp, "name", c);
    cJSON_AddNumberToObject(tmp, "age", 22);
    cJSON_AddStringToObject(tmp, "gender", "male");
    cout << cJSON_Print(tmp) << endl;
    cout << cJSON_GetObjectItem(tmp, "name")->valuestring << endl;
}
