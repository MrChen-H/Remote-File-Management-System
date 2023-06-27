QT       += core gui
QT       += sql
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

INCLUDEPATH += C:/Program Files/MySQL/MySQLServer 8.0

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialog.cpp \
    item_of_share.cpp \
    list_item.cpp \
    main.cpp \
    mainwindow.cpp \
    myfile_page.cpp \
    register.cpp \
    share_file_widget.cpp \
    user_info.cpp

HEADERS += \
    Cheak_Format.h \
    User_Name.h \
    dialog.h \
    item_of_share.h \
    list_item.h \
    mainwindow.h \
    myfile_page.h \
    register.h \
    share_file_widget.h \
    user_info.h

FORMS += \
    dialog.ui \
    item_of_share.ui \
    list_item.ui \
    mainwindow.ui \
    myfile_page.ui \
    register.ui \
    share_file_widget.ui \
    user_info.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    test_json.json

RESOURCES += \
    Resource.qrc
