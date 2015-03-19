include(../defaults.pri)

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TEMPLATE = app

CONFIG += tests

unix{
  QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN
  QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN/../src
  QMAKE_RPATH=
  LIBS += -L../src
}

unix{
  HEADERS+=signaltranslator-posix.hh
}

win32{
  SOURCES+=signaltranslator-win.cc
  HEADERS+=signaltranslator-win.hh

  CONFIG(debug,debug|release) {
    LIBS += -L../src/debug
  } else {
    LIBS += -L../src/release
  }
}

DEFINES+=SOURCEDIR=\\\"\"$$_PRO_FILE_PWD_\"\\\"


SOURCES += tests.cc tests_main.cc ioredirector.cc
HEADERS += ioredirector.hh catch.hpp signaltranslator.hh

LIBS += -lhw2_candy
