#include "database.h"

//构造函数--指定数据库类型
DatabaseManager::DatabaseManager()
{
    m_database = QSqlDatabase::addDatabase("QSQLITE");
}

//获取数据库实例
DatabaseManager *DatabaseManager::getInstance()
{
    static DatabaseManager instance;
    return &instance;
}

//打开数据库
bool DatabaseManager::open()
{
    m_database.setDatabaseName(m_databaseName);
    bool ok = m_database.open();
    if (ok)
    {
        ok = init_tb_Account() && init_tb_Friend();
        if (!ok)
        {
            close();
        }
    }
    else
    {
        qDebug() << "open databae failed, error: " << m_database.lastError().text();
    }

    return ok;
}

//关闭数据库
void DatabaseManager::close()
{
    if (m_database.isOpen())
    {
        m_database.close();
    }
    m_database.removeDatabase(m_databaseName);
}

// 初始化账户信息表
bool DatabaseManager::init_tb_Account()
{
    bool ok = m_database.isOpen();
    if (ok)
    {
        QSqlQuery query;
        query.prepare("create table if not exists tb_Account("
                      "id integer primary key,"
                      "name varchar(50) unique,"
                      "pswd varchar(50),"
                      "online integer default 0,"
                      "sex varchar(50) default '男',"
                      "age integer,"
                      "picture varchar(50));");
        ok = query.exec();
        if (!ok)
        {
            qDebug() << "Initi account table failed, error: " << query.lastError().text();
        }
    }
    else
    {
        qDebug() << "Initi account table failed, error: the database isn't open";
    }

    return ok;
}

//初始化组织信息表
bool DatabaseManager::init_tb_Friend()
{
    bool ok = m_database.isOpen();
    if (ok)
    {
        QSqlQuery query;
        query.prepare("create table if not exists tb_Friend("
                      "id integer,"
                      "friendID integer,"
                      "primary key(id, friendID));");
        ok = query.exec();
        if (!ok)
        {
            qDebug() << "Initi friend table failed, error: " << query.lastError().text();
        }
    }
    else
    {
        qDebug() << "Initi friend table failed, error: the database isn't open";
    }

    return ok;
}

//处理新用户注册操作
bool DatabaseManager::handleUserRegist(const char *name, const char *pswd)
{
    if (NULL == name || NULL == pswd)
    {
        return false;
    }
    bool ok = m_database.isOpen();
    if (ok)
    {
        QSqlQuery query;
        query.prepare("insert into tb_Account(name, pswd)"
                      "values(:name, :pswd);");
        query.bindValue(":name", name);
        query.bindValue(":pswd", pswd);
        ok = query.exec();
        if (!ok)
        {
            qDebug() << "insert into tb_Account failed, error: " << query.lastError().text();
        }
    }
    else
    {
        qDebug() << "insert into tb_Account failed, error: the database isn't open";
    }

    return ok;
}

/*
 * 功能：处理用户登录操作
 * 返回值：0--登录成功、1--参数为空、2--用户名或密码错误、3--用户已登录
*/
int DatabaseManager::handleUserLogin(const char *name, const char *pswd)
{
    int res = 1;
    if (NULL == name || NULL == pswd)
    {
        return res;
    }
    if (m_database.isOpen())
    {
        QSqlQuery query;
        query.prepare("select name, pswd from tb_Account where name = :name and pswd = :pswd");
        query.bindValue(":name", name);
        query.bindValue(":pswd", pswd);
        if (query.exec())
        {
            if (query.next())
            {
                int online = query.value(3).toInt();
                if (0 == online)
                {
                    query.prepare("update tb_Account set online = 1 where name = :name");
                    query.bindValue(":name", name);
                    if (!query.exec())
                    {
                        qDebug() << "updata tb_Account set online failed, error: " << query.lastError().text();
                    }
                    res = 0;
                }
                else
                {
                    return 3;
                }
            }
            else
            {
                return 2;
            }
        }
        else
        {
            qDebug() << "handle user login failed, error: " << query.lastError().text();
        }
    }
    else
    {
        qDebug() << "handle user login failed, error: the database isn't open";
    }

    return res;
}

/*
 * 功能：处理请求添加用户操作
 * 返回值：0--可添加好友、1--参数为空、2--无此用户、3--好友已存在
*/
int DatabaseManager::handleAddFriend(const char *localName, const char *preName)
{
    if (NULL == localName || NULL == preName)
    {
        return 1;
    }
    if (m_database.isOpen())
    {
        QSqlQuery query;
        query.prepare("select * from tb_Account where name = :name");
        query.bindValue(":name", preName);
        if (query.exec())
        {
            if (!query.next())
            {
                return 2;
            }
        }
        else
        {
            qDebug() << "select tb_Account failed, error: " << query.lastError().text();
        }
        query.prepare("select * from tb_Friend where "
                      "(id = (select id from tb_Account where name = :localName) and friendID = (select id from tb_Account where name = :preName)) "
                      "or (id = (select id from tb_Account where name = :preName) and friendID = (select id from tb_Account where name = :localName))");
        query.bindValue(":localName", localName);
        query.bindValue(":preName", preName);
        if (query.exec())
        {
            if(query.next())
            {
                return 3;
            }
        }
        else
        {
            qDebug() << "select tb_Friend failed, error: " << query.lastError().text();
        }
    }
    else
    {
        qDebug() << "select tb_Friend failed, error: the database isn't open";
    }
    return 0;
}

/*
 * 功能：处理添加好友申请是否成功
 * 返回值：0--添加成功、1--添加失败
*/
int DatabaseManager::handleAddfriendAgree(const char *srcName, const char *preName)
{
    if (NULL == srcName || NULL == preName)
    {
        return 1;
    }
    if (m_database.isOpen())
    {
        QSqlQuery query;
        query.prepare("insert into tb_Friend(id, friendID) values("
                      "(select id from tb_Account where name = :srcName), (select id from tb_Account where name = :preName))");
        query.bindValue(":srcName", srcName);
        query.bindValue(":preName", preName);
        if (query.exec())
        {
            return 0;
        }
        else
        {
            qDebug() << "insert into tb_Friend failed, error: " << query.lastError().text();
        }
    }
    else
    {
        qDebug() << "insert into tb_Friend failed, error: the database isn't open";
    }

    return 1;
}

//显示所有好友
QStringList DatabaseManager::handleShowAllFriend(const char *srcName)
{
    QStringList res;
    if (NULL == srcName)
    {
        return res;
    }
    if (m_database.isOpen())
    {
        QSqlQuery query;
        query.prepare("select name from tb_Account where "
                      "id in (select id from tb_Friend where FriendID = (select id from tb_Account where name = :srcName))");
        query.bindValue(":srcName", srcName);
        if (query.exec())
        {
            while (query.next())
            {
                res.append(query.value(0).toString());
            }
        }
        else
        {
            qDebug() << "show all friend failed, error: " << query.lastError().text();
        }
        query.prepare("select name from tb_Account where "
                      "id in (select FriendID from tb_Friend where id = (select id from tb_Account where name = :srcName))");
        query.bindValue(":srcName", srcName);
        if (query.exec())
        {
            while (query.next())
            {
                res.append(query.value(0).toString());
            }
        }
        else
        {
            qDebug() << "show all friend failed, error: " << query.lastError().text();
        }
    }
    else
    {
        qDebug() << "show all friend failed, error: the database isn't open";
    }

    return res;
}
