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
#include "calendarevent.h"
#include "eventstorage.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setSelection(const QDate& date);
    void showTrayIcon();

protected:
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *e);

private:
    Ui::MainWindow *ui;
    SideBar* mSideBar;

    EventMap* mEventMap;
    EventStorage* mEventStorage;

    void setUpCalendarNavigator();

    static const int MIN_WIDTH_WITH_SIDEBAR = 580;
    static const int SIDE_BAR_WIDTH = 240;
    const QString SAVE_FILE_NAME = "./save.json";

public slots:

private slots:
    void formatAndSetMonthLabel(int year, int month);
    void onDateActivated(const QDate &date, QString file);
    void onEventModified(CalendarEvent origEvent, CalendarEvent event, bool isNew);
    void openEventWindow(CalendarEvent event, bool newEvent = false);

};

#endif // MAINWINDOW_H
