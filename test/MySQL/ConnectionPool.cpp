#include "ConnectionPool.h"

ConnectionPool *ConnectionPool::instance = nullptr;

ConnectionPool::ConnectionPool()
{
    this->driver = sql::mysql::get_driver_instance();
}

void ConnectionPool::initialConnections()
{
    pthread_mutex_lock(&lock);
    for (int i = 0; i < INITIAL_SIZE; ++i)
    {
        this->createConnection();
    }
    pthread_mutex_unlock(&lock);
}

void ConnectionPool::createConnection()
{
    int try_time = 3;
    while (try_time--)
    {
        try
        {
            this->connections.push(driver->connect(HOST, USER, PASSWORD));
            this->connections.back()->setSchema("chat");
            return;
        }
        catch(sql::SQLException e)
        {
            cerr << "Fail to create connection! (" + to_string(3 - try_time) + " times)" << endl;
            pthread_mutex_unlock(&lock);
            sleep(1);
            pthread_mutex_lock(&lock);
        }
    }
}

ConnectionPool * ConnectionPool::getInstance()
{
    if (instance == nullptr)
        instance = new ConnectionPool();
    return instance;
}

vector<string> ConnectionPool::getData(string sql)
{
    sql::PreparedStatement *prep_stmt = this->connections.front()->prepareStatement(sql);
    sql::ResultSet *res = prep_stmt->executeQuery();
    vector<string> data;
    string s;
    while (res->next())
    {
        s.clear();
        s.append(to_string(res->getInt("userId")) + "\t").append(res->getString("userName") + "\t").append(res->getString("password"));
        data.push_back(s);
    }
    delete res;
    delete prep_stmt;
    return data;
}