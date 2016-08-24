#ifndef EVENTMAP_H
#define EVENTMAP_H

#include <QMap>
#include <QDate>
#include <QList>

class EventMap : public QMap<QDate, QList<QString> > {

};

#endif // EVENTMAP_H
