#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <QFont>
#include <QResizeEvent>
#include <QTime>
#include "eventdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QFontDatabase::addApplicationFont(":Font/fontawesome");
    QFontDatabase::addApplicationFont(":Font/nevis");
    ui->setupUi(this);

    ui->calendarWidget->setNavigationBarVisible(false);
    setUpCalendarNavigator();

    mSideBar = new SideBar(this);
    mSideBar->setFixedWidth(SIDE_BAR_WIDTH);
    ui->centralHorizontalLayout->addWidget(mSideBar);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(ui->centralHorizontalLayout);
    setCentralWidget(centralWidget);

    if (this->size().width() < this->MIN_WIDTH_WITH_SIDEBAR) {
        mSideBar->setVisible(false);
    }

    mEventMap = new EventMap();

    // DEBUG
    QDate today(2016,8,23);
    QList<CalendarEvent> events;
    CalendarEvent event1;
    event1.setEventName("Eat");
    event1.setStartTime(QDateTime(QDate(2016,8,20), QTime(1,1,1)));
    event1.setEndTime(QDateTime(QDate(2016,8,21), QTime(5,5,5)));
    event1.setRepeatMode(RepeatMode::PER_DAY_OF_WEEK);
    event1.setRepeatEndDate(QDate(2016,9,13));

    EventMap map = event1.expandToMap();
    qDebug() << "sadfcasd";

    for (auto key: map.keys()) {
        qDebug() << key;
        QList<CalendarEvent> list = mEventMap->value(key);
        QList<CalendarEvent> ll = map.value(key);
        for(int i = 0; i < ll.size(); i++) {
            CalendarEvent e = ll.at(i);
            list.append(ll.at(i));
        }
        mEventMap->insert(key, list);
    }
    qDebug() << "asdf";
    // END DEBUG
    ui->calendarWidget->setEventMap(mEventMap);
    mSideBar->setEventMap(mEventMap);

    connect(ui->calendarWidget, &MyCalendarWidget::clicked, mSideBar, &SideBar::updateEventList);
    connect(mSideBar, &SideBar::editEvent, this, &MainWindow::openEventWindow);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mEventMap;
}

void MainWindow::setUpCalendarNavigator() {
    connect(ui->calendarWidget, &MyCalendarWidget::currentPageChanged, this,
            &MainWindow::formatAndSetMonthLabel);
    connect(ui->calendarWidget, &MyCalendarWidget::activated, this,
            &MainWindow::onDateActivated);

    QFile qssFile(":/stylesheet/global");
    qssFile.open(QFile::ReadOnly);
    if (qssFile.isOpen()) {
        QString qss = QString(qssFile.readAll());
        ui->monthLabel->setStyleSheet(qss);
        qssFile.close();
    }

    formatAndSetMonthLabel(ui->calendarWidget->yearShown(),
                           ui->calendarWidget->monthShown());
    QFont font;
    font.setFamily("FontAwesome");
    ui->prevButton->setFont(font);
    ui->prevButton->setText(QChar(0xf104));
    ui->nextButton->setFont(font);
    ui->nextButton->setText(QChar(0xf105));

    connect(ui->prevButton, &QPushButton::clicked, ui->calendarWidget, &MyCalendarWidget::showPreviousMonth);
    connect(ui->nextButton, &QPushButton::clicked, ui->calendarWidget, &MyCalendarWidget::showNextMonth);
    connect(ui->todayButton, &QPushButton::clicked, [=]{ui->calendarWidget->showToday(); ui->calendarWidget->setSelectedDate(QDate::currentDate());});
}

void MainWindow::formatAndSetMonthLabel(int year, int month) {
    ui->monthLabel->setText(tr("%1 - %2").arg(year).arg(month));
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    mSideBar->setVisible(event->size().width() >= MIN_WIDTH_WITH_SIDEBAR);
}

void MainWindow::onDateActivated(const QDate &date) {
    // new event
    CalendarEvent* event = CalendarEvent::newInstance();
    QDateTime startTime, endTime;
    QTime currentTime = QTime::currentTime();
    currentTime.setHMS(currentTime.hour(), currentTime.minute()-currentTime.minute()%15, 0);
    startTime.setDate(date);
    endTime.setDate(date);
    startTime.setTime(currentTime);
    endTime.setTime(currentTime.addSecs(60*120));
    event->setStartTime(startTime);
    event->setEndTime(endTime);

    openEventWindow(*event, true);

}

void MainWindow::onEventModified(const CalendarEvent origEvent, CalendarEvent event, bool isNew) {
    if (!isNew) {
        QList<QDate> expandedOrigDate = origEvent.expandDateFromRepeat();
        for (QDate date: expandedOrigDate) {
            QList<CalendarEvent> list = mEventMap->value(date);
            list.removeOne(origEvent);
            mEventMap->insert(date, list);
        }
    }
    QList<QDate> expandedDate = event.expandDateFromRepeat();
    for (QDate date: expandedDate) {
        QList<CalendarEvent> list = mEventMap->value(date);
        list.append(event);
        mEventMap->insert(date, list);
    }

    // TODO Store!
    mSideBar->updateEventList(ui->calendarWidget->selectedDate());

}

void MainWindow::openEventWindow(CalendarEvent event, bool newEvent) {
    EventDialog* dialog = new EventDialog();
    dialog->setEvent(event, newEvent);
    connect(dialog, &EventDialog::confirmedEventChange, this, &MainWindow::onEventModified);
    dialog->exec();
}
