/*************************************************************************
	> File Name: ConnectionPool.h
	> Author: czb
	> Mail: zhibinc.cs@gmail.com
	> Created Time: 08/20/2017 Sun 14:20:23
 ************************************************************************/

#ifndef CONNECTION_POOL_H
#define CONNECTION_POOL_H

#include <iostream>
#include <queue>
#include <string>
#include <pthread.h>
// MySQL Connector
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h> 

using namespace std;
using namespace sql;

const string HOST = "localhost";
const string USER = "root";
const string PASSWORD = "czb";
const string DATABASE = "chat";

class ConnectionPool
{
private:
	const int INITIAL_SIZE = 10;
	const int MAX_SIZE = 1000;

	int cur_size;
	sql::mysql::MySQL_Driver *driver;
	static ConnectionPool *instance;
	queue<Connection *> connections;
	static pthread_mutex_t lock;
	void initialConnections();
	Connection * createConnection();
	void terminateConnection(Connection *conn);
	void terminateConnectionPool();

    ConnectionPool();

public:
	Connection * getConnection();
	void releaseConnection(Connection *conn);
	int getSize();
    vector<string> getData(string sql);
	static ConnectionPool * getInstance();
	
	~ConnectionPool();
};

#endif