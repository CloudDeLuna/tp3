INCLUDEPATH += $$PWD
SOURCES += $$PWD/openglwindow.cpp \
    gamecamera.cpp \
    gamewindow.cpp \
    gameserver.cpp \
    gameclient.cpp \
    thread.cpp
HEADERS += $$PWD/openglwindow.h \
    gamecamera.h \
    gamewindow.h \
    gameserver.h \
    gameclient.h \
    thread.h

SOURCES += \
    main.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/gui/openglwindow
QT += network
INSTALLS += target
QMAKE_MAC_SDK = macosx10.11

RESOURCES += \
    gestionnaire.qrc
