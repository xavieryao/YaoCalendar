#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFontDatabase>
#include <QMap>
#include <QDate>
#include <QInputDialog>
#include "mycalendarwidget.h"
#include "sidebar.h"
#include "eventmap.h"
#include "calendarevent.h"
#include "eventstorage.h"
#include <QDebug>
#include <QFont>
#include <QResizeEvent>
#include <QTime>
#include <QMessageBox>
#include <QFile>
#include <QMessageBox>
#include <QAbstractButton>
#include <QDir>
#include <QJsonDocument>
#include <QFileInfo>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QSettings>
#include <QMenuBar>
#include "eventdialog.h"
#include "eventmaphelper.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~MainWindow();
    void setSelection(const QDate& date);
    void showTrayIcon();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    Ui::MainWindow *ui;
    SideBar* mSideBar;

    EventMap* mEventMap;
    EventStorage* mEventStorage;
    QSettings* mSettings;

    QMenu* mMenuUser;

    void setUpCalendarNavigator();
    void configureMultiUser(QStringList& userList);

    static const int MIN_WIDTH_WITH_SIDEBAR = 595;
    static const int SIDE_BAR_WIDTH = 240;
    const QString DEFAULT_USER_NAME = "default";

public slots:

private slots:
    void formatAndSetMonthLabel(int year, int month);
    void onDateActivated(const QDate &date, QString file);
    void onEventModified(CalendarEvent origEvent, CalendarEvent event, bool isNew);
    void openEventWindow(CalendarEvent event, bool newEvent = false);
    void onDeleteEvent(CalendarEvent& event);
    void onUserChanged();
    void addUser();
    void editUser();
    void exportEvents();
    void importEvents();
};

#endif // MAINWINDOW_H
