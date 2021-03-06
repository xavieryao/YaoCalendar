#ifndef EVENTSTORAGE_H
#define EVENTSTORAGE_H

#include <QObject>
#include <QCalendarWidget>
#include "eventmap.h"
#include "eventmaphelper.h"
#include <QMap>
#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

class EventStorage : public QObject
{
    Q_OBJECT
public:
    explicit EventStorage(QObject *parent = 0);
    CalendarEvent findPrimaryEvent(CalendarEvent e) const;
    void modifyEvent(CalendarEvent e);
    void removeEvent(CalendarEvent e);
    void createEvent(CalendarEvent e);
    EventMap* createEventMap();
    bool loadFromFile(QString fileName = QString());
    bool saveToFile(QString fileName = QString());
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
    void merge(EventStorage* from);

    QString userName() const;
    void setUserName(const QString &userName);

    QList<CalendarEvent> eventList() const;

signals:

public slots:

private:
    QList<CalendarEvent> mEventList;
    QString mUserName = "default";
};

#endif // EVENTSTORAGE_H
