#-----------------------------------------------------------------------------
# Get and build VTK
#
#-----------------------------------------------------------------------------
set(proj VTK)

ExternalProject_Add(${proj}
  SOURCE_DIR ${CMAKE_BINARY_DIR}/${proj}
  BINARY_DIR ${proj}-build
  GIT_REPOSITORY "${git_protocol}://vtk.org/VTK.git"
  GIT_TAG "origin/release"
  CMAKE_GENERATOR ${gen}
  CMAKE_ARGS
    ${ep_common_args}
    -DCMAKE_CXX_FLAGS:STRING=${ep_common_cxx_flags}
    -DCMAKE_C_FLAGS:STRING=${ep_common_c_flags}
    -DVTK_DEBUG_LEAKS:BOOL=${USE_VTK_DEBUG_LEAKS}
    -DVTK_INSTALL_LIB_DIR:PATH=${GoFigure2_INSTALL_LIB_DIR}
    -DVTK_USE_QT:BOOL=ON
	-DQT_QMAKE_EXECUTABLE:PATH=${QT_QMAKE_EXECUTABLE}
    -DVTK_USE_MYSQL:BOOL=ON
    ${VIDEO_SUPPORT}
  INSTALL_COMMAND ""
  DEPENDS
    ${VTK_DEPENDENCIES}
)

set(VTK_DIR ${CMAKE_BINARY_DIR}/${proj}-build)
