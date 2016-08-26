#ifndef MYCALENDARWIDGET_H
#define MYCALENDARWIDGET_H

#include <QCalendarWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMap>
#include <QRect>
#include <QPair>
#include "eventmap.h"

class MyCalendarWidget : public QCalendarWidget
{
public:
    explicit MyCalendarWidget(QWidget* parent = 0);
    void setEventMap(EventMap* map) {mEventMap = map;}

protected:
    virtual void paintCell(QPainter * painter, const QRect & rect, const QDate & date) const;
    virtual void dragEnterEvent(QDragEnterEvent* e);
    virtual void dropEvent(QDropEvent *e);
    virtual void resizeEvent(QResizeEvent *event);

private:
    EventMap* mEventMap = NULL;
    QList<QPair<QRect, QDate> > mRectDateList;
};

#endif // MYCALENDARWIDGET_H
