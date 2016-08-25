#include "calendarevent.h"
#include <QDebug>

long CalendarEvent::uuid = 0l;

CalendarEvent::CalendarEvent()
{
    this->mId = uuid;
    this->mRepeatMode = RepeatMode::NONE;
    // DEBUG
    this->mRepeatMode = RepeatMode::PER_DAY;
    // END_DEBUG
    uuid++;
}

QList<QDate> CalendarEvent::expandDateFromRepeat() const
{
    QList<QDate> dateList;
    QDate initDate = this->startDateTime().date();
    while (initDate <= this->endDateTime().date()) {
        dateList.append(initDate);
        if (mRepeatMode == RepeatMode::NONE) {
            break;
        }
        switch(this->mRepeatMode) {
        case RepeatMode::PER_DAY_OF_WEEK:
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

int CalendarEvent::id() const
{
    return mId;
}

// Auto-generated getter and setter member functions,
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
