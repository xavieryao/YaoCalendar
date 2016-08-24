#ifndef CALENDAREVENT_H
#define CALENDAREVENT_H

#include <QObject>
#include <QDateTime>
#include <QColor>
#include "repeatoptions.h"

class CalendarEvent : public QObject
{
    Q_OBJECT
public:
    explicit CalendarEvent(QObject *parent = 0);
    QString eventName() const;
    void setEventName(const QString &eventName);

    QString location() const;
    void setLocation(const QString &location);

    QDateTime startTime() const;
    void setStartTime(const QDateTime &startTime);

    QDateTime endTime() const;
    void setEndTime(const QDateTime &endTime);

    QColor color() const;
    void setColor(const QColor &color);

    bool isAllDayEvent() const;
    void setIsAllDayEvent(bool isAllDayEvent);

    QString detail() const;
    void setDetail(const QString &detail);

signals:

public slots:

private:
    QString mEventName;
    QString mLocation;
    QDateTime mStartTime;
    QDateTime mEndTime;
    QColor mColor;
    bool mIsAllDayEvent;
    QString mDetail;
    RepeatMode mRepeatMode;
};

enum RepeatMode
{
    NONE, PER_DAY_OF_WEEK, PER_DAY, PER_MONTH, PER_YEAR
};

#endif // CALENDAREVENT_H
