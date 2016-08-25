#ifndef EVENTDIALOG_H
#define EVENTDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QColor>
#include <QCheckBox>
#include <QFormLayout>
#include <QDateEdit>
#include <QComboBox>
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

class EventDialog : public QDialog
{
    Q_OBJECT
public:
    explicit EventDialog(QWidget *parent = 0);
    void setEvent(CalendarEvent event, bool isNew = true);
signals:
    void eventChanged(CalendarEvent& event);
    void confirmedEventChange(CalendarEvent origEvent, CalendarEvent event, bool isNewEvent);
public slots:
private:
    CalendarEvent mEvent;
    CalendarEvent mOrigEvent;
    bool mIsNew;

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
    QComboBox* mRepeat;

private slots:
    void onAllDayChanged(int state);
    void onStartDateTimeChanged(const QDateTime &datetime);
    void onEndDateTimeChanged(const QDateTime &datetime);
    void onStartDateChanged(const QDate &date);
    void onEndDateChanged(const QDate &date);
    void onEventChanged(CalendarEvent& event);
    void onOkClicked();
    void chooseColor();
};



#endif // EVENTDIALOG_H
