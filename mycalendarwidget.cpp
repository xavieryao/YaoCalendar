#include "mycalendarwidget.h"

MyCalendarWidget::MyCalendarWidget(QWidget* parent) : QCalendarWidget(parent)
{
    // empty constructor
}

void MyCalendarWidget::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
{
   //qDebug()<<"come in paintCell"<<endl;
   QCalendarWidget::paintCell(painter, rect, date);
}
