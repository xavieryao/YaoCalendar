#include "eventdialog.h"
#include <QVBoxLayout>
#include <QTextEdit>
#include <QFont>
#include <QFormLayout>
#include <QColorDialog>
#include <QDateTimeEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QSpacerItem>

EventDialog::EventDialog(QWidget *parent) : QDialog(parent)
{
    QVBoxLayout* rootLayout = new QVBoxLayout(this);

    mTitleLayout = new QHBoxLayout(this);
    mTitleWidget = new QWidget(this);
    mTitleEdit = new QLineEdit(mTitleWidget);
    mTitleEdit->setStyleSheet("QLineEdit{font-size: 20px;}");
    mTitleEdit->setMinimumHeight(25);

    mColorBtn = new ColorButton(mTitleWidget);
    mColorBtn->setColor(QColor::fromRgb(153, 151,244));

    QWidget* durationWidget = new QWidget(this);

    mStartLabel = new QLabel(tr("Start:"), durationWidget);
    mEndLabel = new QLabel(tr("End:"), durationWidget);

    mFormLayout = new QFormLayout(durationWidget);
    mStartDateTime = new QDateTimeEdit(durationWidget);
    mEndDateTime = new QDateTimeEdit(durationWidget);
    mStartDate = new QDateEdit(durationWidget);
    mEndDate = new QDateEdit(durationWidget);
    mStartDate->setVisible(false);
    mEndDate->setVisible(false);

    mAllDay = new QCheckBox(durationWidget);
    mAllDay->setChecked(false);

    mFormLayout->addRow(new QLabel(tr("All Day:"), durationWidget), mAllDay);
    mFormLayout->addRow(mStartLabel, mStartDateTime);
    mFormLayout->addRow(mEndLabel, mEndDateTime);

    durationWidget->setLayout(mFormLayout);

    mTitleLayout->addWidget(mTitleEdit);
    mTitleLayout->addWidget(mColorBtn);
    mTitleWidget->setLayout(mTitleLayout);
    mLocationEdit = new QLineEdit(this);
    mDescriptionEdit = new QTextEdit(this);

    QWidget* buttons = new QWidget(this);
    QHBoxLayout* btnLayout = new QHBoxLayout(buttons);
    QPushButton* btnOk = new QPushButton(tr("Ok"), this);
    QPushButton* btnCancel = new QPushButton(tr("Cancel"), this);
    btnOk->setDefault(true);
    btnLayout->addStretch();
    btnLayout->addWidget(btnCancel);
    btnLayout->addWidget(btnOk);
    buttons->setLayout(btnLayout);

    rootLayout->addWidget(mTitleWidget);
    rootLayout->addWidget(mLocationEdit);
    rootLayout->addWidget(durationWidget);
    rootLayout->addWidget(mDescriptionEdit);
    rootLayout->addWidget(buttons);

    mTitleEdit->setPlaceholderText(tr("New Event"));
    mLocationEdit->setPlaceholderText(tr("Add Location"));
    mDescriptionEdit->setPlaceholderText(tr("Add note, URL or file."));

    connect(mColorBtn, &ColorButton::clicked, this, &EventDialog::chooseColor);
    connect(this->mAllDay, &QCheckBox::stateChanged, this, &EventDialog::onAllDayChanged);

    this->setLayout(rootLayout);

    connect(mStartDate, &QDateTimeEdit::dateChanged, this, &EventDialog::onStartDateChanged);
    connect(mStartDateTime, &QDateTimeEdit::dateTimeChanged, this, &EventDialog::onStartDateTimeChanged);
    connect(mEndDate, &QDateTimeEdit::dateChanged, this, &EventDialog::onEndDateChanged);
    connect(mEndDateTime, &QDateTimeEdit::dateTimeChanged, this, &EventDialog::onEndDateTimeChanged);

    connect(this, &EventDialog::eventChanged, this, &EventDialog::onEventChanged);

    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(btnOk, &QPushButton::clicked, this, &EventDialog::onOkClicked);
}

void EventDialog::setEvent(CalendarEvent event) {
    this->mEvent = event;
    emit eventChanged(this->mEvent);
}

void EventDialog::chooseColor() {
    QColor newColor = QColorDialog::getColor(mColorBtn->color());
    this->mEvent.setColor(newColor);
    mColorBtn->setColor(newColor);
}

void EventDialog::onAllDayChanged(int state) {
    if (state == Qt::CheckState::Checked) {

        mStartDateTime->setVisible(false);
        mEndDateTime->setVisible(false);
        mStartLabel->setVisible(false);
        mEndLabel->setVisible(false);

        mFormLayout->removeWidget(mStartLabel);
        mFormLayout->removeWidget(mStartDateTime);
        mFormLayout->removeWidget(mEndLabel);
        mFormLayout->removeWidget(mEndDateTime);

        mStartDate->setVisible(true);
        mEndDate->setVisible(true);
        mStartLabel->setVisible(true);
        mEndLabel->setVisible(true);

        mFormLayout->addRow(mStartLabel, mStartDate);
        mFormLayout->addRow(mEndLabel, mEndDate);
    } else {
        mStartDate->setVisible(false);
        mEndDate->setVisible(false);
        mStartLabel->setVisible(false);
        mEndLabel->setVisible(false);

        mFormLayout->removeWidget(mStartLabel);
        mFormLayout->removeWidget(mStartDate);
        mFormLayout->removeWidget(mEndLabel);
        mFormLayout->removeWidget(mEndDate);


        mStartDateTime->setVisible(true);
        mEndDateTime->setVisible(true);
        mStartLabel->setVisible(true);
        mEndLabel->setVisible(true);

        mFormLayout->addRow(mStartLabel, mStartDateTime);
        mFormLayout->addRow(mEndLabel, mEndDateTime);
    }
}

void EventDialog::onEventChanged(CalendarEvent& event) {
    mTitleEdit->setText(event.eventName());
    mLocationEdit->setText(event.location());
    mDescriptionEdit->setText(event.detail());
    mStartDateTime->setDateTime(event.startTime());
    mEndDateTime->setDateTime(event.endTime());
    mStartDate->setDate(event.startTime().date());
    mEndDate->setDate(event.endTime().date());
}

void EventDialog::onStartDateTimeChanged(const QDateTime &datetime) {
    this->mEvent.setStartTime(datetime);
    emit eventChanged(this->mEvent);
}

void EventDialog::onEndDateTimeChanged(const QDateTime &datetime) {
    this->mEvent.setEndTime(datetime);
    emit eventChanged(this->mEvent);
}

void EventDialog::onStartDateChanged(const QDate &date) {
    QDateTime newDateTime = this->mEvent.startTime();
    newDateTime.setDate(date);
    this->mEvent.setStartTime(newDateTime);
    emit eventChanged(this->mEvent);
}

void EventDialog::onEndDateChanged(const QDate &date) {
    QDateTime newDateTime = this->mEvent.endTime();
    newDateTime.setDate(date);
    this->mEvent.setEndTime(newDateTime);
    emit eventChanged(this->mEvent);
}

void EventDialog::onOkClicked() {
    emit confirmedEventChange(this->mEvent);
    this->done(QDialog::Accepted);
}
