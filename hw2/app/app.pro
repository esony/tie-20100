include(../defaults.pri)

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TEMPLATE = app

SOURCES += main.cc

unix{
  QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN
  QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN/../src
  QMAKE_RPATH=
  LIBS += -L../src
}
win32{
  CONFIG(debug,debug|release) {
    LIBS += -L../src/debug
  } else {
    LIBS += -L../src/release
  }
}

LIBS += -lhw2_candy
