#include "mycalendarwidget.h"
#include <QDebug>

MyCalendarWidget::MyCalendarWidget(QWidget* parent) : QCalendarWidget(parent)
{
    // empty constructor
}

void MyCalendarWidget::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
{
    if (mEventMap == NULL) {
        QCalendarWidget::paintCell(painter, rect, date);
        return;
    }
    if (mEventMap->contains(date)) {
        qDebug() << "have event(s) on " << date;
    }
}
