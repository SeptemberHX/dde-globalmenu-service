set(_VERSION_MAJOR 1)
set(_VERSION_MINOR 0)
set(_VERSION_PATCH 1)

set(CPACK_GENERATOR "DEB")

############下面是设置debian/control文件中的内容

# 设置版本信息
set(CPACK_PACKAGE_VERSION_MAJOR "${_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${_VERSION_PATCH}")

# 设置安装包的包名，打好的包将会是packagename-version-linux.debian，如果不设置，默认是工程名
set(CPACK_PACKAGE_NAME "dde-globalmenu-service")

# 设置程序名，就是程序安装后的名字
set(CPACK_DEBIAN_PACKAGE_NAME "dde-globalmenu-service")

# 设置架构
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")

# 设置依赖
set(CPACK_DEBIAN_PACKAGE_DEPENDS
        "appmenu-gtk2-module, appmenu-gtk3-module, libdbusmenu-gtk3-4, libdbusmenu-gtk4, libdbusmenu-qt5-2")

# 设置section
set(CPACK_DEBIAN_PACKAGE_SECTION "utils")

# 设置priority
set(CPACK_DEBIAN_PACKAGE_PRIORITY "Optional")

# 设置description
set(CPACK_PACKAGE_DESCRIPTION "DDE Global Menu Service Daemon")

# 设置联系方式
set(CPACK_PACKAGE_CONTACT "september_hx@outlook.com")

# 设置维护人
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "SeptemberHX")

##############debian/control设置完成了，下面设置控制脚本，动态库，changelog等

# lib目录下保存这一些动态库和安装控制脚本（postinst、postinst、preinst、prerm）  doc目录下是changelog和copyright等文件 这些东西是打包时要依赖的一些额外的东西
set(LIBS_DIR ${CMAKE_SOURCE_DIR}/debian/scripts)
set(DOCS_DIR ${CMAKE_SOURCE_DIR}/debian/doc)

# 设置控制脚本，这些都是大包者本人自己准备的，没有就算了
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${LIBS_DIR}/postinst")

# 压缩 changelog
include(GNUInstallDirs)

add_custom_command(
    OUTPUT "${CMAKE_BINARY_DIR}/changelog.gz"
    COMMAND gzip -cn9 "${DOCS_DIR}/changelog" > "${CMAKE_BINARY_DIR}/changelog.gz"
    WORKING_DIRECTORY "${DOCS_DIR}"
    DEPENDS "${DOCS_DIR}/changelog"
    COMMENT "Compressing changelog"
)

add_custom_target(changelog ALL DEPENDS "${CMAKE_BINARY_DIR}/changelog.gz")

install(FILES ${CMAKE_BINARY_DIR}/dde-globalmenu-service DESTINATION /usr/bin/)
install(FILES ${CMAKE_SOURCE_DIR}/dde-globalmenu-service.desktop DESTINATION /usr/share/applications)


# 最后把我们的changelog等东西也加进来，project是你的工程名
install(FILES ${CMAKE_BINARY_DIR}/changelog.gz ${DOCS_DIR}/copyright DESTINATION ${CMAKE_INSTALL_DOCDIR})
