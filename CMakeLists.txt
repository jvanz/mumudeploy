cmake_minimum_required(VERSION 2.8)
PROJECT(mumudeploy)
find_package(Qt4 REQUIRED QTCORE QTGUI QTDECLARATIVE QTSQL QTNETWORK)

set(CMAKE_CXX_FLAGS "-std=c++0x -g ${CMAKE_CXX_FLAGS}")

ADD_DEFINITIONS(${QT_DEFINITIONS})

include_directories(network)

SET(mumudeploy_SOURCE
	main.cpp
	controller/qmlinterface.cpp
	controller/filehandle.cpp
	controller/databasemanager.cpp
	controller/engine.cpp
	controller/mumufriend.cpp
	controller/mumuthread.cpp
	network/client/mumuclient.cpp
	network/server/mumuserver.cpp
	network/server/mumuconnection.cpp
	network/commum/mumufile.cpp
	network/commum/filedescriptor.cpp
	network/commum/util.cpp
	network/commum/mumublock.cpp
)

SET(mumudeploy_HEADERS
	controller/filehandle.h
	controller/databasemanager.h
	controller/engine.h
	controller/qmlinterface.h
	controller/mumufriend.h
	controller/mumuthread.h
	network/client/mumuclient.h
	network/server/mumuserver.h
	network/server/mumuconnection.h
	network/commum/mumufile.h
	network/commum/filedescriptor.h
	network/commum/util.h
	network/commum/mumublock.h
)


QT4_WRAP_CPP(mumudeploy_SOURCE_MOC ${mumudeploy_HEADERS})

INCLUDE(${QT_USE_FILE})

ADD_EXECUTABLE(mumudeploy ${mumudeploy_SOURCE}
${mumudeploy_SOURCE_MOC})

TARGET_LINK_LIBRARIES(mumudeploy ${QT_LIBRARIES})
