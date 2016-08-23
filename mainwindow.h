#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFontDatabase>
#include "mycalendarwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QCalendarWidget *m_calendar;

    void setUpCalendarNavigator();

private slots:
    void formatAndSetMonthLabel(int year, int month);
};

#endif // MAINWINDOW_H
