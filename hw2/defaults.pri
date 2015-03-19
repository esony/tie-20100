INCLUDEPATH += $$PWD/src
SRC_DIR = $$PWD

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS+=-Wall -Wextra -pedantic -Werror -O0 -fnon-call-exceptions
CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11
