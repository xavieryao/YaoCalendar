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
    CalendarEvent event1, event2, event3;
    event1.setEventName("Eat");
    event2.setEventName("Drink");
    event3.setEventName("Litter");
    events.append(event1);
    events.append(event2);
    events.append(event3);

    mEventMap->insert(today, events);

    // END DEBUG
    ui->calendarWidget->setEventMap(mEventMap);
    mSideBar->setEventMap(mEventMap);

    connect(ui->calendarWidget, &MyCalendarWidget::clicked, mSideBar, &SideBar::updateEventList);
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
    CalendarEvent event;

    EventDialog* dialog = new EventDialog();
    QList<CalendarEvent> eventList = this->mEventMap->value(date);
    if (!eventList.empty()) {
        event = eventList.at(0);
    } else {
        QDateTime startTime, endTime;
        QTime currentTime = QTime::currentTime();
        currentTime.setHMS(currentTime.hour(), currentTime.minute()-currentTime.minute()%15, 0);
        startTime.setDate(date);
        endTime.setDate(date);
        startTime.setTime(currentTime);
        endTime.setTime(currentTime.addSecs(60*120));
        event.setStartTime(startTime);
        event.setEndTime(endTime);
    }
    dialog->setEvent(event);
    connect(dialog, &EventDialog::confirmedEventChange, this, &MainWindow::onEventModified);
    dialog->exec();
}

void MainWindow::onEventModified(CalendarEvent event) {
    QList<CalendarEvent> list = mEventMap->value(event.startDateTime().date());
    list.append(event);
    mEventMap->insert(event.startDateTime().date(), list);
}
