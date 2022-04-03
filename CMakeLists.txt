cmake_minimum_required(VERSION 3.13)

set(PROJECT dde-globalmenu-service)

project(${PROJECT})

remove_definitions(-DQT_NO_CAST_FROM_ASCII -DQT_STRICT_ITERATORS -DQT_NO_CAST_FROM_BYTEARRAY -DQT_NO_KEYWORDS)

set(CMAKE_CXX_STANDARD 14)

# 启用 qt moc 的支持
set(CMAKE_AUTOMOC ON)
# 启用 qrc 资源文件的支持
set(CMAKE_AUTORCC ON)

set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(Qt5Widgets REQUIRED)
find_package(Qt5DBus REQUIRED)
find_package(KF5WindowSystem REQUIRED)
find_package(Qt5X11Extras REQUIRED)
find_package(XCB REQUIRED COMPONENTS xcb)

set(SRCS main.cpp
        menuimporter.h menuimporter.cpp
        gmenu-dbusmenu-proxy/dbusmenutypes_p.h gmenu-dbusmenu-proxy/dbusmenutypes_p.cpp
        dbusmenushortcut_p.h dbusmenushortcut_p.cpp

        # gmenu dbus proxy
        gmenu-dbusmenu-proxy/utils.h gmenu-dbusmenu-proxy/utils.cpp
        gmenu-dbusmenu-proxy/window.h gmenu-dbusmenu-proxy/window.cpp
        gmenu-dbusmenu-proxy/menuproxy.h gmenu-dbusmenu-proxy/menuproxy.cpp
        gmenu-dbusmenu-proxy/menu.h gmenu-dbusmenu-proxy/menu.cpp
        gmenu-dbusmenu-proxy/icons.h gmenu-dbusmenu-proxy/icons.cpp
        gdbusmenutypes_p.h gdbusmenutypes_p.cpp
        gmenu-dbusmenu-proxy/actions.h gmenu-dbusmenu-proxy/actions.cpp
        config-X11.h
        )

qt5_add_dbus_adaptor(SRCS com.canonical.AppMenu.Registrar.xml
    menuimporter.h MenuImporter menuimporteradaptor MenuImporterAdaptor)

qt5_add_dbus_adaptor(SRCS com.canonical.dbusmenu.xml gmenu-dbusmenu-proxy/window.h Window)


add_executable(${PROJECT} ${SRCS})

target_include_directories(${PROJECT} PUBLIC
        ${Qt5Widgets_INCLUDE_DIRS}
        ${Qt5DBus_INCLUDE_DIRS}
        ${Qt5X11Extras_INCLUDE_DIRS}
        )

target_link_libraries(${PROJECT}
        Qt5::Widgets
        Qt5::DBus
        Qt5::X11Extras
        KF5::WindowSystem
        ${XCB_LIBRARIES}
        )

include(deb.cmake)
include(CPack)