#include "sidebar.h"
#include <QList>
#include <QLabel>
#include <QListWidgetItem>
#include <QFrame>
#include <QDebug>
#include <QMenu>
#include <QAction>

SideBar::SideBar(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* rootLayout = new QVBoxLayout(parent);

//    mTextBrowser = new QTextBrowser(this);
    mList = new QListWidget(this);
    mList->setContextMenuPolicy(Qt::CustomContextMenu);
    QPalette p = mList->palette();
    p.setColor(QPalette::Highlight, QColor::fromRgb(254,223,194));
    mList->setPalette(p);
    mList->setSelectionMode(QAbstractItemView::SingleSelection);
    mList->setDragEnabled(true);
    mList->viewport()->setAcceptDrops(true);
    mList->setDropIndicatorShown(true);
    mList->setDragDropMode(QAbstractItemView::InternalMove);
//    rootLayout->addWidget(mTextBrowser,3);
    rootLayout->addWidget(mList,7);

    this->setLayout(rootLayout);

    connect(mList, &QListWidget::itemActivated, this, &SideBar::eventActivated);
    connect(mList, &QListWidget::customContextMenuRequested, this, &SideBar::showContextMenu);

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
        item->setData(Qt::UserRole, QVariant::fromValue(event));
        QWidget *itemWidget = new QWidget(this);
        QWidget *title = new QWidget(this);
        QHBoxLayout *titleLayout = new QHBoxLayout(this);
        title->setLayout(titleLayout);

        QLabel* color = new QLabel(itemWidget);
        QLabel* textLabel = new QLabel(itemWidget);
        QLabel* detailLabel = new QLabel(itemWidget);
        textLabel->setAlignment(Qt::AlignLeft);
        detailLabel->setWordWrap(true);

        titleLayout->addWidget(color);
        titleLayout->addWidget(textLabel);
        QFont font;
        font.setFamily("FontAwesome");
        color->setFont(font);
        color->setStyleSheet(QString("color:rgb(%1,%2,%3)").arg(event.color().red())
                             .arg(event.color().green())
                             .arg(event.color().blue()));
        color->setText(QChar(0xf0c8));


        textLabel->setText(QString("<b>%1</b>").arg(event.eventName()));
        detailLabel->setText(formatDescription(event));
        item->setWhatsThis(event.detail());

        QVBoxLayout* itemLayout = new QVBoxLayout(itemWidget);
        itemLayout->addWidget(title);
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


QString SideBar::formatDescription(CalendarEvent& e) const{
    if (e.isAllDayEvent()) {
        QString formatAllDay = tr("All Day<br /><u>From:</u>%1<br /><u>To:</u>%2<br />%3");
        return formatAllDay.arg(e.startDateTime().date().toString())
                .arg(e.endDateTime().date().toString())
                .arg(e.location());
    }
    QString formatDefault = tr("<u>From:</u>%1<br /><u>To:</u>%2<br />%3");

    return formatDefault.arg(e.startDateTime().toString())
            .arg(e.endDateTime().toString())
            .arg(e.location());
}

void SideBar::showContextMenu(const QPoint &pos)
{
    QMenu menu;
    QAction* action = new QAction(tr("Delete"), this);
    menu.addAction(action);
    connect(action, &QAction::triggered, [=]{
        QVariant var = mList->selectedItems().first()->data(Qt::UserRole);
        CalendarEvent e = var.value<CalendarEvent>();
        emit deleteEvent(e);
    });
    menu.exec(this->mapToGlobal(pos));
}
