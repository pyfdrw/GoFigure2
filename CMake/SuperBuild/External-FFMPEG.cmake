#-----------------------------------------------------------------------------
# Get and build FFMPEG
# ONLY MAC AND LINUX
# TODO BUILD GOOD ARCH ON MAC i386 or x86_64????
#-----------------------------------------------------------------------------
set( proj FFMPEG )
set( SHARED_FFMPEG )

IF( SUPER_SHARED_LIBS )
  set( SHARED_FFMPEG --enable-shared --disable-static )
ENDIF( SUPER_SHARED_LIBS )

OPTION( FFMPEG_GPL "Use a GPL version of FFMPEG" OFF )

SET(FFMPEG_GPL_FLAG "")

IF( FFMPEG_GPL )
  SET(FFMPEG_GPL_FLAG "--enable-gpl")
  SET(msg "ATTENTION: You have enabled the use of GPL components of FFMPEG.")
  SET(msg "${msg} By enabling this option, the binary of GoFigure2")
  SET(msg "${msg} that you are going to build will be covered by a GPL license.")
  MESSAGE("${msg}")
ENDIF( FFMPEG_GPL )

SET(FFMPEG_INSTALL_DIR  ${CMAKE_BINARY_DIR}/${proj}-install)

ExternalProject_Add(${proj}
  # Set up dirs
  SOURCE_DIR ${CMAKE_BINARY_DIR}/${proj}
  INSTALL_DIR ${FFMPEG_INSTALL_DIR}
  # get the project
  GIT_REPOSITORY "${git_protocol}://git.videolan.org/ffmpeg.git"
  GIT_TAG "ffmpeg-0.6.3"

  # Build the project
  BUILD_IN_SOURCE 1

  # Configure step
  # DO STH FOR THE ARCHITECTURE...
  CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=${FFMPEG_INSTALL_DIR} ${SHARED_FFMPEG} ${FFMPEG_GPL_FLAG} --enable-avfilter --disable-yasm --disable-decoders --disable-zlib --disable-demuxer=matroska
)

# define the library suffix
IF( SUPER_SHARED_LIBS )
  IF( APPLE )
    SET(LIBRARY_SUFFIX .dylib)
  ELSE( APPLE )
    SET(LIBRARY_SUFFIX .so)
  ENDIF( APPLE )
ELSE( SUPER_SHARED_LIBS )
  SET(LIBRARY_SUFFIX .a)
ENDIF( SUPER_SHARED_LIBS )

# ADD DIRECTORIES FOR DEPENDENCIES IN Main/CMakeLists.txt
set( FFMPEG_LIBRARY_DIRS ${FFMPEG_INSTALL_DIR}/lib/libavcodec
                         ${FFMPEG_INSTALL_DIR}/lib/libavformat
                         ${FFMPEG_INSTALL_DIR}/lib/libavutil
                         ${FFMPEG_INSTALL_DIR}/lib/libswscale
)

set( VIDEO_SUPPORT
  # turn ON ffmpeg encoder
  -DVTK_USE_FFMPEG_ENCODER:BOOL=ON
  # force VTK to use NEW headers
  #-DVTK_FFMPEG_CACHED_INCLUDE:PATH=${CMAKE_BINARY_DIR}/${proj}
  #-DVTK_FFMPEG_HAS_OLD_HEADER:BOOL=FALSE
  # set useful variables
  -DFFMPEG_INCLUDE_DIR:PATH=${FFMPEG_INSTALL_DIR}/include
  -DFFMPEG_avcodec_LIBRARY:FILEPATH=${FFMPEG_INSTALL_DIR}/lib/libavcodec${LIBRARY_SUFFIX}
  -DFFMPEG_avformat_LIBRARY:FILEPATH=${FFMPEG_INSTALL_DIR}/lib/libavformat${LIBRARY_SUFFIX}
  -DFFMPEG_avutil_LIBRARY:FILEPATH=${FFMPEG_INSTALL_DIR}/lib/libavutil${LIBRARY_SUFFIX}
  -DFFMPEG_swscale_LIBRARY:FILEPATH=${FFMPEG_INSTALL_DIR}/lib/libswscale${LIBRARY_SUFFIX}
)
