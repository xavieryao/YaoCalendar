#ifndef EVENTMAP_H
#define EVENTMAP_H

#include <QMap>
#include <QDate>
#include <QList>
#include "calendarevent.h"

class EventMap : public QMap<QDate, QList<CalendarEvent> > {
};

#endif // EVENTMAP_H
