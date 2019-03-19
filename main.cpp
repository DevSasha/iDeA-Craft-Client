#include <QApplication>
#include <QFile>
#include <QDir>
#include <QScopedPointer>
#include <QTextStream>
#include <QDateTime>
#include <QLoggingCategory>
#include "core.h"

// Умный указатель на файл логирования
QScopedPointer<QFile>   m_logFile;

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{

    QTextStream out(m_logFile.data()); // Открываем поток записи в файл
    out << QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss.zzz] "); // Записываем дату записи
    // По типу определяем, к какому уровню относится сообщение
    switch (type)
    {
    case QtInfoMsg:     out << "INF "; break;
    case QtDebugMsg:    out << "DBG "; break;
    case QtWarningMsg:  out << "WRN "; break;
    case QtCriticalMsg: out << "CRT "; break;
    case QtFatalMsg:    out << "FTL "; break;
    }
    // Записываем в вывод категорию сообщения и само сообщение
    out << context.category << ": "
        << msg << endl;
    out.flush();    // Очищаем буферизированные данные
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QApplication::setOrganizationName("DrSasha");
    //QApplication::setApplicationName("iDeA-Craft");

    // Устанавливаем файл логирования
    QDir logs = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/logs/";
    if(!logs.exists()) logs.mkpath(logs.path());
    m_logFile.reset(new QFile(logs.path() + "/latest.txt"));
    m_logFile.data()->open(QFile::Append | QFile::Text); // Открываем файл логирования
    qInstallMessageHandler(messageHandler); // Устанавливаем обработчик

    Core core(&a);
    return a.exec();
}


