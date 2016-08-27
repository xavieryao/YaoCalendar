#ifndef TYPES_H
#define TYPES_H
#include "calendarevent.h"
typedef QMap<QDate, QList<CalendarEvent> > EventMap;


enum ShortcutAction {
    NEW, DELETE, NEXT_MONTH, PREV_MONTH, TODAY, PIN
};

Q_DECLARE_METATYPE(ShortcutAction)
#endif // TYPES_H
