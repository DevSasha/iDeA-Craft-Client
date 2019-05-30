#include "instanse.h"

Instanse::Instanse(QString nameInst, QObject *parent) : QObject(parent)
{
    urlRepo = "http://mc.idea-craft.space/" + nameInst + "/";
    instanses = new QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "\\instanses");
    instDir = new QDir(instanses->path() + "\\" + nameInst);
    if(!instanses->exists()) instanses->mkdir(instanses->path());
    checkInstallation("fast");
    manager = new QNetworkAccessManager;
    connect(manager, &QNetworkAccessManager::finished, this, &Instanse::takeMetadata);
    QNetworkRequest request;    // Отправляемый запрос
    request.setUrl(urlRepo + "meta.json");   // Устанавлвиваем URL в запрос
    manager->get(request);      // Выполняем запрос
}

Instanse::~Instanse()
{
    delete manager;
}

void Instanse::download()
{
    if(!checkInstallation("fast")){
        if(this->infoUpdate){
            qDebug() << "Start download";
            connect(manager, &QNetworkAccessManager::finished, this, &Instanse::takeSHA256);
            QNetworkRequest request;    // Отправляемый запрос
            request.setUrl(urlRepo + "sha256s.json");   // Устанавлвиваем URL в запрос
            manager->get(request);      // Выполняем запрос
            //emit startDownload();
        }else {
            connect(this, &Instanse::infoUpdated, this, &Instanse::startDownload);
        }
    }else {
        emit downloadComplete();
    }
}

void Instanse::run(QString nik)
{
    if(installed){
        nikname = nik;
        this->startRun();
    }else {
        download();
    }
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
        QFile file(instanses->path() + "/" + curDownloadingName);
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
        QUrl url("http://mc.idea-craft.space/" + curDownloadingName);
        QString url2 = QString(url.toEncoded(QUrl::EncodeUnicode));
        request.setUrl(url2);   // Устанавлвиваем URL в запрос
        manager->get(request);  // Выполняем запрос
        // wget();
    }
}

void Instanse::takeMetadata(QNetworkReply *reply)
{
    disconnect(manager, &QNetworkAccessManager::finished, this, &Instanse::takeMetadata);
    if(reply->error()){
        qDebug() << "ERROR:" << reply->errorString(); // Сообщаем об этом и показываем информацию об ошибках
    } else {
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject root(doc.object());

        if(root.value("name").isString()){
            this->instanseName = root.value("name").toString();
        }else {
            QMessageBox::warning(nullptr, "Downloadig error", "Can`t download");
            return;
        }

        if(root.value("version").isString()){
            this->instanseVersion = root.value("version").toString();
        }else {
            QMessageBox::warning(nullptr, "Downloadig error", "Can`t download");
            return;
        }

        if(root.value("MCversion").isString()){
            this->instanseMC = root.value("MCversion").toString();
        }else {
            QMessageBox::warning(nullptr, "Downloadig error", "Can`t download");
            return;
        }

        if(root.value("Forge").isString()){
            this->instanseForge = root.value("Forge").toString();
        }else {
            QMessageBox::warning(nullptr, "Downloadig error", "Can`t download");
            return;
        }
        infoUpdate = 1;
        emit infoUpdated(); // Посылаем сигнал о завершении получения файла
    }
}

void Instanse::takeSHA256(QNetworkReply *reply)
{
    qDebug() << "Metadata"; // << reply->readLine();
    // Если в процесе получения данных произошла ошибка
    if(reply->error()){
        qDebug() << "ERROR:" << reply->errorString(); // Сообщаем об этом и показываем информацию об ошибках
    } else {
        disconnect(manager, &QNetworkAccessManager::finished, this, &Instanse::takeSHA256);
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
                            qDebug() << file.value("file").toString() << "Added";
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
            if(!instDir->exists()){
                instDir->mkdir(instDir->path());
            getFile();
        }
        //emit onReady(); // Посылаем сигнал о завершении получения файла
    }
}

void Instanse::startDownload()
{
    qDebug() << "Start Download";
    connect(manager, &QNetworkAccessManager::finished, this, &Instanse::takeSHA256);
    QNetworkRequest request;    // Отправляемый запрос
    //request.setUrl(urlRepo + "meta.json");   // Устанавлвиваем URL в запрос
    request.setUrl(urlRepo + "sha256s.json");   // Устанавлвиваем URL в запрос
    manager->get(request);      // Выполняем запрос
}

