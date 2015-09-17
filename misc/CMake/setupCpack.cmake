macro(setupCpack PKG_NAME PKG_MAJOR PKG_MINOR PKG_LICENSE)

set(CPACK_GENERATOR ZIP)
set(CPACK_PACKAGE_NAME ${PKG_NAME})
set(CPACK_PACKAGE_VERSION ${PKG_VERSION})

# build a CPack driven installer package
include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE "${PKG_LICENSE}")
set (CPACK_PACKAGE_VERSION_MAJOR "${PKG_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${PKG_MINOR}")
include (CPack)

endmacro(setupCpack)

