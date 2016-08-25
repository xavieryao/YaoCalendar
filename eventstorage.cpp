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
    // Not implemented
}

void EventStorage::loadFromFile(QFile& saveFile) {
    if (!saveFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QByteArray saveData = saveFile.readAll();

    QJsonDocument loadDoc = QJsonDocument::fromJson(saveData);
    read(loadDoc.object());
}

void EventStorage::saveToFile(QFile& saveFile) {
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }
    QJsonObject saveObj;
    write(saveObj);
    QJsonDocument saveDoc(saveObj);
    saveFile.write(saveDoc.toJson());
    saveFile.close();
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