void Instanse::startRun()
{
    qInfo() << "Start MC";
    QString CD = this->instDir->path();
    QList<QString> args = {
        "-Xmx768M",
        "-Dfile.encoding=UTF-8",
        "-Dfml.ignoreInvalidMinecraftCertificates=true",
        "-Dfml.ignorePatchDiscrepancies=true",
        "-Djava.net.useSystemProxies=true",
        "-XX:HeapDumpPath=MojangTricksIntelDriversForPerformance_javaw.exe_minecraft.exe.heapdump",
        "-Djava.library.path="+CD+"/versions/Forge " + this->instanseMC + "/natives",
        "-Dminecraft.launcher.brand=java-minecraft-launcher",
        "-Dminecraft.launcher.version=1.6.84-j",
        "-Dminecraft.client.jar="+CD+"/versions/Forge " + this->instanseMC + "/Forge 1.7.10.jar",
        "-cp",
         CD+"/libraries/net/minecraftforge/forge/1.7.10-10.13.4.1614-1.7.10/forge-1.7.10-10.13.4.1614-1.7.10.jar;"
        +CD+"/libraries/net/minecraft/launchwrapper/1.12/launchwrapper-1.12.jar;"
        +CD+"/libraries/org/ow2/asm/asm-all/5.0.3/asm-all-5.0.3.jar;"
        +CD+"/libraries/com/typesafe/akka/akka-actor_2.11/2.3.3/akka-actor_2.11-2.3.3.jar;"
        +CD+"/libraries/com/typesafe/config/1.2.1/config-1.2.1.jar;"
        +CD+"/libraries/org/scala-lang/scala-actors-migration_2.11/1.1.0/scala-actors-migration_2.11-1.1.0.jar;"
        +CD+"/libraries/org/scala-lang/scala-compiler/2.11.1/scala-compiler-2.11.1.jar;"
        +CD+"/libraries/org/scala-lang/plugins/scala-continuations-library_2.11/1.0.2/scala-continuations-library_2.11-1.0.2.jar;"
        +CD+"/libraries/org/scala-lang/plugins/scala-continuations-plugin_2.11.1/1.0.2/scala-continuations-plugin_2.11.1-1.0.2.jar;"
        +CD+"/libraries/org/scala-lang/scala-library/2.11.1/scala-library-2.11.1.jar;"
        +CD+"/libraries/org/scala-lang/scala-parser-combinators_2.11/1.0.1/scala-parser-combinators_2.11-1.0.1.jar;"
        +CD+"/libraries/org/scala-lang/scala-reflect/2.11.1/scala-reflect-2.11.1.jar;"
        +CD+"/libraries/org/scala-lang/scala-swing_2.11/1.0.1/scala-swing_2.11-1.0.1.jar;"
        +CD+"/libraries/org/scala-lang/scala-xml_2.11/1.0.2/scala-xml_2.11-1.0.2.jar;"
        +CD+"/libraries/lzma/lzma/0.0.1/lzma-0.0.1.jar;"
        +CD+"/libraries/net/sf/jopt-simple/jopt-simple/4.5/jopt-simple-4.5.jar;"
        +CD+"/libraries/com/google/guava/guava/17.0/guava-17.0.jar;"
        +CD+"/libraries/org/apache/commons/commons-lang3/3.3.2/commons-lang3-3.3.2.jar;"
        +CD+"/libraries/net/minecraftforge/forge/1.7.10-10.13.4.1614-1.7.10/forge-1.7.10-10.13.4.1614-1.7.10.jar;"
        +CD+"/libraries/net/minecraft/launchwrapper/1.12/launchwrapper-1.12.jar;"
        +CD+"/libraries/org/ow2/asm/asm-all/5.0.3/asm-all-5.0.3.jar;"
        +CD+"/libraries/com/typesafe/akka/akka-actor_2.11/2.3.3/akka-actor_2.11-2.3.3.jar;"
        +CD+"/libraries/com/typesafe/config/1.2.1/config-1.2.1.jar;"
        +CD+"/libraries/org/scala-lang/scala-actors-migration_2.11/1.1.0/scala-actors-migration_2.11-1.1.0.jar;"
        +CD+"/libraries/org/scala-lang/scala-compiler/2.11.1/scala-compiler-2.11.1.jar;"
        +CD+"/libraries/org/scala-lang/plugins/scala-continuations-library_2.11/1.0.2/scala-continuations-library_2.11-1.0.2.jar;"
        +CD+"/libraries/org/scala-lang/plugins/scala-continuations-plugin_2.11.1/1.0.2/scala-continuations-plugin_2.11.1-1.0.2.jar;"
        +CD+"/libraries/org/scala-lang/scala-library/2.11.1/scala-library-2.11.1.jar;"
        +CD+"/libraries/org/scala-lang/scala-parser-combinators_2.11/1.0.1/scala-parser-combinators_2.11-1.0.1.jar;"
        +CD+"/libraries/org/scala-lang/scala-reflect/2.11.1/scala-reflect-2.11.1.jar;"
        +CD+"/libraries/org/scala-lang/scala-swing_2.11/1.0.1/scala-swing_2.11-1.0.1.jar;"
        +CD+"/libraries/org/scala-lang/scala-xml_2.11/1.0.2/scala-xml_2.11-1.0.2.jar;"
        +CD+"/libraries/lzma/lzma/0.0.1/lzma-0.0.1.jar;"
        +CD+"/libraries/net/sf/jopt-simple/jopt-simple/4.5/jopt-simple-4.5.jar;"
        +CD+"/libraries/com/google/guava/guava/17.0/guava-17.0.jar;"
        +CD+"/libraries/org/apache/commons/commons-lang3/3.3.2/commons-lang3-3.3.2.jar;"
        +CD+"/libraries/com/mojang/netty/1.6/netty-1.6.jar;"
        +CD+"/libraries/com/mojang/realms/1.3.5/realms-1.3.5.jar;"
        +CD+"/libraries/org/apache/commons/commons-compress/1.8.1/commons-compress-1.8.1.jar;"
        +CD+"/libraries/org/apache/httpcomponents/httpclient/4.3.3/httpclient-4.3.3.jar;"
        +CD+"/libraries/commons-logging/commons-logging/1.1.3/commons-logging-1.1.3.jar;"
        +CD+"/libraries/org/apache/httpcomponents/httpcore/4.3.2/httpcore-4.3.2.jar;"
        +CD+"/libraries/java3d/vecmath/1.3.1/vecmath-1.3.1.jar;"
        +CD+"/libraries/net/sf/trove4j/trove4j/3.0.3/trove4j-3.0.3.jar;"
        +CD+"/libraries/com/ibm/icu/icu4j-core-mojang/51.2/icu4j-core-mojang-51.2.jar;"
        +CD+"/libraries/net/sf/jopt-simple/jopt-simple/4.5/jopt-simple-4.5.jar;"
        +CD+"/libraries/com/paulscode/codecjorbis/20101023/codecjorbis-20101023.jar;"
        +CD+"/libraries/com/paulscode/codecwav/20101023/codecwav-20101023.jar;"
        +CD+"/libraries/com/paulscode/libraryjavasound/20101123/libraryjavasound-20101123.jar;"
        +CD+"/libraries/com/paulscode/librarylwjglopenal/20100824/librarylwjglopenal-20100824.jar;"
        +CD+"/libraries/com/paulscode/soundsystem/20120107/soundsystem-20120107.jar;"
        +CD+"/libraries/io/netty/netty-all/4.0.10.Final/netty-all-4.0.10.Final.jar;"
        +CD+"/libraries/com/google/guava/guava/15.0/guava-15.0.jar;"
        +CD+"/libraries/org/apache/commons/commons-lang3/3.1/commons-lang3-3.1.jar;"
        +CD+"/libraries/commons-io/commons-io/2.4/commons-io-2.4.jar;"
        +CD+"/libraries/commons-codec/commons-codec/1.9/commons-codec-1.9.jar;"
        +CD+"/libraries/net/java/jinput/jinput/2.0.5/jinput-2.0.5.jar;"
        +CD+"/libraries/net/java/jutils/jutils/1.0.0/jutils-1.0.0.jar;"
        +CD+"/libraries/com/google/code/gson/gson/2.2.4/gson-2.2.4.jar;"
        +CD+"/libraries/com/mojang/authlib/1.5.21/authlib-1.5.21.jar;"
        +CD+"/libraries/org/apache/logging/log4j/log4j-api/2.0-beta9/log4j-api-2.0-beta9.jar;"
        +CD+"/libraries/org/apache/logging/log4j/log4j-core/2.0-beta9/log4j-core-2.0-beta9.jar;"
        +CD+"/libraries/org/lwjgl/lwjgl/lwjgl/2.9.1/lwjgl-2.9.1.jar;"
        +CD+"/libraries/org/lwjgl/lwjgl/lwjgl_util/2.9.1/lwjgl_util-2.9.1.jar;"
        +CD+"/libraries/tv/twitch/twitch/5.16/twitch-5.16.jar;"
        +CD+"/versions/Forge " + this->instanseMC + "/Forge 1.7.10.jar",
        "net.minecraft.launchwrapper.Launch",
        "--username", this->nikname,
        "--version", "Forge " + this->instanseMC,
        "--gameDir", CD,
        "--assetsDir", CD+"/assets",
        "--assetIndex", this->instanseMC,
        "--uuid", "00000000000000000000000000000000",
        "--accessToken", "null",
        "--userProperties", "{}",
        "--userType", "legacy",
        "--tweakClass", "cpw.mods.fml.common.launcher.FMLTweaker",
        "--width", "925",
        "--height", "530"
    };
    QList<QString> args3 = {
        "-Xmn128M",
        "-Xmx2048M",
        "-Dfile.encoding=UTF-8",
        "-Dfml.ignoreInvalidMinecraftCertificates=true",
        "-Dfml.ignorePatchDiscrepancies=true",
        "-XX:HeapDumpPath=MojangTricksIntelDriversForPerformance_javaw.exe_minecraft.exe.heapdump",
        "-Djava.net.useSystemProxies=true",
        "-Djava.library.path=" + CD + "/versions/Lite/natives",
        "-cp",
        CD + "/libraries/net/minecraftforge/forge/1.12.2-14.23.5.2815/forge-1.12.2-14.23.5.2815.jar;"
      + CD + "/libraries/net/minecraft/launchwrapper/1.12/launchwrapper-1.12.jar;"
      + CD + "/libraries/org/ow2/asm/asm-all/5.2/asm-all-5.2.jar;CD/libraries/jline/jline/2.13/jline-2.13.jar;"
      + CD + "/libraries/com/typesafe/akka/akka-actor_2.11/2.3.3/akka-actor_2.11-2.3.3.jar;CD/libraries/com/typesafe/config/1.2.1/config-1.2.1.jar;"
      + CD + "/libraries/org/scala-lang/scala-actors-migration_2.11/1.1.0/scala-actors-migration_2.11-1.1.0.jar;"
      + CD + "/libraries/org/scala-lang/scala-compiler/2.11.1/scala-compiler-2.11.1.jar;"
      + CD + "/libraries/org/scala-lang/plugins/scala-continuations-library_2.11/1.0.2/scala-continuations-library_2.11-1.0.2.jar;"
      + CD + "/libraries/org/scala-lang/plugins/scala-continuations-plugin_2.11.1/1.0.2/scala-continuations-plugin_2.11.1-1.0.2.jar;"
      + CD + "/libraries/org/scala-lang/scala-library/2.11.1/scala-library-2.11.1.jar;"
      + CD + "/libraries/org/scala-lang/scala-parser-combinators_2.11/1.0.1/scala-parser-combinators_2.11-1.0.1.jar;"
      + CD + "/libraries/org/scala-lang/scala-reflect/2.11.1/scala-reflect-2.11.1.jar;"
      + CD + "/libraries/org/scala-lang/scala-swing_2.11/1.0.1/scala-swing_2.11-1.0.1.jar;"
      + CD + "/libraries/org/scala-lang/scala-xml_2.11/1.0.2/scala-xml_2.11-1.0.2.jar;"
      + CD + "/libraries/lzma/lzma/0.0.1/lzma-0.0.1.jar;"
      + CD + "/libraries/net/sf/jopt-simple/jopt-simple/5.0.3/jopt-simple-5.0.3.jar;"
      + CD + "/libraries/java3d/vecmath/1.5.2/vecmath-1.5.2.jar;"
      + CD + "/libraries/net/sf/trove4j/trove4j/3.0.3/trove4j-3.0.3.jar;"
      + CD + "/libraries/org/apache/maven/maven-artifact/3.5.3/maven-artifact-3.5.3.jar;"
      + CD + "/libraries/org/tlauncher/patchy/1.1/patchy-1.1.jar;"
      + CD + "/libraries/oshi-project/oshi-core/1.1/oshi-core-1.1.jar;"
      + CD + "/libraries/net/java/dev/jna/jna/4.4.0/jna-4.4.0.jar;"
      + CD + "/libraries/net/java/dev/jna/platform/3.4.0/platform-3.4.0.jar;"
      + CD + "/libraries/com/ibm/icu/icu4j-core-mojang/51.2/icu4j-core-mojang-51.2.jar;"
      + CD + "/libraries/net/sf/jopt-simple/jopt-simple/5.0.3/jopt-simple-5.0.3.jar;"
      + CD + "/libraries/com/paulscode/codecjorbis/20101023/codecjorbis-20101023.jar;"
      + CD + "/libraries/com/paulscode/codecwav/20101023/codecwav-20101023.jar;"
      + CD + "/libraries/com/paulscode/libraryjavasound/20101123/libraryjavasound-20101123.jar;"
      + CD + "/libraries/com/paulscode/librarylwjglopenal/20100824/librarylwjglopenal-20100824.jar;"
      + CD + "/libraries/com/paulscode/soundsystem/20120107/soundsystem-20120107.jar;"
      + CD + "/libraries/io/netty/netty-all/4.1.9.Final/netty-all-4.1.9.Final.jar;"
      + CD + "/libraries/com/google/guava/guava/21.0/guava-21.0.jar;"
      + CD + "/libraries/org/apache/commons/commons-lang3/3.5/commons-lang3-3.5.jar;"
      + CD + "/libraries/commons-io/commons-io/2.5/commons-io-2.5.jar;"
      + CD + "/libraries/commons-codec/commons-codec/1.10/commons-codec-1.10.jar;"
      + CD + "/libraries/net/java/jinput/jinput/2.0.5/jinput-2.0.5.jar;"
      + CD + "/libraries/net/java/jutils/jutils/1.0.0/jutils-1.0.0.jar;"
      + CD + "/libraries/com/google/code/gson/gson/2.8.0/gson-2.8.0.jar;"
      + CD + "/libraries/com/mojang/authlib/1.5.25/authlib-1.5.25.jar;"
      + CD + "/libraries/com/mojang/realms/1.10.22/realms-1.10.22.jar;"
      + CD + "/libraries/org/apache/commons/commons-compress/1.8.1/commons-compress-1.8.1.jar;"
      + CD + "/libraries/org/apache/httpcomponents/httpclient/4.3.3/httpclient-4.3.3.jar;"
      + CD + "/libraries/commons-logging/commons-logging/1.1.3/commons-logging-1.1.3.jar;"
      + CD + "/libraries/org/apache/httpcomponents/httpcore/4.3.2/httpcore-4.3.2.jar;"
      + CD + "/libraries/it/unimi/dsi/fastutil/7.1.0/fastutil-7.1.0.jar;"
      + CD + "/libraries/org/apache/logging/log4j/log4j-api/2.8.1/log4j-api-2.8.1.jar;"
      + CD + "/libraries/org/apache/logging/log4j/log4j-core/2.8.1/log4j-core-2.8.1.jar;"
      + CD + "/libraries/org/lwjgl/lwjgl/lwjgl/2.9.4-nightly-20150209/lwjgl-2.9.4-nightly-20150209.jar;"
      + CD + "/libraries/org/lwjgl/lwjgl/lwjgl_util/2.9.4-nightly-20150209/lwjgl_util-2.9.4-nightly-20150209.jar;"
      + CD + "/libraries/com/mojang/text2speech/1.10.3/text2speech-1.10.3.jar;"
      + CD + "/versions/Lite/Lite.jar",
         "-Dminecraft.applet.TargetDirectory=" + CD,
         "net.minecraft.launchwrapper.Launch",
         "--username", this->nikname,
         "--version", "Lite",
         "--gameDir", CD,
         "--assetsDir", CD+"/assets",
         "--assetIndex", "1.12", //this->instanseMC,
         "--uuid", "00000000000000000000000000000000",
         "--accessToken", "null",
         "--userProperties", "{}",
         "--userType", "forge",
         "--tweakClass", "net.minecraftforge.fml.common.launcher.FMLTweaker",
         "--width", "925",
         "--height", "530",
         "--server", "drsaha.hopto.org:25567"
    };
    QProcess runMC;
    runMC.setWorkingDirectory(CD);
    runMC.startDetached("java", args3);
    qDebug() << runMC.arguments();
}

bool Instanse::checkInstallation(QString mode)
{
    if(mode == "fast"){
        if(!instDir->exists()){
            //instDir->mkdir(instDir->path());
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

void Instanse::wget(QUrl url, const QMetaMethod &method)
{
    QNetworkRequest request;                        // Отправляемый запрос
    QString url2 = QString(url.toEncoded(QUrl::EncodeUnicode));
    request.setUrl(url2);   // Устанавлвиваем URL в запрос
    manager->get(request);  // Выполняем запрос
}
