#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <string>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "../../lib/md5.h"
#include "../../lib/cJSON.h"
#include "../../src/connect.cpp"

char *transfer_string_to_char(string s);
string messageHandler(string &sql);

#endif