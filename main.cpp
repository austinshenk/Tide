#include "tide.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    qDebug() << "Tide start.";
    QApplication a(argc, argv);
    Tide w;
    w.show();

    return a.exec();
}
