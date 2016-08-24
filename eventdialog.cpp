#include "eventdialog.h"
#include <QVBoxLayout>
#include <QTextEdit>
#include <QFont>
#include <QColorDialog>

EventDialog::EventDialog(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* rootLayout = new QVBoxLayout(this);

    mTitleLayout = new QHBoxLayout(this);
    mTitleWidget = new QWidget(this);
    mTitleEdit = new QLineEdit(mTitleWidget);
    mTitleEdit->setStyleSheet("QLineEdit{font-size: 20px;}");
    mTitleEdit->setMinimumHeight(25);

    mColorBtn = new ColorButton(mTitleWidget);
    mColorBtn->setColor(QColor::fromRgb(153, 151,244));

    mTitleLayout->addWidget(mTitleEdit);
    mTitleLayout->addWidget(mColorBtn);
    mTitleWidget->setLayout(mTitleLayout);
    mLocationEdit = new QLineEdit(this);
    mDescriptionEdit = new QTextEdit(this);

    rootLayout->addWidget(mTitleWidget);
    rootLayout->addWidget(mLocationEdit);
    rootLayout->addWidget(mDescriptionEdit);

    mTitleEdit->setPlaceholderText(tr("New Event"));
    mLocationEdit->setPlaceholderText(tr("Add Location"));
    mDescriptionEdit->setPlaceholderText(tr("Add note, URL or file."));

    connect(mColorBtn, &ColorButton::clicked, this, &EventDialog::chooseColor);

    this->setLayout(rootLayout);
}

void EventDialog::setEvent(const CalendarEvent event) {
    this->mEvent = event;
    mTitleEdit->setText(event.eventName());
    mLocationEdit->setText(event.location());
    mDescriptionEdit->setText(event.detail());
}

void EventDialog::chooseColor() {
    QColor newColor = QColorDialog::getColor(mColorBtn->color());
    this->mEvent.setColor(newColor);
    mColorBtn->setColor(newColor);
}
