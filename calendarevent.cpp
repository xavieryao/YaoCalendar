#include "calendarevent.h"

CalendarEvent::CalendarEvent()
{

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

QDateTime CalendarEvent::startTime() const
{
    return mStartTime;
}

void CalendarEvent::setStartTime(const QDateTime &startTime)
{
    mStartTime = startTime;
}

QDateTime CalendarEvent::endTime() const
{
    return mEndTime;
}

void CalendarEvent::setEndTime(const QDateTime &endTime)
{
    mEndTime = endTime;
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
