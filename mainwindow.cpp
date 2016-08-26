#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <QFont>
#include <QResizeEvent>
#include <QTime>
#include <QFile>
#include <QMessageBox>
#include <QAbstractButton>
#include <QDir>
#include <QJsonDocument>
#include <QFileInfo>
#include <QMenu>
#include <QSystemTrayIcon>
#include "eventdialog.h"
#include "eventmaphelper.h"

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

    mEventStorage = new EventStorage;

    mEventStorage->loadFromFile();
    mEventMap = mEventStorage->createEventMap();

    ui->calendarWidget->setEventMap(mEventMap);
    mSideBar->setEventMap(mEventMap);

//    this->setWindowFlags(this->windowFlags() | Qt::WindowTransparentForInput);

    connect(ui->calendarWidget, &MyCalendarWidget::clicked, mSideBar, &SideBar::updateEventList);
    connect(mSideBar, &SideBar::editEvent, this, &MainWindow::openEventWindow);
    connect(mSideBar, &SideBar::deleteEvent, this, &MainWindow::onDeleteEvent);

    mSideBar->updateEventList(ui->calendarWidget->selectedDate());
    configureMultiUser();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mEventMap;
    delete mEventStorage;
}

void MainWindow::setUpCalendarNavigator() {
    connect(ui->calendarWidget, &MyCalendarWidget::currentPageChanged, this,
            &MainWindow::formatAndSetMonthLabel);
    connect(ui->calendarWidget, &MyCalendarWidget::newEvent, this,
            &MainWindow::onDateActivated);
    QWidget* toolBar = new QWidget(this);

    QPushButton* pinButton = new QPushButton(this);
    QPushButton* prevButton = new QPushButton(this);
    QPushButton* todayButton = new QPushButton(tr("Today"), this);
    QPushButton* nextButton = new QPushButton(this);

    QHBoxLayout* toolBarLayout = new QHBoxLayout(this);
    toolBarLayout->setSpacing(2);
    toolBarLayout->addWidget(pinButton);
    toolBarLayout->addWidget(prevButton);
    toolBarLayout->addWidget(todayButton);
    toolBarLayout->addWidget(nextButton);

    toolBar->setLayout(toolBarLayout);
    ui->horizontalLayout->addWidget(toolBar);

    QFile qssFile(":/stylesheet/global");
    qssFile.open(QFile::ReadOnly);
    if (qssFile.isOpen()) {
        QString qss = QString(qssFile.readAll());
        ui->monthLabel->setStyleSheet(qss);
        toolBar->setStyleSheet(qss);
        qssFile.close();
    }

    formatAndSetMonthLabel(ui->calendarWidget->yearShown(),
                           ui->calendarWidget->monthShown());
    QFont font;
    font.setFamily("FontAwesome");
    pinButton->setFont(font);
    pinButton->setText(QChar(0xf276));
    prevButton->setFont(font);
    prevButton->setText(QChar(0xf104));
    nextButton->setFont(font);
    nextButton->setText(QChar(0xf105));

    connect(todayButton, &QPushButton::clicked, [=]{ui->calendarWidget->showToday(); ui->calendarWidget->setSelectedDate(QDate::currentDate());});
    connect(prevButton, &QPushButton::clicked, ui->calendarWidget, &MyCalendarWidget::showPreviousMonth);
    connect(nextButton, &QPushButton::clicked, ui->calendarWidget, &MyCalendarWidget::showNextMonth);
    connect(pinButton, &QPushButton::clicked, [=]{
//        this->centralWidget()->setAttribute(Qt::WA_TransparentForMouseEvents);
        this->setWindowOpacity(0.8);
#ifdef Q_OS_OSX
        MainWindow* mainWindow = new MainWindow;
        mainWindow->setWindowFlags(mainWindow->windowFlags() | Qt::WindowTransparentForInput
                                   | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
        mainWindow->move(this->pos().x(), this->pos().y());
        mainWindow->resize(this->size());
        mainWindow->setSelection(ui->calendarWidget->selectedDate());
        mainWindow->setWindowOpacity(0.8);
        mainWindow->show();
        mainWindow->showTrayIcon();
        this->close();
#else
        Qt::WindowFlags flag = windowFlags();
        flag = flag | Qt::WindowTransparentForInput | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint;
        this->setWindowFlags(flag);
        show();
        showTrayIcon();
#endif
    });
}

void MainWindow::formatAndSetMonthLabel(int year, int month) {
    ui->monthLabel->setText(tr("%1 - %2").arg(year).arg(month));
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    mSideBar->setVisible(event->size().width() >= MIN_WIDTH_WITH_SIDEBAR);
}

void MainWindow::onDateActivated(const QDate &date, QString file) {
    // new event
    CalendarEvent event = CalendarEvent::newInstance();
    QDateTime startTime, endTime;
    QTime currentTime = QTime::currentTime();
    currentTime.setHMS(currentTime.hour(), currentTime.minute()-currentTime.minute()%15, 0);
    startTime.setDate(date);
    endTime.setDate(date);
    startTime.setTime(currentTime);
    endTime.setTime(currentTime.addSecs(60*120));
    event.setStartTime(startTime);
    event.setEndTime(endTime);
    event.setAttachment(file);

    if (file != QString()) {
        QFileInfo info(file);
        event.setEventName(info.fileName());
    }

    openEventWindow(event, true);

}

void MainWindow::onEventModified(const CalendarEvent origEvent, CalendarEvent event, bool isNew) {
    if (!isNew) {
        if (origEvent.repeatMode() != RepeatMode::NONE) {
            QMessageBox box;
            box.setText(tr("Repeating Event"));
            box.setInformativeText(tr("How do you want to modify a repeating event?"));
            QAbstractButton *allButton = box.addButton(tr("This And Later"), QMessageBox::YesRole);
            QAbstractButton *thisButton = box.addButton(tr("Only This"), QMessageBox::NoRole);

            box.setIcon(QMessageBox::Question);
            box.exec();

            CalendarEvent primary = mEventStorage->findPrimaryEvent(event);
            EventMap m = primary.expandToMap();
            EventMapHelper::splitMap(*mEventMap, m);
            qDebug() << "remove primary";
            mEventStorage->removeEvent(primary);
            if (origEvent.startDateTime().date() > primary.startDateTime().date()) {
                CalendarEvent prevE = CalendarEvent::newInstance(primary);
                QDateTime t = event.startDateTime();
                prevE.setRepeatEndDate(event.startDateTime().date().addDays(-1));
                EventMap prevMap = prevE.expandToMap();
                EventMapHelper::mergeMap(*mEventMap, prevMap);

                mEventStorage->createEvent(prevE);
            }
            if (origEvent.startDateTime().date() < primary.repeatEndDate() && box.clickedButton() == thisButton) {
                event.setRepeatMode(RepeatMode::NONE);
                qDebug() << "future";
                CalendarEvent futureE = CalendarEvent::newInstance(primary);
                futureE.setStartTime(event.startDateTime().addDays(1));
                futureE.setEndTime(event.endDateTime().addDays(1));
                EventMap futureMap = futureE.expandToMap();
                EventMapHelper::mergeMap(*mEventMap, futureMap);
                mSideBar->updateEventList(ui->calendarWidget->selectedDate());
                mEventStorage->createEvent(futureE);
            }
            mEventStorage->createEvent(event);
            EventMap map = event.expandToMap();
            EventMapHelper::mergeMap(*mEventMap, map);
            mSideBar->updateEventList(ui->calendarWidget->selectedDate());
            mEventStorage->saveToFile();
            return;
        } else {
            EventMap m = origEvent.expandToMap();
            EventMapHelper::splitMap(*mEventMap, m);
            mEventStorage->modifyEvent(event);
        }

    } else {
        mEventStorage->createEvent(event);
    }
    EventMap map = event.expandToMap();
    EventMapHelper::mergeMap(*mEventMap, map);
    mSideBar->updateEventList(ui->calendarWidget->selectedDate());
    mEventStorage->saveToFile();

}

void MainWindow::openEventWindow(CalendarEvent event, bool newEvent) {
    EventDialog* dialog = new EventDialog();
    dialog->setEvent(event, newEvent);
    connect(dialog, &EventDialog::confirmedEventChange, this, &MainWindow::onEventModified);
    dialog->exec();
}

void MainWindow::mousePressEvent(QMouseEvent *e) {
    qDebug() << "mouse move";
}

void MainWindow::setSelection(const QDate& date)
{
    ui->calendarWidget->setSelectedDate(date);
}

void MainWindow::showTrayIcon()
{
    QSystemTrayIcon* trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/icon/pin"));
    QMenu* menu = new QMenu(this);
    QAction* showAction = new QAction(tr("&Unpin"),this);
    menu->addAction(showAction);
    trayIcon->setContextMenu(menu);
    trayIcon->show();
    connect(showAction, &QAction::triggered, [=]{
#ifdef Q_OS_OSX
        MainWindow* mainWindow = new MainWindow;
        mainWindow->move(this->pos().x(), this->pos().y());
        mainWindow->resize(this->size());
        mainWindow->setSelection(ui->calendarWidget->selectedDate());
        mainWindow->show();
        mainWindow->raise();
        this->close();
#else
        trayIcon->hide();
        setWindowOpacity(1);
        Qt::WindowFlags flag = windowFlags();
        flag = flag & !Qt::WindowTransparentForInput;
        flag = flag & !Qt::WindowStaysOnTopHint;
        flag = flag & !Qt::FramelessWindowHint;
//        flag = flag | Qt::WindowTransparentForInput | Qt::WindowStaysOnTopHint;
        this->setWindowFlags(flag);
        show();
        raise();
#endif
        trayIcon->deleteLater();

    });
}

void MainWindow::onDeleteEvent(CalendarEvent &event)
{
    if (event.repeatMode() != RepeatMode::NONE) {
        QMessageBox box;
        box.setText(tr("Repeating Event"));
        box.setInformativeText(tr("How do you want to modify a repeating event?"));
        QAbstractButton *allButton = box.addButton(tr("This And Later"), QMessageBox::YesRole);
        QAbstractButton *thisButton = box.addButton(tr("Only This"), QMessageBox::NoRole);

        box.setIcon(QMessageBox::Question);
        box.exec();

        CalendarEvent primary = mEventStorage->findPrimaryEvent(event);
        EventMap m = primary.expandToMap();
        EventMapHelper::splitMap(*mEventMap, m);
        qDebug() << "remove primary";
        mEventStorage->removeEvent(primary);
        if (event.startDateTime().date() > primary.startDateTime().date()) {
            CalendarEvent prevE = CalendarEvent::newInstance(primary);
            QDateTime t = event.startDateTime();
            prevE.setRepeatEndDate(event.startDateTime().date().addDays(-1));
            EventMap prevMap = prevE.expandToMap();
            EventMapHelper::mergeMap(*mEventMap, prevMap);

            mEventStorage->createEvent(prevE);
        }
        if (event.startDateTime().date() < primary.repeatEndDate() && box.clickedButton() == thisButton) {
            qDebug() << "future";
            CalendarEvent futureE = CalendarEvent::newInstance(primary);
            futureE.setStartTime(event.startDateTime().addDays(1));
            futureE.setEndTime(event.endDateTime().addDays(1));
            EventMap futureMap = futureE.expandToMap();
            EventMapHelper::mergeMap(*mEventMap, futureMap);
            mSideBar->updateEventList(ui->calendarWidget->selectedDate());
            mEventStorage->createEvent(futureE);
        }
        mEventStorage->removeEvent(event);
        mSideBar->updateEventList(ui->calendarWidget->selectedDate());
        mEventStorage->saveToFile();
        return;
    } else {
        EventMap m = event.expandToMap();
        EventMapHelper::splitMap(*mEventMap, m);
        mEventStorage->removeEvent(event);
    }
    mSideBar->updateEventList(ui->calendarWidget->selectedDate());
    mEventStorage->saveToFile();
}

void MainWindow::configureMultiUser()
{

}
