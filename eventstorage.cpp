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

}

void EventStorage::read(const QJsonObject &json) {
    mEventList.clear();
    QJsonArray arr = json["events"].toArray();
    for (int i = 0; i < arr.size(); i++) {
        QJsonObject obj = arr.at(i).toObject();
        CalendarEvent e;
        e.read(obj);
        mEventList.append(e);
    }
}

void EventStorage::write(QJsonObject &json) const {
    json["version"] = "1.0"; // TEMP
    json["other_meta_info"] = "foobar";
    QJsonArray eventArr;
    for (CalendarEvent e: mEventList) {
        QJsonObject obj;
        e.write(obj);
        eventArr.append(obj);
    }
    json["events"] = eventArr;
}
