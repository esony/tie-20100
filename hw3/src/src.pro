include(../defaults.pri)

CONFIG   += console
CONFIG   -= app_bundle
CONFIG   -= qt

mac{
  CONFIG += staticlib
}

TEMPLATE = lib

TARGET = hw3_nysse

SOURCES += datastructure.cc utility.cc
HEADERS += datastructure.hh utility.hh
