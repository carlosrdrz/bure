# find libxmlPP
#
# exports:
#
#   LibXMLPP_FOUND
#   LibXMLPP_INCLUDE_DIRS
#   LibXMLPP_LIBRARIES
#

include(LibFindMacros)

libfind_package(LibXMLPP Glibmm)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(LibXMLPP_PKGCONF QUIET libxml++-3.0)

# Include dir
find_path(LibXMLPP_INCLUDE_DIR
  NAMES libxml++/libxml++.h
  PATHS
    ${LibXMLPP_PKGCONF_INCLUDE_DIRS}
    ${LibXMLPP_ROOT_DIR}
    /usr
  PATH_SUFFIXES
    include/libxml++-3.0
)

# Config Include dir
find_path(LibXMLPPConfig_INCLUDE_DIR
  NAMES libxml++config.h
  PATHS
    ${LibXMLPP_PKGCONF_INCLUDE_DIRS}
    ${LibXMLPP_ROOT_DIR}
    /usr
  PATH_SUFFIXES
    lib/libxml++-3.0/include
)

# Finally the library itself
find_library(LibXMLPP_LIBRARY
  NAMES xml++ xml++-3.0
  PATHS
    ${LibXMLPP_PKGCONF_LIBRARY_DIRS}
    ${LibXMLPP_ROOT_DIR}
    /usr
  PATH_SUFFIXES
    lib
)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibXMLPP DEFAULT_MSG LibXMLPP_LIBRARY LibXMLPP_INCLUDE_DIR)

if(LibXMLPP_INCLUDE_DIR AND LibXMLPP_LIBRARY)
  set(LibXMLPP_LIBRARIES ${Glibmm_LIBRARY} ${LibXMLPP_LIBRARY} ${LibXMLPP_PKGCONF_LIBRARIES})
  set(LibXMLPP_INCLUDE_DIRS ${Glib_INCLUDE_DIR} ${GlibConfig_INCLUDE_DIR} ${Glibmm_INCLUDE_DIR} ${GlibmmConfig_INCLUDE_DIR} ${LibXMLPP_INCLUDE_DIR} ${LibXMLPPConfig_INCLUDE_DIR} ${LibXMLPP_PKGCONF_INCLUDE_DIRS})
  set(LibXMLPP_FOUND yes)
else()
  set(LibXMLPP_LIBRARIES)
  set(LibXMLPP_INCLUDE_DIRS)
  set(LibXMLPP_FOUND no)
endif()
