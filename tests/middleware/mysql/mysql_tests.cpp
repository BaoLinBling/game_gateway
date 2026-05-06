#include <gtest/gtest.h>
#include "middleware/mysql/mysql_pool.h"

TEST(MySQLTest, ExecAndQuery) {
    auto& pool = MySQLPool::getInstance();

    pool.exec("CREATE TABLE IF NOT EXISTS test(id INT PRIMARY KEY, val VARCHAR(32));");
    pool.exec("DELETE FROM test;");
    pool.exec("INSERT INTO test VALUES(1,'abc');");

    auto res = pool.query("SELECT val FROM test WHERE id=1;");
    ASSERT_EQ(res.size(), 1);
    ASSERT_EQ(res[0]["val"], "abc");
}

TEST(MySQLTest, Transaction) {
    auto& pool = MySQLPool::getInstance();

    bool ok = pool.transaction([](MySQLConn& conn) {
        conn.exec("INSERT INTO test VALUES(2,'tx');");
        return true;
    });

    ASSERT_TRUE(ok);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
