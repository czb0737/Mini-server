#ifndef USER_HANDLER_H
#define USER_HANDLER_H

#include <string>
#include <string.h>
#include <time.h>

#include "../../lib/md5.h"
#include "../../lib/cJSON.h"
#include "../../src/connect.cpp"

char *transfer_string_to_char(string s);
string userHandler(string &sql);

#endif