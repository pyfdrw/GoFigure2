SET( CTEST_SOURCE_DIRECTORY "C:/Devel/SVNROOT/sourceforge/gofigure2" )
SET( CTEST_BINARY_DIRECTORY "C:/Devel/SVNROOT/sourceforge/gofigure2/NBUILD64" )

SET( CTEST_COMMAND 
 "\"C:/Program Files (x86)/CMake 2.6/bin/ctest.exe\" -C Debug -D Nightly -A \"${CTEST_SCRIPT_DIRECTORY}/${CTEST_SCRIPT_NAME}\""
 )

SET( CTEST_CMAKE_COMMAND 
  "\"C:/Program Files (x86)/CMake 2.6/bin/cmake.exe\""
  )

SET( CTEST_START_WITH_EMPTY_BINARY_DIRECTORY TRUE)

SET( CTEST_INITIAL_CACHE "

BUILDNAME:STRING=Win64-vs80
SITE:STRING=megawin.megason-lab.hms
CMAKE_CONFIGURATION_TYPES:STRING=Release
ITK_DIR:PATH=C:/Devel/CVSROOT/Insight/BUILD64-REL
VTK_DIR:PATH=C:/Devel/CVSROOT/VTK/BUILD64
QT_QMAKE_EXECUTABLE:FILEPATH=C:/Qt/2009.01/win64/qt/bin/qmake.exe
CMAKE_GENERATOR:INTERNAL=Visual Studio 8 2005 Win64
DROP_METHOD:STRING=http
CMAKE_BUILD_TYPE:STRING=Debug
SVNCOMMAND:FILEPATH=C:/cygwin/bin/svn.exe
" )


