#include "mainwindow.h"
#include "calendarevent.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);


    QTranslator myTranslator;
    qDebug() << QLocale::system().name();
    myTranslator.load(":/translations/" + QLocale::system().name());
    a.installTranslator(&myTranslator);

    MainWindow w;
    w.show();

    return a.exec();
}
