######################################################################
# Automatically generated by qmake (3.0) ter mai 21 11:44:06 2013
######################################################################
QT += core gui network
TEMPLATE = app
TARGET = server
INCLUDEPATH += .
QMAKE_CXXFLAGS += -std=c++0x

# Input
HEADERS += mumuconnection.h \
           mumuserver.h \
           ../networkmanager.h \
           ../commum/mumufile.h \
           ../commum/mumublock.h \
           ../commum/filedescriptor.h \
           ../commum/util.h \
           ../networkinterface.h \
           ../../controller/filehandle.h
SOURCES += main.cpp \
           mumuconnection.cpp \
           mumuserver.cpp \
           ../commum/mumufile.cpp \
           ../commum/mumublock.cpp \
           ../commum/filedescriptor.cpp \
           ../commum/util.cpp \
           ../../controller/filehandle.cpp
