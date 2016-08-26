#include "calendarevent.h"
#include <QDebug>

long long CalendarEvent::uuid = 0l;

CalendarEvent::CalendarEvent()
{
    this->mColor = QColor::fromRgb(153, 151,244);
}

CalendarEvent CalendarEvent::newInstance(CalendarEvent old) {
    old.mId = uuid;
    uuid++;
    return old;
}

void CalendarEvent::makeUnique()
{
    this->mId = uuid;
    uuid++;
}


QList<QDate> CalendarEvent::expandDateFromRepeat() const
{
    QList<QDate> dateList;
    QDate initDate = this->startDateTime().date();
    while (initDate <= this->repeatEndDate()) {
        dateList.append(initDate);
        switch(this->mRepeatMode) {
        case RepeatMode::NONE:
            initDate = initDate.addDays(1);
        case RepeatMode::PER_WEEK:
            initDate = initDate.addDays(7);
            break;
        case RepeatMode::PER_DAY:
            initDate = initDate.addDays(1);
            break;
        case RepeatMode::PER_MONTH:
            initDate = initDate.addMonths(1);
            break;
        case RepeatMode::PER_YEAR:
            initDate = initDate.addYears(1);
            break;
        default:
            break;
        }
    }
    return dateList;
}

QMap<QDate, QList<CalendarEvent> > CalendarEvent::expandToMap() const
{
    QMap<QDate, QList<CalendarEvent> > map;
    QDate initDate = this->startDateTime().date();
    CalendarEvent e = *this;
    while (initDate <= this->repeatEndDate()) {
        QList<QDate> dateList = e.expandDateFromDuration();
        for(QDate date: dateList) {
            QList<CalendarEvent> eList = map.value(date);
            eList.append(e);
            map.insert(date, eList);
        }
        if (mRepeatMode == RepeatMode::NONE) {
            break;
        }

        switch(this->mRepeatMode) {
        case RepeatMode::PER_WEEK:
            initDate = initDate.addDays(7);
            e.setEndTime(e.endDateTime().addDays(7));
            break;
        case RepeatMode::PER_DAY:
            initDate = initDate.addDays(1);
            e.setEndTime(e.endDateTime().addDays(1));
            break;
        case RepeatMode::PER_MONTH:
            initDate = initDate.addMonths(1);
            e.setEndTime(e.endDateTime().addMonths(1));
            break;
        case RepeatMode::PER_YEAR:
            initDate = initDate.addYears(1);
            e.setEndTime(e.endDateTime().addYears(1));
            break;
        default:
            break;
        }
        QDateTime start = e.startDateTime();
        start.setDate(initDate);
        e.setStartTime(start);

    }
    return map;
}

QList<QDate> CalendarEvent::expandDateFromDuration() const {
    QList<QDate> dateList;
    QDate initDate = this->startDateTime().date();
    while (initDate <= this->endDateTime().date()) {
        dateList.append(initDate);
        initDate = initDate.addDays(1);
    }
    return dateList;
}

void CalendarEvent::write(QJsonObject &json) const {
    json["name"] = mEventName;
    json["location"] = mLocation;
    json["detail"] = mDetail;
    json["color"] = mColor.name();
    json["startDateTime"] = startDateTime().toString(Qt::ISODate);
    json["endDateTime"] = endDateTime().toString(Qt::ISODate);
    json["repeatMode"] = (int) mRepeatMode;
    json["repeatEndDate"] = repeatEndDate().toString(Qt::ISODate);
    json["isAllDay"] = mIsAllDayEvent;
    json["attachment"] = mAttachment;
}

void CalendarEvent::read(const QJsonObject &json) {
    mEventName = json["name"].toString();
    mLocation = json["location"].toString();
    mDetail = json["detail"].toString();
    mColor = QColor(json["color"].toString());
    mStartDateTime = QDateTime::fromString(json["startDateTime"].toString(), Qt::ISODate);
    mEndDateTime = QDateTime::fromString(json["endDateTime"].toString(), Qt::ISODate);
    mRepeatMode = RepeatMode(json["repeatMode"].toInt());
    mRepeatEndDate = QDate::fromString(json["repeatEndDate"].toString(), Qt::ISODate);
    mIsAllDayEvent = json["isAllDay"].toBool();
    mAttachment = json["attachment"].toString();
}

// Auto-generated getter and setter member functions,

QString CalendarEvent::attachment() const
{
    return mAttachment;
}

void CalendarEvent::setAttachment(const QString &attachment)
{
    mAttachment = attachment;
}

long long CalendarEvent::id() const
{
    return mId;
}

QDate CalendarEvent::repeatEndDate() const
{
    return mRepeatEndDate;
}

void CalendarEvent::setRepeatEndDate(const QDate &repeatEndDate)
{
    mRepeatEndDate = repeatEndDate;
}

RepeatMode CalendarEvent::repeatMode() const
{
    return mRepeatMode;
}

void CalendarEvent::setRepeatMode(const RepeatMode &repeatMode)
{
    mRepeatMode = repeatMode;
}

QString CalendarEvent::eventName() const
{
    return mEventName;
}

void CalendarEvent::setEventName(const QString &eventName)
{
    mEventName = eventName;
}

QString CalendarEvent::location() const
{
    return mLocation;
}

void CalendarEvent::setLocation(const QString &location)
{
    mLocation = location;
}

QDateTime CalendarEvent::startDateTime() const
{
    return mStartDateTime;
}

void CalendarEvent::setStartTime(const QDateTime &startTime)
{
    mStartDateTime = startTime;
}

QDateTime CalendarEvent::endDateTime() const
{
    return mEndDateTime;
}

void CalendarEvent::setEndTime(const QDateTime &endTime)
{
    mEndDateTime = endTime;
}

QColor CalendarEvent::color() const
{
    return mColor;
}

void CalendarEvent::setColor(const QColor &color)
{
    mColor = color;
}

bool CalendarEvent::isAllDayEvent() const
{
    return mIsAllDayEvent;
}

void CalendarEvent::setIsAllDayEvent(bool isAllDayEvent)
{
    mIsAllDayEvent = isAllDayEvent;
}

QString CalendarEvent::detail() const
{
    return mDetail;
}

void CalendarEvent::setDetail(const QString &detail)
{
    mDetail = detail;
}
