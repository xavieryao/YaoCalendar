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

QDate CalendarEvent::repeatStartDate() const
{
    return mRepeatStartDate;
}

void CalendarEvent::setRepeatStartDate(const QDate &repeatStartDate)
{
    mRepeatStartDate = repeatStartDate;
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

QList<QDate> CalendarEvent::expandDateFromRepeat() const
{
    QList<QDate> dateList;
    QDate initDate = this->startDateTime().date();
    while (initDate <= this->endDateTime().date()) {
        dateList.append(initDate);
        switch(this->mRepeatMode) {
        case RepeatMode::NONE:
            initDate = initDate.addDays(1);
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

long long CalendarEvent::id() const
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
