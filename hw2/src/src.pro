include(../defaults.pri)

CONFIG   += console
CONFIG   -= app_bundle
CONFIG   -= qt

TEMPLATE = lib

TARGET = hw2_candy

SOURCES += datastructure.cc utility.cc
HEADERS += datastructure.hh utility.hh
