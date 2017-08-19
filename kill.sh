#!/bin/bash

kill -9 $(ps -ef|grep ./access_layer/server|gawk '$0 !~/grep/ {print $2}' |tr -s '\n' ' ')
kill -9 $(ps -ef|grep ./logic_layer/user/server|gawk '$0 !~/grep/ {print $2}' |tr -s '\n' ' ')
kill -9 $(ps -ef|grep ./logic_layer/friend/server|gawk '$0 !~/grep/ {print $2}' |tr -s '\n' ' ')
kill -9 $(ps -ef|grep ./logic_layer/message/server|gawk '$0 !~/grep/ {print $2}' |tr -s '\n' ' ')
kill -9 $(ps -ef|grep ./storage_layer/server|gawk '$0 !~/grep/ {print $2}' |tr -s '\n' ' ')
