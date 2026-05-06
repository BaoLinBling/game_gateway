#include "middleware/mysql/mysql_conn.h"

MySQLConn::MySQLConn() {
    conn_ = mysql_init(nullptr);
}

MySQLConn::~MySQLConn() {
    if (conn_) {
        mysql_close(conn_);
    }
}

bool MySQLConn::connect(const std::string& host,
                        int port,
                        const std::string& user,
                        const std::string& pwd,
                        const std::string& db) {
    if (!mysql_real_connect(conn_, host.c_str(), user.c_str(),
                            pwd.c_str(), db.c_str(), port, nullptr, 0)) {
        return false;
    }
    return true;
}

bool MySQLConn::exec(const std::string& sql) {
    return mysql_query(conn_, sql.c_str()) == 0;
}

std::vector<std::map<std::string, std::string>> MySQLConn::query(const std::string& sql) {
    std::vector<std::map<std::string, std::string>> res;

    if (mysql_query(conn_, sql.c_str()) != 0) return res;

    MYSQL_RES* result = mysql_store_result(conn_);
    if (!result) return res;

    int num_fields = mysql_num_fields(result);
    MYSQL_FIELD* fields = mysql_fetch_fields(result);

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        std::map<std::string, std::string> m;
        for (int i = 0; i < num_fields; i++) {
            m[fields[i].name] = row[i] ? row[i] : "";
        }
        res.push_back(m);
    }

    mysql_free_result(result);
    return res;
}

void MySQLConn::setLastUsed(time_t t) {
    last_used_ = t;
}

time_t MySQLConn::getLastUsed() const {
    return last_used_;
}
