#include "qclient.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QClient::getInstance().show();
    return a.exec();
}
