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

    mTitleLayout->addWidget(mTitleEdit);
    mTitleLayout->addWidget(mColorBtn);
    mTitleWidget->setLayout(mTitleLayout);
    mLocationEdit = new QLineEdit(this);
    mDescriptionEdit = new QTextEdit(this);

    mTitleEdit->setPlaceholderText(tr("New Event"));
    mLocationEdit->setPlaceholderText(tr("Add Location"));
    mDescriptionEdit->setPlaceholderText(tr("Add note, URL or file."));

    mRepeatWidget = new RepeatWidget(this);

    rootLayout->addWidget(mTitleWidget);
    rootLayout->addWidget(mLocationEdit);
    rootLayout->addWidget(setUpDurationWidget());
    rootLayout->addWidget(setUpRepeatCombo());
    rootLayout->addWidget(mRepeatWidget);
    rootLayout->addWidget(mDescriptionEdit);
    rootLayout->addWidget(setUpButtonWidget());

    connect(mColorBtn, &ColorButton::clicked, this, &EventDialog::chooseColor);
    connect(this->mAllDay, &QCheckBox::stateChanged, this, &EventDialog::onAllDayChanged);

    this->setLayout(rootLayout);

    connect(mStartDate, &QDateTimeEdit::dateChanged, this, &EventDialog::onStartDateChanged);
    connect(mStartDateTime, &QDateTimeEdit::dateTimeChanged, this, &EventDialog::onStartDateTimeChanged);
    connect(mEndDate, &QDateTimeEdit::dateChanged, this, &EventDialog::onEndDateChanged);
    connect(mEndDateTime, &QDateTimeEdit::dateTimeChanged, this, &EventDialog::onEndDateTimeChanged);

    connect(this, &EventDialog::eventChanged, this, &EventDialog::onEventChanged);


}

QWidget* EventDialog::setUpDurationWidget() {
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
    return durationWidget;
}

QWidget* EventDialog::setUpButtonWidget() {
    QWidget* buttons = new QWidget(this);
    QHBoxLayout* btnLayout = new QHBoxLayout(buttons);
    QPushButton* btnOk = new QPushButton(tr("Ok"), this);
    QPushButton* btnCancel = new QPushButton(tr("Cancel"), this);
    btnOk->setDefault(true);
    btnLayout->addStretch();
    btnLayout->addWidget(btnCancel);
    btnLayout->addWidget(btnOk);
    buttons->setLayout(btnLayout);

    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(btnOk, &QPushButton::clicked, this, &EventDialog::onOkClicked);

    return buttons;
}

QWidget* EventDialog::setUpRepeatCombo() {
    QWidget* widget = new QWidget(this);
    QFormLayout* layout = new QFormLayout(widget);
    widget->setLayout(layout);
    mRepeat = new QComboBox(widget);
    mRepeat->addItem(tr("Don\'t Repeat"), QVariant::fromValue(RepeatMode::NONE));
    mRepeat->addItem(tr("Per Day"), QVariant::fromValue(RepeatMode::PER_DAY));
    mRepeat->addItem(tr("Per Week"), QVariant::fromValue(RepeatMode::PER_DAY_OF_WEEK));
    mRepeat->addItem(tr("Per Month"), QVariant::fromValue(RepeatMode::PER_MONTH));
    mRepeat->addItem(tr("Per Year"), QVariant::fromValue(RepeatMode::PER_YEAR));
    layout->addRow(new QLabel(tr("Repeat:")), mRepeat);

    connect(mRepeat, static_cast<void (QComboBox:: *)(int index)>(&QComboBox::currentIndexChanged)
            , this, &EventDialog::onRepeatModeChanged);
    return widget;
}

void EventDialog::setEvent(CalendarEvent event, bool isNew) {
    this->mEvent = event;
    this->mOrigEvent = event;
    this->mIsNew = isNew;
    emit eventChanged(this->mEvent);
}

void EventDialog::chooseColor() {
    QColor newColor = QColorDialog::getColor(mColorBtn->color());
    this->mEvent.setColor(newColor);
    emit eventChanged(this->mEvent);
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
    // set text
    mTitleEdit->setText(event.eventName());
    mLocationEdit->setText(event.location());
    mDescriptionEdit->setText(event.detail());

    // set duration
    mStartDateTime->setDateTime(event.startDateTime());
    mEndDateTime->setDateTime(event.endDateTime());
    mStartDate->setDate(event.startDateTime().date());
    mEndDate->setDate(event.endDateTime().date());

    // set repeat
    mRepeat->setCurrentIndex(event.repeatMode());
    if (event.repeatMode() == RepeatMode::NONE) {
        mRepeatWidget->setVisible(false);
    } else {
        mRepeatWidget->setVisible(true);
    }
    if (event.repeatEndDate() == QDate()) {
        event.setRepeatEndDate(event.startDateTime().date());
    }
    if (event.repeatStartDate() == QDate()) {
        event.setRepeatStartDate(event.startDateTime().date());
    }
    mRepeatWidget->startDateEdit->setDate(event.repeatStartDate());
    mRepeatWidget->endDateEdit->setDate(event.repeatEndDate());

    // set color
    mColorBtn->setColor(event.color());
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
    QDateTime newDateTime = this->mEvent.startDateTime();
    newDateTime.setDate(date);
    this->mEvent.setStartTime(newDateTime);
    emit eventChanged(this->mEvent);
}

void EventDialog::onEndDateChanged(const QDate &date) {
    QDateTime newDateTime = this->mEvent.endDateTime();
    newDateTime.setDate(date);
    this->mEvent.setEndTime(newDateTime);
    emit eventChanged(this->mEvent);
}

void EventDialog::onOkClicked() {
    mEvent.setIsAllDayEvent(mAllDay->checkState() == Qt::Checked);
    mEvent.setEventName(mTitleEdit->text());
    mEvent.setDetail(mDescriptionEdit->toHtml());
    mEvent.setLocation(mLocationEdit->text());

    emit confirmedEventChange(this->mOrigEvent, this->mEvent, this->mIsNew);
    this->done(QDialog::Accepted);
}

void EventDialog::onRepeatModeChanged(int index) {
    mEvent.setRepeatMode(RepeatMode(index));
    emit eventChanged(this->mEvent);
}
