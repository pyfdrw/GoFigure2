# setup CPack

INCLUDE( InstallRequiredSystemLibraries )

SET( CPACK_PACKAGE_DESCRIPTION_SUMMARY "GoFigure2" )
SET( CPACK_PACKAGE_VENDOR "Megason Lab, Systems Biology, HMS" )
SET( CPACK_PACKAGE_DESCRIPTION_FILE "${GOFIGURE2_SOURCE_DIR}/ReadMe.txt")
SET( CPACK_RESOURCE_FILE_LICENSE "${GOFIGURE2_SOURCE_DIR}/Copyright.txt")
SET( CPACK_PACKAGE_VERSION_MAJOR ${GOFIGURE2_MAJOR_VERSION} )
SET( CPACK_PACKAGE_VERSION_MINOR ${GOFIGURE2_MINOR_VERSION} )
SET( CPACK_PACKAGE_VERSION_PATCH ${GOFIGURE2_WC_REVISION} )
SET( CPACK_PACKAGE_INSTALL_DIRECTORY "GoFigure2 ${GOFIGURE2_VERSION}" )

SET( CPACK_COMPONENTS_ALL
  applications
  libraries
#  headers
)

#IF( CMAKE_SYSTEM_PROCESSOR MATCHES "unknown" )
#  SET( CMAKE_SYSTEM_PROCESSOR "x86" )
#ENDIF( CMAKE_SYSTEM_PROCESSOR MATCHES "unknown" )

IF( NOT DEFINED CPACK_SYSTEM_NAME )
  SET( CPACK_SYSTEM_NAME ${CMAKE_SYSTEM_NAME} ) #-${CMAKE_SYSTEM_PROCESSOR} )
ENDIF( NOT DEFINED CPACK_SYSTEM_NAME )

#IF( ${CPACK_SYSTEM_NAME} MATCHES Windows )
#  IF( CMAKE_CL_64 )
#    SET( CPACK_SYSTEM_NAME win64-${CMAKE_SYSTEM_PROCESSOR} )
#  ELSE( CMAKE_CL_64 )
#    SET( CPACK_SYSTEM_NAME win32-${CMAKE_SYSTEM_PROCESSOR} )
#  ENDIF( CMAKE_CL_64 )
#ENDIF( ${CPACK_SYSTEM_NAME} MATCHES Windows )

#IF( NOT DEFINED CPACK_PACKAGE_FILE_NAME )
#  SET( CPACK_PACKAGE_FILE_NAME
#  "GOFIGURE2-${GOFIGURE2_VERSION}-${CPACK_SYSTEM_NAME}" )
#ENDIF( NOT DEFINED CPACK_PACKAGE_FILE_NAME )

IF( UNIX )
  IF( APPLE )
    SET( CPACK_PACKAGE_EXECUTABLES "gofigure;gofigure" )
  ELSE( APPLE )
  ENDIF( APPLE )
ELSE( UNIX )
  IF( WIN32 )
    # There is a bug in NSI that does not handle full unix paths properly.
    # Make sure there is at least one set of four (4) backslashes.
    # SET( CPACK_PACKAGE_ICON
    #   "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp" )
    SET( CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\gofigure.exe")
    SET( CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY}
      GoFigure2" )
    SET( CPACK_NSIS_HELP_LINK
      "http:\\\\\\\\sourceforget.net\\\\projects\\\\gofigure2" )
    SET( CPACK_NSIS_URL_INFO_ABOUT "http:\\\\megason.med.harvard.edu" )
  #       SET( CPACK_NSIS_CONTACT "me@my-personal-home-page.com" )
    SET( CPACK_NSIS_MODIFY_PATH ON )
  ELSE( WIN32 )
    SET( CPACK_STRIP_FILES "bin/gofigure" )
    SET( CPACK_SOURCE_STRIP_FILES "" )
  ENDIF( WIN32 )
ENDIF( UNIX )

SET( CPACK_PACKAGE_EXECUTABLES "gofigure" "gofigure" )

IF( CPACK_GENERATOR STREQUAL "PackageMaker" )
  SET( CPACK_INSTALL_CMAKE_PROJECTS
  "${CMAKE_BINARY_DIR};${CMAKE_PROJECT_NAME};ALL;/" )
ENDIF( CPACK_GENERATOR STREQUAL "PackageMaker" )

INCLUDE( CPack )
