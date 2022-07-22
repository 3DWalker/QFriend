#include "qserver.h"
#include "database.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QServer w;
    w.show();

    DatabaseManager *m_database = DatabaseManager::getInstance();
    m_database->open();

    return a.exec();
}
