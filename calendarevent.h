#ifndef CALENDAREVENT_H
#define CALENDAREVENT_H

#include <QObject>

class CalendarEvent : public QObject
{
    Q_OBJECT
public:
    explicit CalendarEvent(QObject *parent = 0);

signals:

public slots:
};

#endif // CALENDAREVENT_H