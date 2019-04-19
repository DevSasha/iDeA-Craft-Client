#-------------------------------------------------
#
# Project created by QtCreator 2018-12-14T21:20:09
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = iDeA-Craft
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11
CONFIG -= debug_and_release debug_and_release_target

# Выбираем директорию сборки исполняемого файла
# в зависимости от режима сборки проекта
CONFIG(debug, debug|release) {
    #DESTDIR = $$PWD/../iDeA-Craft-Client-Debug
} else {
    #DESTDIR = $$PWD/../iDeA-Craft-Client-Release
}

# разделяем по директориям все выходные файлы проекта
MOC_DIR = $$OUT_PWD/common/Client/build
RCC_DIR = $$OUT_PWD/common/Client/build
UI_DIR = $$OUT_PWD/common/Client/build
unix:OBJECTS_DIR = $$OUT_PWD/common/Client/build/o/unix
win32:OBJECTS_DIR = $$OUT_PWD/common/Client/build/o/win32
macx:OBJECTS_DIR = $$OUT_PWD/common/Client/build/o/mac

# в зависимости от режима сборки проекта
# запускаем win deploy qt приложения в целевой директории, то есть собираем все dll
CONFIG(debug, debug|release) {
    #QMAKE_POST_LINK = $$(QTDIR)/bin/windeployqt $$DESTDIR --no-opengl-sw --no-angle --no-webkit2 --no-compiler-runtime --no-system-d3d-compiler --no-quick-import --no-translations
} else {
    #QMAKE_POST_LINK = $$(QTDIR)/bin/windeployqt $$DESTDIR --no-opengl-sw --no-angle --no-webkit2 --no-compiler-runtime --no-system-d3d-compiler --no-quick-import --no-translations
}

win32
{
    CONFIG += embed_manifest_exe
    QMAKE_LFLAGS += /MANIFESTUAC:"level='requireAdministrator'"
}

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    authorization.cpp \
    client.cpp \
    core.cpp \
    instanse.cpp

HEADERS += \
        mainwindow.h \
    authorization.h \
    client.h \
    core.h \
    instanse.h

FORMS += \
        mainwindow.ui \
    authorization.ui

#Installer

# В зависимости от режима сборки, определяем, куда именно будут собираться инсталляторы
CONFIG(release, debug|release) {
    # Задаём переменные, которые будут содержать пути с названиями инсталляторов
    INSTALLER_OFFLINE = $$PWD/../InstallerRelease/iDeA-Craft.offline
    INSTALLER_ONLINE = $$PWD/../InstallerRelease/iDeA-Craft.online

    # Задаём переменную, которая должна содержать путь к папке с данными
    DESTDIR_WIN = $$PWD/../packages/fun.ideacraft.launcher/data
    DESTDIR_WIN ~= s,/,\\,g
    # Задаём путь откуда всё приложение с DLL-ками нужно будет скопировать
    PWD_WIN = $$DESTDIR
    PWD_WIN ~= s,/,\\,g

    # Прежде, чем выполнять сборку инсталляторов, необходимо скопировать файлы
    # из выходной папки проекта вместе со всеми DLL в папку data, которая относится
    # к собираемому пакету
    #copydata.commands = $(COPY_DIR) $$PWD_WIN $$DESTDIR_WIN
    #copydata.depends = $(first)
    #first.depends = $(first) copydata
    #export(first.depends)
    #export(copydata.commands)
    # задаём кастомную цель сборки, при которой сначала выполним компирование файлов
    # а потом уже и остальное, что следует по скрипту QMake
    #QMAKE_EXTRA_TARGETS += copydata
    #QMAKE_POST_LINK += && $(COPY_DIR) $$PWD_WIN $$DESTDIR_WIN

    #CONFIG_XML1 = $$PWD/../packages/fun.ideacraft.launcher/config/config.xml
    CONFIG_XML = $$PWD/config/config.xml

    # Создаём цель по сборке Оффлайн Инсталлятора
    INPUT = $$CONFIG_XML $$PWD/../packages
    offlineInstaller.depends = copydata
    offlineInstaller.input = $$INPUT
    offlineInstaller.output = $$INSTALLER_OFFLINE
    offlineInstaller.commands = $$(QTDIR)/../../QtIFW2.0.1/bin/binarycreator --offline-only -c $$CONFIG_XML -p $$PWD/../packages ${QMAKE_FILE_OUT}
    offlineInstaller.CONFIG += target_predeps no_link combine
    #message($$offlineInstaller.commands)
    #QMAKE_EXTRA_COMPILERS += offlineInstaller

    # Создаём цель по сборке Онлайн Инсталлятора
    INPUT = $$CONFIG_XML $$PWD/../packages
    onlineInstaller.depends = copydata
    onlineInstaller.input = INPUT
    onlineInstaller.output = $$INSTALLER_ONLINE
    onlineInstaller.commands = $$(QTDIR)/../../QtIFW2.0.1/bin/binarycreator --online-only -c $$CONFIG_XML -p $$PWD/../packages ${QMAKE_FILE_OUT}
    onlineInstaller.CONFIG += target_predeps no_link combine

    #QMAKE_EXTRA_COMPILERS += onlineInstaller

    # Сборку репозитория производим после того, как были собраны Инсталляторы
    # Для этого воспользуемся QMAKE_POST_LINK вместо QMAKE_EXTRA_COMPILERS
    # Поскольку он хорошо для этого подходит
    #QMAKE_POST_LINK += $$(QTDIR)/../../QtIFW2.0.1/bin/repogen -p $$PWD/packages -i fun.ideacraft.launcher --update $$OUT_PWD/../../repository

}
