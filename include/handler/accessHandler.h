#ifndef ACCESS_HANDLER_H
#define ACCESS_HANDLER_H

#include <string>

#include "../../src/connect.cpp"
#include "../../lib/cJSON.c"

#define ACCESS_PORT 8192    // Access layer port
#define USER_PORT 8193      // Logic layer user module port
#define FRIEND_PORT 8194    // Logic layer friend module port
#define MESSAGE_PORT 8195   // Logic layer message module port
#define STORAGE_PORT 8196   // Storage layer port

char *transfer_string_to_char(string s);
string accessHandler(string &sql);

#endif