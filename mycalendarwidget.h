#ifndef MYCALENDARWIDGET_H
#define MYCALENDARWIDGET_H

#include <QCalendarWidget>
#include "eventmap.h"

class MyCalendarWidget : public QCalendarWidget
{
public:
    explicit MyCalendarWidget(QWidget* parent = 0);
    void setEventMap(EventMap* map) {mEventMap = map;}

protected:
    virtual void paintCell(QPainter * painter, const QRect & rect, const QDate & date) const;

private:
    EventMap* mEventMap = NULL;
};

#endif // MYCALENDARWIDGET_H
