project(telepathy-bell)

cmake_minimum_required(VERSION 2.8)

include_directories(${CMAKE_SOURCE_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

# Uses QT5 by default.
if (USE_QT4)
    find_package(Qt4 4.6.0 REQUIRED QtCore QtNetwork)
    find_package(TelepathyQt4 0.9.6 REQUIRED)
    find_package(TelepathyQt4Service 0.9.6 REQUIRED)
else()
    find_package(Qt5 REQUIRED COMPONENTS Core DBus Xml Network)
    find_package(TelepathyQt5 0.9.6 REQUIRED)
    find_package(TelepathyQt5Service 0.9.6 REQUIRED)
endif()

# Add Ring specific deps here.
find_package(Ring REQUIRED)

include(GNUInstallDirs)
include_directories(${CMAKE_SOURCE_DIR} ${ring_INCLUDE_DIRS})

set(bell_SOURCES
    telepathy-bell.cpp
    common.cpp
    connection.cpp
    protocol.cpp
    ringdaemon.cpp
)

add_executable(telepathy-bell ${bell_SOURCES})
set_target_properties(telepathy-bell PROPERTIES AUTOMOC TRUE)

if (USE_QT4)
    target_include_directories(telepathy-bell PRIVATE
        ${QT_INCLUDES}
        ${TELEPATHY_QT4_INCLUDE_DIR}
      )
    target_link_libraries(telepathy-bell
        ${QT_QTCORE_LIBRARY}
        ${QT_QTDBUS_LIBRARY}
        ${QT_QTNETWORK_LIBRARY}
        ${QT_QTGUI_LIBRARY}
        ${TELEPATHY_QT4_LIBRARIES}
        ${TELEPATHY_QT4_SERVICE_LIBRARIES}
        )
else()
    target_include_directories(telepathy-bell PRIVATE
        ${TELEPATHY_QT5_INCLUDE_DIR}
    )
    target_link_libraries(telepathy-bell
        Qt5::Core
        Qt5::DBus
        Qt5::Network
        ${TELEPATHY_QT5_LIBRARIES}
        ${TELEPATHY_QT5_SERVICE_LIBRARIES}
      )
endif()

configure_file(bell.service.in org.freedesktop.Telepathy.ConnectionManager.bell.service)

install(
    TARGETS telepathy-bell
    DESTINATION ${CMAKE_INSTALL_LIBEXECDIR}
)

install(
    FILES bell.manager
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/telepathy/managers
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/org.freedesktop.Telepathy.ConnectionManager.bell.service
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/dbus-1/services
)
