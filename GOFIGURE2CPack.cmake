# setup CPack

INCLUDE( ${GOFIGURE2_SOURCE_DIR}/CMake/InstallSupportLibraries.cmake )

CONFIGURE_FILE( "${GOFIGURE2_SOURCE_DIR}/GOFIGURE2CPackOptions.cmake.in"
    "${GOFIGURE2_BINARY_DIR}/GOFIGURE2CPackOptions.cmake" @ONLY 
  )

SET( CPACK_PROJECT_CONFIG_FILE 
  "${GOFIGURE2_BINARY_DIR}/GOFIGURE2CPackOptions.cmake"
  )
SET( CPACK_PACKAGE_DESCRIPTION_SUMMARY 
  "GoFigure2 is a cross-platform application for microscope image visualization and analysis" 
  )
SET( CPACK_PACKAGE_VENDOR "Megason Lab, Systems Biology, HMS" )
SET( CPACK_PACKAGE_CONTACT "gofigure2-developpers@lists.sourceforge.net" )
SET( CPACK_PACKAGE_DESCRIPTION_FILE "${GOFIGURE2_SOURCE_DIR}/ReadMe.txt")
SET( CPACK_RESOURCE_FILE_LICENSE "${GOFIGURE2_SOURCE_DIR}/Licenses/Copyright.txt")
SET( CPACK_RESOURCE_FILE_WELCOME "${GOFIGURE2_SOURCE_DIR}/CPack.GenericWelcome.txt" )
SET( CPACK_PACKAGE_VERSION_MAJOR ${GOFIGURE2_MAJOR_VERSION} )
SET( CPACK_PACKAGE_VERSION_MINOR ${GOFIGURE2_MINOR_VERSION} )
SET( CPACK_PACKAGE_VERSION_PATCH ${GOFIGURE2_WC_REVISION} )
SET( CPACK_PACKAGE_INSTALL_DIRECTORY "GoFigure2" )
SET( CPACK_SOURCE_PACKAGE_FILE_NAME "GoFigure2-${GOFIGURE2_VERSION}" )

# taken from CMake
IF( NOT DEFINED CPACK_SYSTEM_NAME )
  # make sure package is not Cygwin-unknown, for Cygwin just
  # cygwin is good for the system name
  IF("${CMAKE_SYSTEM_NAME}" STREQUAL "CYGWIN")
    SET( CPACK_SYSTEM_NAME Cygwin )
  ELSE("${CMAKE_SYSTEM_NAME}" STREQUAL "CYGWIN")
    IF( CMAKE_SYSTEM_PROCESSOR MATCHES "unknown" )
      EXEC_PROGRAM( uname ARGS "-m" OUTPUT_VARIABLE CMAKE_SYSTEM_PROCESSOR )
    ENDIF( CMAKE_SYSTEM_PROCESSOR MATCHES "unknown" )

    SET( CPACK_SYSTEM_NAME ${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR} )
  ENDIF("${CMAKE_SYSTEM_NAME}" STREQUAL "CYGWIN")
ENDIF( NOT DEFINED CPACK_SYSTEM_NAME )

IF( ${CPACK_SYSTEM_NAME} MATCHES Windows )
 IF( CMAKE_CL_64 )
   SET( CPACK_SYSTEM_NAME win64-${CMAKE_SYSTEM_PROCESSOR} )
 ELSE( CMAKE_CL_64 )
   SET( CPACK_SYSTEM_NAME win32-${CMAKE_SYSTEM_PROCESSOR} )
 ENDIF( CMAKE_CL_64 )
ENDIF( ${CPACK_SYSTEM_NAME} MATCHES Windows )

IF( NOT DEFINED CPACK_PACKAGE_FILE_NAME )
  # if the CPACK_PACKAGE_FILE_NAME is not defined by the cache
  # default to source package - system, on cygwin system is not 
  # needed
  IF( CYGWIN )
    SET( CPACK_PACKAGE_FILE_NAME 
      "${CPACK_SOURCE_PACKAGE_FILE_NAME}" )
  ELSE( CYGWIN )
    SET( CPACK_PACKAGE_FILE_NAME
      "${CPACK_SOURCE_PACKAGE_FILE_NAME}-${CPACK_SYSTEM_NAME}" )
  ENDIF( CYGWIN )
ENDIF( NOT DEFINED CPACK_PACKAGE_FILE_NAME )

IF( WIN32 AND NOT UNIX )
  INCLUDE( ${CPACK_PROJECT_CONFIG_FILE} )
ELSE( WIN32 AND NOT UNIX )
  SET( CPACK_STRIP_FILES "bin/gofigure" )
  SET( CPACK_SOURCE_STRIP_FILES "" )
ENDIF( WIN32 AND NOT UNIX )

SET( CPACK_PACKAGE_EXECUTABLES "gofigure" "GoFigure2" )

# # # # cygwin specific packaging stuff
# # IF(CYGWIN)
# # ENDIF( CYGWIN )

IF( WIN32 )
	INSTALL( PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} 
		DESTINATION bin COMPONENT Runtime )
ENDIF( WIN32 )


SET( CPACK_COMPONENTS_ALL
  Runtime
  libraries
#  headers
)

INCLUDE( CPack )
