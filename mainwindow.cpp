#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) :
    QMainWindow(parent, flags),
    ui(new Ui::MainWindow)
{

    mSettings = new QSettings("./config.plist", QSettings::IniFormat,
                              this);

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

    QStringList userList = mSettings->value("users").toStringList();
    if (userList.size() == 0) {
        userList.append("default");
        mSettings->setValue("users", userList);
    }

    if (!mSettings->contains("currentUser")) {
        mSettings->setValue("currentUser", userList.first());
    }
    QString currentUser = mSettings->value("currentUser", "default").toString();
    mEventStorage->setUserName(currentUser);
    mEventStorage->loadFromFile();
    mEventMap = mEventStorage->createEventMap();

    ui->calendarWidget->setEventMap(mEventMap);
    mSideBar->setEventMap(mEventMap);

    //    this->setWindowFlags(this->windowFlags() | Qt::WindowTransparentForInput);

    connect(ui->calendarWidget, &MyCalendarWidget::clicked, mSideBar, &SideBar::updateEventList);
    connect(mSideBar, &SideBar::editEvent, this, &MainWindow::openEventWindow);
    connect(mSideBar, &SideBar::deleteEvent, this, &MainWindow::onDeleteEvent);

    mSideBar->updateEventList(ui->calendarWidget->selectedDate());
    configureMultiUser(userList);
    configureShortcuts();
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
        MainWindow* mainWindow = new MainWindow(0, Qt::WindowTransparentForInput
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

void MainWindow::onDateActivated(const QDate &date, QString file, QString name) {
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

    if (file != QString()) {
        QFileInfo info(file);
        event.setEventName(info.fileName());
        event.setAttachment(CopyHelper::store(file, name));
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
            ui->calendarWidget->update();
            mEventStorage->saveToFile();
            return;
        } else {
            EventMap m = origEvent.expandToMap();
            EventMapHelper::splitMap(*mEventMap, m);
            ui->calendarWidget->update();
            mEventStorage->modifyEvent(event);
        }

    } else {
        mEventStorage->createEvent(event);
    }
    EventMap map = event.expandToMap();
    EventMapHelper::mergeMap(*mEventMap, map);
    ui->calendarWidget->update();
    mSideBar->updateEventList(ui->calendarWidget->selectedDate());
    mEventStorage->saveToFile();

}

void MainWindow::openEventWindow(CalendarEvent event, bool newEvent) {
    EventDialog* dialog = new EventDialog();
    dialog->setEvent(event, newEvent);
    connect(dialog, &EventDialog::confirmedEventChange, this, &MainWindow::onEventModified);
    dialog->exec();
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
        ui->calendarWidget->update();
        mSideBar->updateEventList(ui->calendarWidget->selectedDate());
        mEventStorage->saveToFile();

        return;
    } else {
        EventMap m = event.expandToMap();
        EventMapHelper::splitMap(*mEventMap, m);
        ui->calendarWidget->update();
        mEventStorage->removeEvent(event);
    }
    ui->calendarWidget->update();
    mSideBar->updateEventList(ui->calendarWidget->selectedDate());
    mEventStorage->saveToFile();
}

void MainWindow::configureMultiUser(QStringList& userList)
{
#ifdef Q_OS_OSX
    if (this->windowFlags() & Qt::WindowTransparentForInput) {
        return;
    }
#endif
    QMenuBar* bar = new QMenuBar(this);
    setMenuBar(bar);
    mMenuUser = menuBar()->addMenu(tr("&User"));
    QString currentUser = mSettings->value("currentUser").toString();
    for (auto user : userList) {
        QAction* action = new QAction(user, this);
        action->setData(user);
        action->setCheckable(true);
        if (user == currentUser) {
            action->setChecked(true);
        }
        connect(action, &QAction::triggered, this, &MainWindow::onUserChanged);
        mMenuUser->addAction(action);

        qDebug() << "add";
    }
    mMenuUser->addSeparator();
    QAction* addUser = new QAction(tr("&Add User"), this);
    connect(addUser, &QAction::triggered, this, &MainWindow::addUser);
    mMenuUser->addAction(addUser);

    QAction* editUser = new QAction(tr("&Edit User"), this);
    connect(editUser, &QAction::triggered, this, &MainWindow::editUser);
    mMenuUser->addAction(editUser);

    mMenuUser->addSeparator();
    QAction* exportEvents = new QAction(tr("E&xport Events..."), this);
    QAction* importEvents = new QAction(tr("&Import Events..."), this);
    connect(exportEvents, &QAction::triggered, this, &MainWindow::exportEvents);
    connect(importEvents, &QAction::triggered, this, &MainWindow::importEvents);

    mMenuUser->addAction(exportEvents);
    mMenuUser->addAction(importEvents);

    // configure settigns menu
    QMenu* settings = menuBar()->addMenu(tr("&Preferences"));
    QAction* droppable = new QAction(tr("Drop File"), this);
    droppable->setCheckable(true);
    droppable->setChecked(mSettings->value("droppable", true).toBool());
    connect(droppable, &QAction::triggered, [=]{
        mSettings->setValue("droppable", droppable->isChecked());
    });
    settings->addAction(droppable);
}

