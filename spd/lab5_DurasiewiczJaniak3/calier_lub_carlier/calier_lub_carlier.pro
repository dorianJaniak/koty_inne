TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -O3

HEADERS += interfejs.h OpPozycji.h proces.h kopiec.h \
    permutacja.h
SOURCES += main.cpp interfejs.cpp kopiec.cpp \
    oppozycji.cpp \
    permutacja.cpp

include(deployment.pri)
qtcAddDeployment()

