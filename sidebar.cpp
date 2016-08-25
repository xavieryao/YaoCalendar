#include "sidebar.h"
#include <QList>
#include <QLabel>
#include <QListWidgetItem>
#include <QFrame>

SideBar::SideBar(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* rootLayout = new QVBoxLayout(parent);

    mTextBrowser = new QTextBrowser(this);
    mList = new QListWidget(this);
    rootLayout->addWidget(mTextBrowser,3);
    rootLayout->addWidget(mList,7);

    this->setLayout(rootLayout);

    connect(mList, &QListWidget::itemActivated, this, &SideBar::eventActivated);
}

void SideBar::updateEventList(const QDate &date) {
    mList->clear();
    QList<CalendarEvent> eventList = mEventMap->value(date);
    if (eventList.empty()) {
        return;
    }
    for (int i = 0; i < eventList.size(); i++) {
        CalendarEvent event = eventList.at(i);
        QListWidgetItem* item = new QListWidgetItem();
        QWidget *itemWidget = new QWidget(this);
        QLabel* textLabel = new QLabel(itemWidget);
        QLabel* detailLabel = new QLabel(itemWidget);
        detailLabel->setWordWrap(true);

        textLabel->setText(QString("<b>%1</b>").arg(event.eventName()));
        detailLabel->setText(tr("From:%1<br />To:%2<br />%3")
                           .arg(event.startDateTime().toString())
                           .arg(event.endDateTime().toString(), event.location()));
        item->setWhatsThis(event.detail());

        QVBoxLayout* itemLayout = new QVBoxLayout(itemWidget);
        itemLayout->addWidget(textLabel);
        itemLayout->addWidget(detailLabel);

        QFrame* line = new QFrame();
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        itemLayout->addWidget(line);

        itemWidget->setLayout(itemLayout);
        item->setSizeHint(QSize(item->sizeHint().width(), itemWidget->sizeHint().height()));
        item->setData(Qt::UserRole, QVariant::fromValue(event));
        mList->addItem(item);
        mList->setItemWidget(item, itemWidget);
    }
}

void SideBar::eventActivated(QListWidgetItem *item) {
    CalendarEvent e = item->data(Qt::UserRole).value<CalendarEvent>();
    emit editEvent(e);
}
