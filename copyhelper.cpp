#include "copyhelper.h"
#include "wavewidget.h"
#include <cstdlib>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <ctime>

CopyHelper::CopyHelper(QObject* parent) : QObject(parent)
{

}

QString CopyHelper::store(const QString& source, const QString& fileName)
{
    QDir::current().mkdir("files");
    QString dest = QString("%1/files/%2_%3").arg(QDir::currentPath(), randStr(), fileName);
//    QtFileCopier* copier = new QtFileCopier(this);
//    copier->setProgressInterval(1);
    qint64 full = QFile(source).size();
    qDebug() << "size:" << full;
    wave = new WaveWidget;
    wave->show();

//    copier->copy(source, dest);

    return dest;
}

void CopyHelper::get(const QString& dest, const QString& from)
{

}

void CopyHelper::progressed(long identifier, qint64 progress){
    qDebug() << "pro";

    wave->setProgress(((qreal)progress)/full);
}

void CopyHelper::done()
{
    qDebug() << "done";
    wave->setProgress(1);
    wave->close();
    wave->deleteLater();
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