void MainWindow::onUserChanged()
{
    QAction* action = qobject_cast<QAction*>(sender());
    action->setChecked(true);
    QString currentUser = action->data().toString();
    qDebug() << "change to " <<  currentUser;

    QList<QAction* >actions = mMenuUser->actions();
    for (auto act : actions) {
        if (action != act) {
            act->setChecked(false);
        }
    }

    mSettings->setValue("currentUser", currentUser);
    delete mEventStorage;
    delete mEventMap;
    mEventStorage = new EventStorage();
    mEventStorage->setUserName(currentUser);
    mEventStorage->loadFromFile();
    mEventMap = mEventStorage->createEventMap();
    ui->calendarWidget->setEventMap(mEventMap);
    ui->calendarWidget->update();
    mSideBar->setEventMap(mEventMap);
    mSideBar->updateEventList(ui->calendarWidget->selectedDate());
}

void MainWindow::addUser()
{
    QString userName = QInputDialog::getText(this, tr("Add User"), "New User Name:");
    QStringList userList = mSettings->value("users").toStringList();
    if (userList.contains(userName)) {
        QMessageBox::warning(this, tr("Naming Collision"), tr("There is already a user with the same name."));
        return;
    }
    userList.append(userName);
    mSettings->setValue("users", userList);
    configureMultiUser(userList);
}

void MainWindow::editUser()
{
    QString current = mSettings->value("currentUser").toString();
    if (current == "default") {
        QMessageBox::warning(this, tr("Can Not Modify Default"), tr("You cannnot modify the default user."));
        return;
    }
    QString userName = QInputDialog::getText(this, tr("Add User"), "New User Name:", QLineEdit::Normal,
                                             current);
    QStringList userList = mSettings->value("users").toStringList();
    if (userName == current) {
        return;
    }
    if (userList.contains(userName)) {
        QMessageBox::warning(this, tr("Naming Collision"), tr("There is already a user with the same name."));
        return;
    }

    userList.removeAll(current);
    userList.append(userName);
    mSettings->setValue("users", userList);
    mSettings->setValue("currentUser", userName);
    configureMultiUser(userList);
}

void MainWindow::exportEvents()
{
    QString saveFile = QFileDialog::getSaveFileName(this, tr("Export to..."),
                                                    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+"/exported.json",
                                                    "JSON (*.json)");
    qDebug() << saveFile;
    if (saveFile.isEmpty()) {
        return;
    }
    mEventStorage->saveToFile(saveFile);
}

void MainWindow::importEvents()
{
    QString saveFile = QFileDialog::getOpenFileName(this, tr("Import from..."),
                                                    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                    "JSON (*.json)");
    if (saveFile.isEmpty()) {
        return;
    }
    EventStorage* storage = new EventStorage;
    bool result = storage->loadFromFile(saveFile);
    if (result &&
            QMessageBox::Yes == QMessageBox::question(this, tr("Confirm Importing Events"),
                                                      tr("Import all events from user %1").arg(storage->userName()))) {
        mEventStorage->merge(storage);
        mEventMap = mEventStorage->createEventMap();
        ui->calendarWidget->setEventMap(mEventMap);
        ui->calendarWidget->update();
        mSideBar->setEventMap(mEventMap);
        mSideBar->updateEventList(ui->calendarWidget->selectedDate());
    }

}

void MainWindow::configureShortcuts()
{
    QMap<QString, QVariant> map = mSettings->value("shortcuts").toMap();
    // DEBUG
    map.insert("Ctrl+O", QVariant::fromValue(ShortcutActions::DELETE));
    map.insert("Ctrl+P", QVariant::fromValue(ShortcutActions::NEW));

    QList<QString> keys = map.keys();
    for(int i = 0; i < keys.size(); i++) {
        QString key = keys.at(i);
        ShortcutActions action = map.value(key).value<ShortcutActions>();
            QShortcut* shortcut = new QShortcut(QKeySequence(key), this);
        switch (action) {
        case ShortcutActions::NEW:
            qDebug() << "new shortcut";
            connect(shortcut, &QShortcut::activated, [=]{
                qDebug() << "new clicked";
                onDateActivated(ui->calendarWidget->selectedDate());
            });
            break;
        case ShortcutActions::DELETE:
            connect(shortcut, &QShortcut::activated, [=]{
                if (mSideBar->hasSelected()) {
                    CalendarEvent e = mSideBar->selectedEvent();
                    onDeleteEvent(e);
                }
            });
            break;
        default:

            break;
        }
    }



}
