#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QCoreApplication>

class DatabaseManager
{
private:
    DatabaseManager();
    QSqlDatabase m_database;
    QString m_databaseName = QCoreApplication::applicationDirPath() + "cloud.db";
    QString m_userName;

public:
    static DatabaseManager *getInstance();

    bool open();

    void close();

    bool init_tb_Account();

    bool init_tb_Friend();

    bool handleUserRegist(const char *name, const char *pswd);

    int handleUserLogin(const char *name, const char *pswd);

    int handleAddFriend(const char *localName, const char *preName);

    int handleAddfriendAgree(const char *srcName, const char *preName);

    QStringList handleShowAllFriend(const char *srcName);
};

#endif // DATABASE_H
