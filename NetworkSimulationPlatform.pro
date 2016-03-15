#-------------------------------------------------
#
# Project created by QtCreator 2014-07-26T15:50:25
#
#-------------------------------------------------

QT       += core gui network

TARGET = NetworkSimulationPlatform
TEMPLATE = app

INCLUDEPATH += ./TopoInf/

SOURCES += main.cpp\
        NetworkSimulationPlatform.cpp \
    CUnitboxWidget.cpp \
    CNewSwitcher.cpp \
    CNetworkEditWidget.cpp \
    BoundingRectItem.cpp \
    Device/CSwitherConfigure.cpp \
    Device/CSwitcher.cpp \
    Device/CNetworkPort.cpp \
    Device/CItemGroup.cpp \
    Clink.cpp \
    CWizard.cpp \
    DragNetwork.cpp \
    PlacementAlgorithm.cpp \
    CTaskConfigDialog.cpp \
    CPartitionNumDialog.cpp \
    Application/CEchoAppTaskDialog.cpp \
    Application/COnOffAppTaskDialog.cpp \
    CEventscheduldialog.cpp \
    Application/CBulkAppTaskDialog.cpp \
    CPortDialog.cpp \
    TopoInf/topotest.cpp \
    SDNNet/showflowsdialog.cpp \
    SDNNet/addflowdialog.cpp \
    SDNNet/showsportdialog.cpp \
    SDNCurl/sdncurl.cpp \
    SDNNet/sdnserver.cpp \
    SDNNet/serverprocess.cpp \
    TopoInf/DataDefine.cpp

HEADERS  += NetworkSimulationPlatform.h \
    StructFile.h \
    CUnitboxWidget.h \
    CNetworkEditWidget.h \
    label.h \
    CNewSwitcher.h \
    BoundingRectItem.h \
    Device/CSwitherConfigure.h \
    Device/CSwitcher.h \
    Device/CNetworkPort.h \
    Device/CModel.h \
    Device/CItemGroup.h \
    Device/CBaseObject.h \
    Clink.h \
    CWizard.h \
    DragNetwork.h \
    PlacementAlgorithm.h \
    StructDefine.h \
    CTaskConfigDialog.h \
    CPartitionNumDialog.h \
    Application/CEchoAppTaskDialog.h \
    Application/COnOffAppTaskDialog.h \
    Application/CBulkAppTaskDialog.h \
    CEventscheduldialog.h \
    CPortDialog.h \
    TopoInf/DataDefine.h \
    TopoInf/topotest.h \
    SDNNet/showflowsdialog.h \
    SDNNet/addflowdialog.h \
    SDNNet/showsportdialog.h \
    SDNCurl/sdncurl.h \
    SDNNet/sdnserver.h \
    SDNNet/serverprocess.h

FORMS    += NetworkSimulationPlatform.ui \
    CSwitherConfigure.ui \
    CNewSwitcher.ui \
    CDeviceConfigure.ui \
    wizard.ui \
    CTaskConfigDialog.ui \
    CEchoAppTaskDialog.ui \
    CPartitionNumDialog.ui \
    COnOffAppTaskDialog.ui \
    CBulkAppTaskDialog.ui \
    CEventscheduldialog.ui \
    CPortDialog.ui \
    SDNNet/showflowsdialog.ui \
    SDNNet/addflowdialog.ui \
    SDNNet/showsportdialog.ui


UI_DIR=./UI

QMAKE_CXXFLAGS += -Wno-unused-parameter

RESOURCES += NetworkSimulationPlatform.qrc

OTHER_FILES += \
    Resources/double.PNG
#-------------------linux curl-------------------------
INCLUDEPATH += /usr/local/curl/include/curl/
INCLUDEPATH += /usr/include/qjson/

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/curl/lib/release/ -lcurl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/curl/lib/debug/ -lcurl
else:unix: LIBS += -L$$PWD/../../../../usr/local/curl/lib/ -lcurl

INCLUDEPATH += $$PWD/../../../../usr/local/curl/include
DEPENDPATH += $$PWD/../../../../usr/local/curl/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib/i386-linux-gnu/release/ -lqjson
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib/i386-linux-gnu/debug/ -lqjson
else:unix: LIBS += -L$$PWD/../../../../usr/lib/i386-linux-gnu/ -lqjson

INCLUDEPATH += $$PWD/../../../../usr/lib/i386-linux-gnu
DEPENDPATH += $$PWD/../../../../usr/lib/i386-linux-gnu

INCLUDEPATH += $$PWD/../../../../usr/include/qjson/

