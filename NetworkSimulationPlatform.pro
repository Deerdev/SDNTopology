#-------------------------------------------------
#
# Project created by QtCreator 2014-07-26T15:50:25
#
#-------------------------------------------------

QT       += core gui

TARGET = NetworkSimulationPlatform
TEMPLATE = app


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
    CPortDialog.cpp

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
    CPortDialog.h

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
    CPortDialog.ui


UI_DIR=./UI

QMAKE_CXXFLAGS += -Wno-unused-parameter

RESOURCES += NetworkSimulationPlatform.qrc

OTHER_FILES += \
    Resources/double.PNG
