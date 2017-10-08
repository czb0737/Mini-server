#ifndef FRIEND_HANDLER_H
#define FRIEND_HANDLER_H

#include <string>
#include <string.h>
#include <time.h>

#include "../../lib/md5.h"
#include "../../lib/cJSON.h"
#include "../../src/connect.cpp"

char *transfer_string_to_char(string s);
string friendHandler(string &sql);

#endif