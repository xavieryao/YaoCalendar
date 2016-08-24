#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFontDatabase>
#include <QMap>
#include <QDate>
#include "mycalendarwidget.h"
#include "sidebar.h"
#include "eventmap.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    Ui::MainWindow *ui;
    SideBar* mSideBar;

    EventMap* mEventMap;

    void setUpCalendarNavigator();

    const int MIN_WIDTH_WITH_SIDEBAR = 590;
    const int SIDE_BAR_WIDTH = 240;

private slots:
    void formatAndSetMonthLabel(int year, int month);
};

#endif // MAINWINDOW_H
