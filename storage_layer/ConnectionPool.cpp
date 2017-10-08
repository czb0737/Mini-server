#include "ConnectionPool.h"

ConnectionPool *ConnectionPool::instance = nullptr;
pthread_mutex_t ConnectionPool::lock = PTHREAD_MUTEX_INITIALIZER;

ConnectionPool::ConnectionPool()
{
    try
    {
        this->driver = sql::mysql::get_driver_instance();
    }
    catch (SQLException e)
    {
        cerr << "Fail to get driver!" << endl;
    }
    this->initialConnections();
}

ConnectionPool::~ConnectionPool()
{
    this->terminateConnectionPool();
}

void ConnectionPool::initialConnections()
{
    pthread_mutex_lock(&lock);
    for (int i = 0; i < INITIAL_SIZE; ++i)
    {
        Connection *conn = this->createConnection();
        if (conn != nullptr)
        {
            connections.push(conn);
        }
    }
    pthread_mutex_unlock(&lock);
    this->cur_size = INITIAL_SIZE;
}

Connection * ConnectionPool::createConnection()
{
    int try_time = 3;
    while (try_time--)
    {
        try
        {
            Connection *conn = driver->connect(HOST, USER, PASSWORD);
            conn->setSchema(DATABASE);
            return conn;
        }
        catch(sql::SQLException e)
        {
            cerr << "Fail to create connection! (" + to_string(3 - try_time) + " times)" << endl;
            pthread_mutex_unlock(&lock);
            sleep(1);
            pthread_mutex_lock(&lock);
        }
    }
    return nullptr;
}

Connection * ConnectionPool::getConnection()
{
    pthread_mutex_lock(&lock);
    if (connections.empty())
    {
        if (cur_size < MAX_SIZE)
        {
            Connection *conn = createConnection();
            if (conn == nullptr)
            {
                pthread_mutex_unlock(&lock);
                return nullptr;
            }
            else
            {
                ++cur_size;
                pthread_mutex_unlock(&lock);
                return conn;
            }
        }
        else
        {
            cout << "GetConnection: The size of the connection pool has reached the maximum!" << endl;
            pthread_mutex_unlock(&lock);
            return nullptr;
        }
    }
    else
    {
        Connection *conn = nullptr;
        while (!connections.empty() && conn == nullptr)
        { 
            conn = connections.front();
            connections.pop();
            // Delete the closed connection in the front of the connection queue
            if (conn->isClosed())
            {
                delete conn;
                conn = nullptr;
                --cur_size;
            }
        }
        if (conn == nullptr)
        {
            conn = createConnection();
            ++cur_size;
        }
        pthread_mutex_unlock(&lock);
        return conn;
    }
}

void ConnectionPool::releaseConnection(Connection *conn)
{
    if (conn != nullptr)
    {
        pthread_mutex_lock(&lock);
        this->connections.push(conn);
        pthread_mutex_unlock(&lock);
    }
}

ConnectionPool * ConnectionPool::getInstance()
{
    if (instance == nullptr)
    {
        pthread_mutex_lock(&lock);
        if (instance == nullptr)
            instance = new ConnectionPool();
        pthread_mutex_unlock(&lock);
    }
    return instance;
}

void ConnectionPool::terminateConnection(Connection *conn)
{
    if (conn != nullptr)
    {
        try
        {
            conn->close();
        }
        catch (SQLException e)
        {
            cerr << "Fail to close connection!" << endl;
        }
        delete conn;
        conn = nullptr;
    }
}

void ConnectionPool::terminateConnectionPool()
{
    pthread_mutex_lock(&lock);
    while (!this->connections.empty())
    {
        this->terminateConnection(connections.front());
        connections.pop();
    }
    this->cur_size = 0;
    pthread_mutex_unlock(&lock);
}

int ConnectionPool::getSize()
{
    pthread_mutex_lock(&lock);
    int size = this->cur_size;
    pthread_mutex_unlock(&lock);
    return size;
}

vector<string> ConnectionPool::getData(string sql)
{
    vector<string> data;
    // sql::PreparedStatement *prep_stmt = this->connections.front()->prepareStatement(sql);
    // sql::ResultSet *res = prep_stmt->executeQuery();
    // string s;
    // while (res->next())
    // {
    //     s.clear();
    //     s.append(to_string(res->getInt("userId")) + "\t").append(res->getString("userName") + "\t").append(res->getString("password"));
    //     data.push_back(s);
    // }
    // delete res;
    // delete prep_stmt;
    return data;
}