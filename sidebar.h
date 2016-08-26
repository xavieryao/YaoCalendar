#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QListWidget>
#include <QDate>

#include "eventmap.h"

class SideBar : public QWidget
{
    Q_OBJECT
public:
    explicit SideBar(QWidget *parent = 0);
    void setEventMap(EventMap* map) {
        mEventMap = map;
    }


signals:
    void editEvent(CalendarEvent event, bool isNew = false);
    void deleteEvent(CalendarEvent& e);

public slots:
    void updateEventList(const QDate &date);
private:
    EventMap* mEventMap;

    QString formatDescription(CalendarEvent& e) const;

    QTextBrowser* mTextBrowser;
    QListWidget* mList;

private slots:
    void eventActivated(QListWidgetItem* item);
    void showContextMenu(const QPoint &pos);

};

#endif // SIDEBAR_H
