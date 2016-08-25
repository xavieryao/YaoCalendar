#include "eventstorage.h"

EventStorage::EventStorage(QObject *parent) : QObject(parent)
{

}

CalendarEvent EventStorage::findPrimaryEvent(CalendarEvent e) const {
    return mEventList.at(mEventList.indexOf(e));
}

void EventStorage::modifyEvent(CalendarEvent e) {
    mEventList.replace(mEventList.indexOf(e), e);
}

void EventStorage::removeEvent(CalendarEvent e) {
    mEventList.removeOne(e);
}

void EventStorage::createEvent(CalendarEvent e) {
    mEventList.append(e);
}
