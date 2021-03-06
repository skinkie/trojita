QT += core network
lessThan(QT_MAJOR_VERSION, 5) {
    CONFIG += qtestlib
} else {
    QT += testlib
}
DEPENDPATH += ../../src/ ../
INCLUDEPATH += ../../src/ ../
CONFIG += staticlib
TEMPLATE = lib
TARGET = test_LibMailboxSync

#trojita_libs = Imap Streams
#myprefix = ../../src/
#include(../../src/linking.pri)
include(../../configh.pri)

HEADERS += ../TagGenerator.h \
    FakeCapabilitiesInjector.h \
    test_LibMailboxSync.h \
    ModelEvents.h
SOURCES += test_LibMailboxSync.cpp \
    ModelEvents.cpp

# the upper makefile really wants to call `make check` in here...
check.target = check
QMAKE_EXTRA_TARGETS += check
