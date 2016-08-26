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
    QJsonDocument doc(obj);

    QFile saveFile("config.json");
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open config file.");
        return;
    }
    saveFile.write(doc.toJson());
    saveFile.close();
}

void GlobalConfig::load()
{
    QFile saveFile("config.json");
    if (!saveFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open config file.");
        return;
    }

    QByteArray saveData = saveFile.readAll();
    saveFile.close();

    QJsonDocument loadDoc = QJsonDocument::fromJson(saveData);
    QJsonArray userArr = loadDoc.object()["users"].toArray();
    mUserList.clear();
    for (auto val : userArr) {
        mUserList.append(val.toString());
    }

}
