#ifndef EVENTSTORAGE_H
#define EVENTSTORAGE_H

#include <QObject>
#include <QCalendarWidget>

class EventStorage : public QObject
{
    Q_OBJECT
public:
    explicit EventStorage(QObject *parent = 0);

signals:

public slots:
};

#endif // EVENTSTORAGE_H
