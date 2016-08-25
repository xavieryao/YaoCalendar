#include "eventmaphelper.h"

EventMapHelper::EventMapHelper()
{

}

void EventMapHelper::mergeMap(EventMap &orig, EventMap &newMap) {
    for (auto key: newMap.keys()) {
        QList<CalendarEvent> list = orig.value(key);
        list += newMap.value(key);
        orig.insert(key, list);
    }
}

void EventMapHelper::splitMap(EventMap &orig, EventMap &newMap) {
    for (auto key: newMap.keys()) {
        QList<CalendarEvent> list = orig.value(key);
        list -= newMap.value(key);
        orig.insert(key, list);
    }
}
