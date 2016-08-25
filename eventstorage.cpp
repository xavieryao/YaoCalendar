#include "eventstorage.h"
#include <QDebug>

EventStorage::EventStorage(QObject *parent) : QObject(parent)
{

}

CalendarEvent EventStorage::findPrimaryEvent(CalendarEvent e) const {
    qDebug() << "find primary";
    qDebug() << mEventList.indexOf(e);
    qDebug() << mEventList.at(mEventList.indexOf(e)).startDateTime();
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

EventMap* EventStorage::createEventMap() {

}

void EventStorage::loadFromFile(QString fileName) {
    // DEBUG

}
