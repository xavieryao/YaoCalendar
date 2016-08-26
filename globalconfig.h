#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

#include <QJsonDocument>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QList>

class GlobalConfig
{
public:
    GlobalConfig();
    QList<QString> getUserList();
    void addUser(const QString name);
    void removeUser(const QString name);
    void save();
    void load();
private:
    QList<QString> mUserList;
};

#endif // GLOBALCONFIG_H
