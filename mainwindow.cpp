#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_calendar = new MyCalendarWidget(this);
    ui->setupUi(this);

    m_calendar->setNavigationBarVisible(false);
    setCentralWidget(m_calendar);
}

MainWindow::~MainWindow()
{
    delete ui;
}
