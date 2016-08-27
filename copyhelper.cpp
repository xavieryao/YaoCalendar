#include "copyhelper.h"
#include "qtfilecopier.h"
#include "wavewidget.h"
#include <cstdlib>
#include <QDebug>
#include <QDir>
#include <ctime>

CopyHelper::CopyHelper()
{

}

QString CopyHelper::store(const QString& source, const QString& fileName)
{
    QDir::current().mkdir("files");
    QString dest = QString("%1/files/%2_%3").arg(QDir::currentPath(), randStr(), fileName);
    QtFileCopier copier;

    copier.copy(source, dest);
    return dest;
}

void CopyHelper::get(const QString& dest, const QString& from)
{

}

QString CopyHelper::randStr(int digits)
{
    srand(time(NULL));
    QString str;
    for (int i = 0; i < digits; i++) {
        str.append(QChar('A' + rand()%26));
    }
    return str;
}
