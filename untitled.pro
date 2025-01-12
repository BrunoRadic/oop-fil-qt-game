QT       += core gui widgets qml quick

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    finishline.cpp \
    main.cpp \
    mainwindow.cpp \
    platform.cpp \
    collectible.cpp

HEADERS += \
    finishline.hpp \
    mainwindow.hpp \
    platform.hpp \
    collectible.hpp


FORMS += \
    mainwindow.ui

RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
