#include "sslmain.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SSLMain w;
    w.setWindowIcon(QIcon(":/logo.ico"));
    w.show();
    return a.exec();
}
