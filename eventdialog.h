#ifndef EVENTDIALOG_H
#define EVENTDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QObject>
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
#include <QTimeEdit>
#include <QUrl>
#include <QLabel>
#include "calendarevent.h"
#include "dndtextedit.h"
#include "attachmentwidget.h"

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

class RepeatWidget : public QWidget{
    Q_OBJECT
public:
    explicit RepeatWidget(QWidget* parent = 0) : QWidget(parent) {
        QFormLayout* layout = new QFormLayout(this);
        endLabel = new QLabel(tr("Until:"), this);
        endDateEdit = new QDateEdit(this);
        layout->addRow(endLabel, endDateEdit);
        this->setLayout(layout);
    }

    QLabel* endLabel;
    QDateEdit* endDateEdit;
};

class EventDialog : public QDialog
{
    Q_OBJECT
public:
    explicit EventDialog(QWidget *parent = 0);
    void setEvent(CalendarEvent event, bool isNew = true);
signals:
    void eventDateTimeChanged(CalendarEvent& event);
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
    DnDTextEdit* mDescriptionEdit;
    QDateTimeEdit* mStartDateTime;
    QDateTimeEdit* mEndDateTime;
    QCheckBox* mAllDay;
    QDateEdit* mStartDate;
    QDateEdit* mEndDate;
    QFormLayout* mFormLayout;
    QLabel* mStartLabel;
    QLabel* mEndLabel;
    QComboBox* mRepeat;
    RepeatWidget* mRepeatWidget;
    AttachmentWidget* mAttachment;

    QWidget* setUpRepeatCombo();
    QWidget* setUpDurationWidget();
    QWidget* setUpButtonWidget();
    void setUpAttachmentWidget();
    void configureUiFromEvent();

private slots:
    void onAllDayChanged(int state);
    void onStartDateTimeChanged(const QDateTime &datetime);
    void onEndDateTimeChanged(const QDateTime &datetime);
    void onStartDateChanged(const QDate &date);
    void onEndDateChanged(const QDate &date);
    void onEventDateTimeChanged(CalendarEvent& event);
    void onOkClicked();
    void chooseColor();
    void onFileDropped(const QUrl &url);
    void onRepeatModeChanged(int index);
};



#endif // EVENTDIALOG_H
