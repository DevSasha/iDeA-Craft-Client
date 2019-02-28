#include "instanse.h"

Instanse::Instanse(QString nameInst, QObject *parent) : QObject(parent)
{
    QSettings set("DrSasha", "iDeA-Craft Launcher");
    set.beginGroup("network");
    urlRepo = "http://" + set.value("host").toString() + "/repository/mc/";
    set.endGroup();
    instName = nameInst;
    instanses = new QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "\\instanses");
    instDir = new QDir(instanses->path() + "\\" + instName);
    if(!instanses->exists()) instanses->mkdir(instanses->path());
    //checkInstallation();
    manager = new QNetworkAccessManager;
    connect(manager, &QNetworkAccessManager::finished, this, &Instanse::takeMetadata);
    QNetworkRequest request;    // Отправляемый запрос
    request.setUrl(urlRepo + nameInst + "/sha256s.json");   // Устанавлвиваем URL в запрос
    manager->get(request);      // Выполняем запрос
}

Instanse::~Instanse()
{
    delete manager;
}

void Instanse::takeFile(QNetworkReply *reply)
{
    // Если в процесе получения данных произошла ошибка
    if(reply->error()){
        // Сообщаем об этом и показываем информацию об ошибках
        qDebug() << "ERROR:" << reply->errorString();
    } else {
        emit processUpdate(downloadedFiles * 100 / allFile);
        // В противном случае создаём объект для работы с файлом
        QFile file(instanses->path() + "\\" + curDownloadingName);
        qDebug() << "Get file" << file.fileName(); // << instanses->path() + "/" + curDownloadingName;
        // Создаём файл или открываем его на перезапись ...
        if(file.open(QFile::WriteOnly)){
            file.write(reply->readAll());  // ... и записываем всю информацию со страницы в файл
            file.close();                  // закрываем файл
        }
        //emit onReady(); // Посылаем сигнал о завершении получения файла
        if(downloadedFiles < allFile)
            getFile();
        else
            checkInstallation("fast");
    }
}

void Instanse::getFile()
{
    ++downloadedFiles;
    if(filetypes.first() == "dir"){
        QDir dir(instanses->path() + "\\" + filenames.first());
        qDebug() << "Make dir" << dir.path();
        if(!dir.exists()) dir.mkdir(dir.path());
        filenames.pop_front();
        filesizes.pop_front();
        filetypes.pop_front();
        cheksums.pop_front();
        getFile();
    }else {
        curDownloadingName = filenames.first();
        curDownloadingSize = filesizes.first();
        curDownloadingSum = cheksums.first();
        filenames.pop_front();
        filesizes.pop_front();
        filetypes.pop_front();
        cheksums.pop_front();
        QNetworkRequest request;                        // Отправляемый запрос
        QUrl url(urlRepo + curDownloadingName);
        QString url2 = QString(url.toEncoded(QUrl::EncodeUnicode));
        request.setUrl(url2);   // Устанавлвиваем URL в запрос
        manager->get(request);  // Выполняем запрос
    }
}

void Instanse::takeMetadata(QNetworkReply *reply)
{
    qDebug() << "Metadata"; // << reply->readLine();
    // Если в процесе получения данных произошла ошибка
    if(reply->error()){
        qDebug() << "ERROR:" << reply->errorString(); // Сообщаем об этом и показываем информацию об ошибках
    } else {
        disconnect(manager, &QNetworkAccessManager::finished, this, &Instanse::takeMetadata);
        connect(manager, &QNetworkAccessManager::finished, this, &Instanse::takeFile);

        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject root(doc.object());
        if(!root.value("size").isNull()){
            allFile = root.value("size").toInt();
            if(root.value("data").isArray()){
                QJsonArray arr = root.value("data").toArray();
                for(int i = 0;i < allFile;++i){
                    if(arr[i].isObject()){
                        QJsonObject file = arr[i].toObject();
                        if(file.value("file").isString() && !file.value("size").isNull() && file.value("hash").isString() && file.value("type").isString()){
                            filenames.push_back(file.value("file").toString());
                            filesizes.push_back(file.value("size").toInt());
                            cheksums.push_back(file.value("hash").toString());
                            filetypes.push_back(file.value("type").toString());
                        }
                    }else {
                        qDebug() << "Value is not object";
                        return;
                    }
                }
            }else {
                qDebug() << "Value is not array";
                return;
            }
        }else {
            qDebug() << "Size not stated";
            return;
        }
        qDebug() << "Downloading metadata is completed";
        if(!checkInstallation("fast"))
            getFile();
        //emit onReady(); // Посылаем сигнал о завершении получения файла
    }
}

bool Instanse::checkInstallation(QString mode)
{
    if(mode == "fast"){
        if(!instDir->exists()){
            instDir->mkdir(instDir->path());
            installed = false;
            return false;
        }else {
            installed = true;
            emit downloadComplete();
            return true;
        }
    }
    return false;
}
