#include "mycalendarwidget.h"
#include <QDebug>
#include <QTextCharFormat>
#include <QBrush>
#include <QColor>
#include <QPainter>
#include <QPoint>
#include <QPainterPath>

MyCalendarWidget::MyCalendarWidget(QWidget* parent) : QCalendarWidget(parent)
{
    QTextCharFormat weekdayFormat, weekendFormat;
    weekdayFormat = this->weekdayTextFormat(Qt::DayOfWeek::Sunday);
    weekendFormat = this->weekdayTextFormat(Qt::DayOfWeek::Monday);
    weekendFormat.setForeground(QBrush(QColor::fromRgb(168, 168, 168)));
    weekendFormat.setBackground(QBrush(QColor::fromRgb(255,255,255)));
    weekdayFormat.setForeground(QBrush(Qt::black));
    weekdayFormat.setBackground(QBrush(QColor::fromRgb(255,255,255)));
    for (int i = 1; i <=5; i++) {
        this->setWeekdayTextFormat(Qt::DayOfWeek(i), weekdayFormat);
    }

    this->setWeekdayTextFormat(Qt::DayOfWeek::Saturday, weekendFormat);
    this->setWeekdayTextFormat(Qt::DayOfWeek::Sunday, weekendFormat);
    this->setVerticalHeaderFormat(MyCalendarWidget::NoVerticalHeader);
}

void MyCalendarWidget::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
{
    // Draw cell background
    painter->save();
    QBrush backgroundBrush(Qt::SolidPattern);
    if (date.dayOfWeek() > Qt::DayOfWeek::Friday) {
        backgroundBrush.setColor(QColor::fromRgb(0xf2, 0xf2, 0xf2));
    } else {
        backgroundBrush.setColor(Qt::white);
    }
    painter->setPen(Qt::NoPen);
    painter->setBrush(backgroundBrush);
    painter->drawRect(rect);
    painter->restore();

    // Draw cell outline
    painter->save();
    painter->setPen(QPen(QColor::fromRgb(223,224,224), 1));
    painter->drawRect(rect);
    painter->restore();

    if (mEventMap->contains(date)) {
//        qDebug() << "have event(s) on " << date << rect;

        // Draw a triangle event indicator.
        QPoint topRightCorner = rect.topRight();
        QPoint topLeftPoint(rect.right() - (rect.right() - rect.left())*0.2,
                            topRightCorner.y());
        QPoint rightPoint(topRightCorner.x(), rect.top() + (rect.bottom() - rect.top())*0.2);

        QPainterPath path;
        path.moveTo(topRightCorner);

        path.lineTo(topLeftPoint);
        path.lineTo(rightPoint);
        path.lineTo(topRightCorner);

        painter->save();
        painter->setPen(Qt::NoPen);
        painter->fillPath(path, QBrush(QColor::fromRgb(251, 32, 37)));
        painter->restore();
    }

    // Draw day text.
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    if (date.month() != this->monthShown()) {
        painter->setPen(QColor::fromRgb(177, 177, 177));
    } else if (date.dayOfWeek() > Qt::DayOfWeek::Friday) {
        painter->setPen(QColor::fromRgb(116,115,131));
    } else {
        painter->setPen(QColor::fromRgb(35,38,39));
    }
    painter->setBackgroundMode(Qt::BGMode::TransparentMode);
    painter->drawText(rect, Qt::AlignHCenter, QString::number(date.day()));
    painter->restore();
}
