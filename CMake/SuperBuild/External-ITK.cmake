#---------------------------------------------------------------------------
# Get and build itk

set(proj ITK)

if( WIN32 )
  set( ITK_TAG "release" )
else()
  set( ITK_TAG "ce6cacdb19b889506bf0f97b994c0ddcf21ca5bf" )
endif()

ExternalProject_Add(${proj}
  GIT_REPOSITORY "${git_protocol}://itk.org/ITK.git"
  GIT_TAG "${ITK_TAG}"
  UPDATE_COMMAND ""
  SOURCE_DIR ${proj}
  BINARY_DIR ${proj}-build
  CMAKE_GENERATOR ${gen}
  CMAKE_ARGS
    ${ep_common_args}
    -DITK_INSTALL_LIB_DIR:PATH=${GOFIGURE2_INSTALL_LIB_DIR}
     # should change soon
    -DITK_USE_REVIEW:INTERNAL=ON
  INSTALL_COMMAND ""
  DEPENDS
    ${ITK_DEPENDENCIES}
)

set(ITK_DIR ${CMAKE_BINARY_DIR}/${proj}-build)
