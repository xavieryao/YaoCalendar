#ifndef EVENTSTORAGE_H
#define EVENTSTORAGE_H

#include <QObject>
#include <QCalendarWidget>
#include "eventmap.h"
#include "eventmaphelper.h"
#include <QMap>
#include <QList>

class EventStorage : public QObject
{
    Q_OBJECT
public:
    explicit EventStorage(QObject *parent = 0);
    CalendarEvent findPrimaryEvent(CalendarEvent e) const;
    void modifyEvent(CalendarEvent e);
    void removeEvent(CalendarEvent e);
    void createEvent(CalendarEvent e);

signals:

public slots:

private:
    QList<CalendarEvent> mEventList;
};

#endif // EVENTSTORAGE_H
