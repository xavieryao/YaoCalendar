#include "mycalendarwidget.h"
#include <QDebug>
#include <QTextCharFormat>
#include <QBrush>
#include <QColor>
#include <QPainter>
#include <QPoint>
#include <QSettings>
#include <QUrl>
#include <QDate>
#include <QMimeData>
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

    setAcceptDrops(true);

    connect(this, &QCalendarWidget::activated, [=](const QDate& date){emit newEvent(date);});
}

void MyCalendarWidget::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
{
    (const_cast<MyCalendarWidget *>(this))->mRectDateList.append(QPair<QRect, QDate>(rect, date));
    // Draw cell background
    painter->save();
    QBrush backgroundBrush(Qt::SolidPattern);
    if (date == this->selectedDate()) {
        backgroundBrush.setColor(QColor::fromRgb(254,223,194));
    } else if (date.dayOfWeek() > Qt::DayOfWeek::Friday) {
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

    if (!mEventMap->value(date).empty()) {
//        qDebug() << "have event(s) on " << date << rect;

        // Draw a triangle event indicator.
        QPoint topRightCorner = rect.topRight();
        QPoint topLeftPoint(rect.right() - (rect.right() - rect.left())*0.2,
                            topRightCorner.y());
        QPoint rightPoint(topRightCorner.x(), rect.top() + (rect.right() - rect.left())*0.2);

        QPainterPath path;
        path.moveTo(topRightCorner);

        path.lineTo(topLeftPoint);
        path.lineTo(rightPoint);
        path.lineTo(topRightCorner);

        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
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

void MyCalendarWidget::dragEnterEvent(QDragEnterEvent *e)
{
    QSettings settings("./config.ini", QSettings::IniFormat,
                                  this);
    if (e->mimeData()->hasUrls() && e->mimeData()->urls().size() == 1 && settings.value("droppable", true).toBool()) {
        e->acceptProposedAction();
    }

}

void MyCalendarWidget::dropEvent(QDropEvent *e)
{
    QDate date;
    for (auto rect: mRectDateList) {
        if (rect.first.contains(e->pos())) {
            date = rect.second;
        }
    }
    if (date == QDate()) {
        return;
    }
    e->acceptProposedAction();
    qDebug() << "drop on " << date;
    emit newEvent(date, e->mimeData()->urls().first().toLocalFile(), e->mimeData()->urls().first().fileName());
}

void MyCalendarWidget::resizeEvent(QResizeEvent *event)
{
    mRectDateList.clear();
}
