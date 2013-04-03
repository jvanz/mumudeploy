cmake_minimum_required(VERSION 2.8)
PROJECT(mumudeploy)
find_package(Qt4 REQUIRED QTCORE QTGUI QTDECLARATIVE QTSQL)

set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

ADD_DEFINITIONS(${QT_DEFINITIONS})

SET(mumudeploy_SOURCE
	main.cpp
	controller/qmlinterface.cpp
	controller/filehandle.cpp
	controller/databasemanager.cpp
	controller/engine.cpp
)

SET(mumudeploy_HEADERS
	controller/filehandle.h
	controller/databasemanager.h
	controller/engine.h
	controller/qmlinterface.h
)

QT4_WRAP_CPP(mumudeploy_SOURCE_MOC ${mumudeploy_HEADERS})

INCLUDE(${QT_USE_FILE})

ADD_EXECUTABLE(mumudeploy ${mumudeploy_SOURCE}
${mumudeploy_SOURCE_MOC})

TARGET_LINK_LIBRARIES(mumudeploy ${QT_LIBRARIES})
