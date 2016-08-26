#include "globalconfig.h"

GlobalConfig::GlobalConfig()
{

}

QList<QString> GlobalConfig::getUserList()
{
    return mUserList;
}

void GlobalConfig::addUser(const QString name)
{
    mUserList.append(name);
}

void GlobalConfig::removeUser(const QString name)
{
    mUserList.removeAll(name);
}

void GlobalConfig::save()
{
    QJsonObject obj;
    obj["version"] = "1.0.0";
    obj["description"] = "global config file for YaoCalendar";
    QJsonArray arr;
    for (QString user: mUserList) {
        arr.append(user);
    }
    obj["users"] = arr;
}

void GlobalConfig::load()
{

}
