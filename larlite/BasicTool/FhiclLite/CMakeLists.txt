set(MODULE_NAME BasicTool_FhiclLite )

# DEFINE THE TARGET
set( LIBNAME larlite_${MODULE_NAME} )

set( HEADERS ConfigManager.h
  FhiclLiteException.h
  FhiclLite-TypeDef.h
  FhiclLiteUtilFunc.h
  Parser.h
  PSet.h
  UtilFunc.h )

add_library( ${LIBNAME} SHARED ConfigManager.cxx
  FhiclLiteUtilFunc.cxx
  Parser.cxx
  PSet.cxx
  UtilFunc.cxx )

# create a property to hold our headers
set_target_properties(${LIBNAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")

# DEFINE SOME VARS THAT MIGHT BE USEFUL FOR SUBMODULES
#message("msg[${MODULE_NAME}]: for debug")

set_target_properties(${LIBNAME} PROPERTIES
  PUBLIC_HEADER "${HEADERS}"
  CXX_STANDARD ${CXX_STANDARD}
  CXX_STANDARD_REQUIRED ON )  


target_include_directories(${LIBNAME}
  PUBLIC
  $<INSTALL_INTERFACE:include/larlite/BasicTool/FhiclLite>
  ${LARLITE_INC_DIRS}
  PRIVATE
  ${PROJECT_SOURCE_DIR}
  )
target_link_libraries(${LIBNAME} ${ROOT_LIBRARIES})

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR} )
ROOT_GENERATE_DICTIONARY(G__${LIBNAME} ${ALL_HEADERS} ${HEADERS} LINKDEF LinkDef.h)
target_sources( ${LIBNAME} PRIVATE G__${LIBNAME}.cxx )

# install libraries and headers
install(TARGETS ${LIBNAME}
  EXPORT larlite
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include/larlite/BasicTool/FhiclLite)

# install files needed for dictionary use in CINT/CLING/PYTHON
install( FILES
  ${CMAKE_CURRENT_BINARY_DIR}/lib${LIBNAME}_rdict.pcm
  ${CMAKE_CURRENT_BINARY_DIR}/lib${LIBNAME}.rootmap
  DESTINATION lib  )
