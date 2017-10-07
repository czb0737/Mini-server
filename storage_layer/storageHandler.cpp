#include "storageHandler.h"

ConnectionPool *cp = ConnectionPool::getInstance();

string storageHandler(string &sql)
{
    Connection *conn = cp->getConnection();
    PreparedStatement *pstmt = conn->prepareStatement(sql);
    Json json = Json();

    // Check the SQL is "select..." or others
    if (sql.front() == 'S' || sql.front() == 's')
    {
        ResultSet *rs = pstmt->executeQuery();
        ResultSetMetaData *rs_meta = rs->getMetaData();

        int num_cols = rs_meta->getColumnCount();
        vector<string> col_names;

        for (int i = 0; i < num_cols; ++i)
        {
            col_names.push_back(rs_meta->getColumnLabel(i + 1));
        }

        cout << "Column name: " << endl;
        for (auto &name : col_names)
            cout << name << endl;

        while (rs->next())
        {
            for (int i = 0; i < num_cols; ++i)
            {
                cout << col_names[i] << " @@ " << rs->getString(col_names[i]);
                json[col_names[i]] = rs->getString(col_names[i]);
            }
        }
        cout << "Before count" << endl;
        json["count"] = rs->rowsCount();
        cout << "Before result" << endl;
        json["result"] = 0;//rs->wasNull();
        cout << "Before delete" << endl;
        delete rs;
    }
    else
    {
        int count = pstmt->executeUpdate();
        json["count"] = count;
        json["result"] = count < 1;
    }
    cout << "Before release" << endl;
    delete pstmt;
    cp->releaseConnection(conn);
    cout << "After release" << endl;
    return json.dump();
}