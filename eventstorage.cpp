#include "eventstorage.h"
#include <QDebug>

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

EventMap* EventStorage::createEventMap() {
    EventMap* map = new EventMap;
    for (auto event : mEventList) {
        EventMap toExpand = event.expandToMap();
        EventMapHelper::mergeMap(*map, toExpand);
    }
    return map;
}

bool EventStorage::loadFromFile(QString fileName) {
    QString name = fileName;
    if (fileName == QString()) {
        name = QString("./%1_save.json").arg(mUserName);
    }
    QFile saveFile(name);
    if (!saveFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray saveData = saveFile.readAll();

    QJsonDocument loadDoc = QJsonDocument::fromJson(saveData);
    read(loadDoc.object());
    return true;
}

bool EventStorage::saveToFile(QString fileName) {
    QString name = fileName;
    if (fileName == QString()) {
        name = QString("./%1_save.json").arg(mUserName);
    }
    QFile saveFile(name);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    QJsonObject saveObj;
    write(saveObj);
    QJsonDocument saveDoc(saveObj);
    saveFile.write(saveDoc.toJson());
    saveFile.close();
    return true;
}

void EventStorage::read(const QJsonObject &json) {
    mEventList.clear();
    QJsonArray arr = json["events"].toArray();
    for (int i = 0; i < arr.size(); i++) {
        QJsonObject obj = arr.at(i).toObject();
        CalendarEvent e = CalendarEvent::newInstance();
        e.read(obj);
        mEventList.append(e);
    }
    mUserName = json["username"].toString();
}

void EventStorage::write(QJsonObject &json) const
{
    json["version"] = "1.0"; // TEMP
    json["other_meta_info"] = "foobar";
    json["username"] = mUserName;
    QJsonArray eventArr;
    for (CalendarEvent e: mEventList) {
        QJsonObject obj;
        e.write(obj);
        eventArr.append(obj);
    }
    json["events"] = eventArr;
}

QString EventStorage::userName() const
{
    return mUserName;
}

void EventStorage::setUserName(const QString &userName)
{
    mUserName = userName;
}

QList<CalendarEvent> EventStorage::eventList() const
{
    return mEventList;
}

void EventStorage::merge(EventStorage *from)
{
    QList<CalendarEvent> list = from->eventList();
    for (auto event: list) {
        CalendarEvent e = CalendarEvent::newInstance(event);
        this->createEvent(e);
    }
}
