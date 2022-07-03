#include "QtSortComparison.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtSortComparison w;
    w.show();
    return a.exec();
}
