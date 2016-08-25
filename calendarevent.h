#ifndef CALENDAREVENT_H
#define CALENDAREVENT_H

#include <QObject>
#include <QDateTime>
#include <QColor>
#include <QList>
#include <QDate>

enum RepeatMode
{
    NONE, PER_DAY_OF_WEEK, PER_DAY, PER_MONTH, PER_YEAR
};

class CalendarEvent
{
public:
    explicit CalendarEvent();
    QString eventName() const;
    void setEventName(const QString &eventName);

    QString location() const;
    void setLocation(const QString &location);

    QDateTime startDateTime() const;
    void setStartTime(const QDateTime &startDateTime);

    QDateTime endDateTime() const;
    void setEndTime(const QDateTime &endDateTime);

    QColor color() const;
    void setColor(const QColor &color);

    bool isAllDayEvent() const;
    void setIsAllDayEvent(bool isAllDayEvent);

    QString detail() const;
    void setDetail(const QString &detail);

    QList<QDate> expandDateFromRepeat();

signals:

public slots:

private:
    QString mEventName;
    QString mLocation;
    QDateTime mStartDateTime;
    QDateTime mEndDateTime;
    QColor mColor;
    bool mIsAllDayEvent;
    QString mDetail;
    RepeatMode mRepeatMode;
};


#endif // CALENDAREVENT_H
