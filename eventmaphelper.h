#ifndef EVENTMAPHELPER_H
#define EVENTMAPHELPER_H

#include "types.h"
#include "calendarevent.h"
#include <QMap>
#include <QList>

class EventMapHelper
{
public:
    EventMapHelper();
    static void mergeMap(EventMap& orig, EventMap& newMap);
    static void splitMap(EventMap& orig, EventMap& newMap);
};

#endif // EVENTMAPHELPER_H
