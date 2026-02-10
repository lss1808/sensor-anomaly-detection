QT += widgets
CONFIG += c++17
TEMPLATE = app
TARGET = detector_gui

SOURCES += \
    src/gui_main.cpp \
    src/outlier_filter.cpp

HEADERS += \
    src/outlier_filter.h
