#include "QtTranslate.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtTranslate w;
    w.show();
    return a.exec();
}
