#ifndef EVENTDIALOG_H
#define EVENTDIALOG_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QColor>
#include <QCheckBox>
#include <QFormLayout>
#include <QDateEdit>
#include <QLabel>
#include "calendarevent.h"

class ColorButton : public QPushButton {
    Q_OBJECT
public:
    explicit ColorButton(QWidget* parent = 0) : QPushButton(parent) {
        QFont font;
        font.setFamily("FontAwesome");
        this->setFont(font);
        this->setText(QChar(0xf0c8));
    }

    void setColor(const QColor color) {
        this->mColor = color;
        this->setStyleSheet(QString("QPushButton{color: rgb(%1, %2, %3);}")
            .arg(color.red()).arg(color.green()).arg(color.blue()));
    }
    QColor color() const {
        return mColor;
    }
private:
    QColor mColor;
};

class EventDialog : public QWidget
{
    Q_OBJECT
public:
    explicit EventDialog(QWidget *parent = 0);
    void setEvent(CalendarEvent* event);
signals:
    void eventChanged(CalendarEvent* event);
public slots:
    void chooseColor();
private:
    CalendarEvent* mEvent;

    QVBoxLayout* mRootLayout;
    QWidget* mTitleWidget;
    QHBoxLayout* mTitleLayout;
    ColorButton* mColorBtn;
    QLineEdit* mTitleEdit;
    QLineEdit* mLocationEdit;
    QTextEdit* mDescriptionEdit;
    QDateTimeEdit* mStartDateTime;
    QDateTimeEdit* mEndDateTime;
    QCheckBox* mAllDay;
    QDateEdit* mStartDate;
    QDateEdit* mEndDate;
    QFormLayout* mFormLayout;
    QLabel* mStartLabel;
    QLabel* mEndLabel;

private slots:
    void onAllDayChanged(int state);
    void onStartDateTimeChanged(const QDateTime &datetime);
    void onEndDateTimeChanged(const QDateTime &datetime);
    void onStartDateChanged(const QDate &date);
    void onEndDateChanged(const QDate &date);
    void onEventChanged(CalendarEvent* event);
};



#endif // EVENTDIALOG_H
