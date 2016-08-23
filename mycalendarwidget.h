#ifndef MYCALENDARWIDGET_H
#define MYCALENDARWIDGET_H

#include <QCalendarWidget>
#include <QVBoxLayout>

class MyCalendarWidget : public QCalendarWidget
{
public:
    explicit MyCalendarWidget(QWidget* parent = 0);

protected:
    virtual void paintCell(QPainter * painter, const QRect & rect, const QDate & date) const;
};

#endif // MYCALENDARWIDGET_H
