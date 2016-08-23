#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QFontDatabase::addApplicationFont(":Font/fontawesome");
    QFontDatabase::addApplicationFont(":Font/nevis");
    ui->setupUi(this);
    QFile qssFile(":/stylesheet/global");
    qssFile.open(QFile::ReadOnly);
    if (qssFile.isOpen()) {
        QString qss = QString(qssFile.readAll());
        qDebug() << "qss loaded. " << qss;
        qApp->setStyleSheet(qss);
        qssFile.close();
    }

    setCentralWidget(ui->verticalLayoutWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
