#ifndef CALENDAREVENT_H
#define CALENDAREVENT_H

#include <QObject>
#include <QDateTime>
#include <QColor>
#include <QList>
#include <QDate>
#include <QMap>

enum RepeatMode
{
    NONE, PER_DAY_OF_WEEK, PER_DAY, PER_MONTH, PER_YEAR
};

Q_DECLARE_METATYPE(RepeatMode)

class CalendarEvent
{
public:
    explicit CalendarEvent();
    static CalendarEvent newInstance(CalendarEvent old = CalendarEvent());

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

    QList<QDate> expandDateFromRepeat() const;
    QList<QDate> expandDateFromDuration() const;
    QMap<QDate, QList<CalendarEvent> > expandToMap() const;

    long long id() const;
    void makeUnique();

    static long long uuid;

    bool operator==(const CalendarEvent& right) {
        return this->id() == right.id();
    }

    QDate repeatStartDate() const;
    void setRepeatStartDate(const QDate &repeatStartDate);

    QDate repeatEndDate() const;
    void setRepeatEndDate(const QDate &repeatEndDate);

    RepeatMode repeatMode() const;
    void setRepeatMode(const RepeatMode &repeatMode);

signals:

public slots:

private:
    QString mEventName;
    QString mLocation;
    QDateTime mStartDateTime;
    QDateTime mEndDateTime;
    QDate mRepeatStartDate;
    QDate mRepeatEndDate;
    QColor mColor;
    bool mIsAllDayEvent;
    QString mDetail;
    RepeatMode mRepeatMode = RepeatMode::NONE;

    long long mId;

};

Q_DECLARE_METATYPE(CalendarEvent)


#endif // CALENDAREVENT_H
