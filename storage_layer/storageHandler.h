#ifndef STORAGE_HANDLER_H
#define STORAGE_HANDLER_H

#include <iostream>
#include <vector>
#include "ConnectionPool.h"
#include "../lib/json.hpp"

using Json = nlohmann::json;

extern ConnectionPool *cp;

string storageHandler(string &sql);

#endif