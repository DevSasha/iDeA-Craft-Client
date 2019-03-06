#include "core.h"

Core::Core(QApplication *parent) : QObject(parent)
{
    app = parent;
    manager = new QNetworkAccessManager;
    checkUpdate();
}

void Core::checkUpdate()
{
    connect(manager, &QNetworkAccessManager::finished, this, &Core::takeUpdeteNews);
    QNetworkRequest request;    // Отправляемый запрос
    request.setUrl(QString("http://drsaha.hopto.org/repo/idea-launcher/versions.json")); // Устанавлвиваем URL в запрос
    manager->get(request);      // Выполняем запрос
}

void Core::authorized()
{
    cfgs->beginGroup("AuthorizationData");
    cfgs->setValue("login", window_auth->login);
    cfgs->setValue("password", window_auth->password);
    cfgs->endGroup();
    window_auth->close();
    window_main->setNik(window_auth->nik);
    window_main->show();
}

void Core::startInst(QString name)
{
    QString CD = Local->path() + "/instanses/" + name;
    QList<QString> args = {
        "-Xmx768M",
        "-Dfile.encoding=UTF-8",
        "-Dfml.ignoreInvalidMinecraftCertificates=true",
        "-Dfml.ignorePatchDiscrepancies=true",
        "-Djava.net.useSystemProxies=true",
        "-XX:HeapDumpPath=MojangTricksIntelDriversForPerformance_javaw.exe_minecraft.exe.heapdump",
        "-Djava.library.path="+CD+"/versions/Forge_1.7.10/natives",
        "-Dminecraft.launcher.brand=java-minecraft-launcher",
        "-Dminecraft.launcher.version=1.6.84-j",
        "-Dminecraft.client.jar="+CD+"/versions/Forge_1.7.10/Forge_1.7.10.jar",
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
        +CD+"/versions/Forge_1.7.10/Forge_1.7.10.jar",
        "net.minecraft.launchwrapper.Launch",
        "--username", window_auth->nik,
        "--version", "Forge_1.7.10",
        "--gameDir", CD,
        "--assetsDir", CD+"/assets",
        "--assetIndex", "1.7.10",
        "--uuid", "00000000000000000000000000000000",
        "--accessToken", "null",
        "--userProperties", "{}",
        "--userType", "legacy",
        "--tweakClass", "cpw.mods.fml.common.launcher.FMLTweaker",
        "--width", "925",
        "--height", "530"
    };
    QProcess runMC;
    runMC.setWorkingDirectory(CD);
    qDebug() << runMC.workingDirectory();
    qDebug() << args[11];
    runMC.startDetached("java", args);
}

void Core::downloadInst(QString name)
{
    inst = new Instanse(name);
    connect(inst, &Instanse::processUpdate, window_main, &MainWindow::processUpdate);
    connect(inst, &Instanse::downloadComplete, this, &Core::downloadComplete);
}

void Core::downloadComplete()
{
    window_main->installationComplete();
    qDebug() << "Installation complete";
}

void Core::takeUpdeteNews(QNetworkReply *reply)
{
    if(reply->error()){
            qDebug() << "ERROR1" << reply->errorString();
            QMessageBox::critical(nullptr, "Error update", "Can`t get list versions");
            QApplication::exit();
    } else {

        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject root = doc.object();
        QJsonValue lastVersion = root.value("lastVersion");
        QString lastVer;
        if(lastVersion.isString()) lastVer = lastVersion.toString(); else {
            QMessageBox::critical(nullptr, "Error update", "Can`t read last version");
            QApplication::exit();
        }

        if(lastVer == appVersion)
            load();
        else {
            disconnect(manager, &QNetworkAccessManager::finished, this, &Core::takeUpdeteNews);
            connect(manager, &QNetworkAccessManager::finished, this, &Core::takeUpdate);
            QNetworkRequest request;    // Отправляемый запрос
            request.setUrl(QString("http://drsaha.hopto.org/repo/idea-launcher/iDeA-Craft-Updater.exe")); // Устанавлвиваем URL в запрос
            manager->get(request);      // Выполняем запрос
        }
    }
}

void Core::takeUpdate(QNetworkReply* reply)
{
    if(reply->error()){
            qDebug() << "ERROR"  << reply->errorString();
            QMessageBox::critical(nullptr, "Error update", "Can`t download updater");
            QApplication::exit();
    } else {
        QFile file("./Updater.exe");
        if(file.open(QFile::WriteOnly)){
            file.write(reply->readAll());  // ... и записываем всю информацию со страницы в файл
            file.close();                  // закрываем файл
        }

        QProcess updater;
        updater.setWorkingDirectory(".");
        QList<QString> args;
        args << "-Ver" << appVersion;
        updater.startDetached("Updater", args);
        QApplication::exit();
    }
}

void Core::load()
{
    //cfgs = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/configs.ini", QSettings::IniFormat);
    cfgs = new QSettings("DrSasha", "iDeA-Craft Launcher");
    client = new Client;
    window_auth = new Authorization(client);
    window_main = new MainWindow;
        connect(window_auth, &Authorization::authorized, this, &Core::authorized);
        connect(window_main, &MainWindow::startInst, this, &Core::startInst);
        connect(window_main, &MainWindow::downloadInst, this, &Core::downloadInst);
    Local = new QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
        if(!Local->exists())Local->mkdir(Local->path());

    cfgs->beginGroup("AuthorizationData");
    if(!cfgs->value("login").isNull() && !cfgs->value("password").isNull()){
        window_auth->login = cfgs->value("login").toString();
        window_auth->password = cfgs->value("password").toString();
        window_auth->auth();
    }else window_auth->show();
    cfgs->endGroup();
}
