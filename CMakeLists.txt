cmake_minimum_required(VERSION 2.8)
PROJECT(mumudeploy)
find_package(Qt4 REQUIRED QTCORE QTGUI QTDECLARATIVE QTSQL)

#add_definitions(${QDECLARATIVE_BOOSTABLE_CFLAGS})
#link_libraries(${QDECLARATIVE_BOOSTABLE_LDFLAGS})

ADD_DEFINITIONS(${QT_DEFINITIONS})

SET(mumudeploy_SOURCE main.cpp filehandle.cpp engine.cpp databasemanager.h)
SET(mumudeploy_HEADERS filehandle.h engine.h databasemanager.cpp)

QT4_WRAP_CPP(mumudeploy_SOURCE_MOC ${mumudeploy_HEADERS})

INCLUDE(${QT_USE_FILE})

ADD_EXECUTABLE(mumudeploy ${mumudeploy_SOURCE}
${mumudeploy_SOURCE_MOC})

TARGET_LINK_LIBRARIES(mumudeploy ${QT_LIBRARIES})
