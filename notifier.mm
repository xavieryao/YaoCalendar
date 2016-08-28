#include "notifier.h"
#include <Foundation/NSUserNotification.h>
#include <Foundation/NSDate.h>
#include <QDebug>

void OSXNotifier::scheduleNoticition(const QString &title, const QString &subtitle,
                                     const QString &informativeText, QDateTime time)
{
    NSUserNotification *userNotification = [[[NSUserNotification alloc] init] autorelease];
    userNotification.title = title.toNSString();
    userNotification.subtitle = subtitle.toNSString();
    userNotification.informativeText = informativeText.toNSString();


    if (time == QDateTime()) {
        [[NSUserNotificationCenter defaultUserNotificationCenter] deliverNotification:userNotification];
    } else {
        userNotification.deliveryDate = [NSDate dateWithTimeIntervalSince1970: time.toTime_t()];
        [[NSUserNotificationCenter defaultUserNotificationCenter] scheduleNotification:userNotification];
    }
}

void OSXNotifier::clear()
{
    NSArray<NSUserNotification *> *notifications = [[NSUserNotificationCenter defaultUserNotificationCenter] scheduledNotifications];
    for (id obj in notifications) {
        [[NSUserNotificationCenter defaultUserNotificationCenter] removeScheduledNotification:obj];
    }

}
