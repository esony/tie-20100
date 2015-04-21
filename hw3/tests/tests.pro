include(../defaults.pri)

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TEMPLATE = app

CONFIG += tests

unix:!mac{
  QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN
  QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN/../src
  QMAKE_RPATH=
}

mac{
  CONFIG += static
}

unix{
  HEADERS+=signaltranslator-posix.hh
  LIBS += -L../src
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

SOURCES += tests.cc tests_main.cc ioredirector.cc
HEADERS += ioredirector.hh catch.hpp signaltranslator.hh

LIBS += -lhw3_nysse
