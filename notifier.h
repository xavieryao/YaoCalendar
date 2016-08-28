#ifndef OSXNOTIFIER_H
#define OSXNOTIFIER_H
#include <QObject>
#include <QDateTime>

class OSXNotifier : public QObject {
public:
    OSXNotifier(QObject* parent = 0) : QObject(parent) {

    }

    void scheduleNoticition(const QString& title = QString(), const QString& subtitle = QString(),
                            const QString& informativeText = QString(), QDateTime time = QDateTime());
    void clear();
};

#endif // OSXNOTIFIER_H
