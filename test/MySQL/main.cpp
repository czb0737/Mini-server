#include <iostream>
#include <vector>
#include <string>
#include <pthread.h>

#include "ConnectionPool.h"

using namespace std;

vector<string> getData(string sql, Connection *conn);

int main(int argc, char *argv[])
{
    ConnectionPool *cp = ConnectionPool::getInstance();
    Connection *conn = cp->getConnection();
    // sleep(10);
    cout << string(argv[1]) << endl;
    // cout << string(argv[2]) << endl;
    // cp->getData(string(argv[1]));
    auto v = getData(string(argv[1]), conn);
    for (auto s : v)
        cout << s << endl;
    cp->releaseConnection(conn);
    cout << cp->getSize() << endl;
    delete cp;
}

vector<string> getData(string sql, Connection *conn)
{
    sql::PreparedStatement *prep_stmt = conn->prepareStatement(sql);
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