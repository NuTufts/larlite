set(MODULE_NAME Base )

# DEFINE THE TARGET
set( LIBNAME larlite_${MODULE_NAME} )

set( HEADERS Base-TypeDef.h
  AnalysisConstants.h
  DataFormatConstants.h
  DetectorStatusTypes.h
  FEMConstants.h
  FrameworkConstants.h
  GeoConstants.h
  GeoTypes.h
  geo_types.h
  MCConstants.h
  PhysicalConstants.h  
  RawConstants.h
  RecoTypes.h
  UtilFunc.h
  larlite_base.h
  geo_vectors.h
  messenger.h
  span.h
  )

add_library( ${LIBNAME} SHARED DataFormatConstants.cxx
  UtilFunc.cxx
  GeoTypes.cxx
  larlite_base.cxx
  messenger.cxx )

# create a property to hold our headers
set_target_properties(${LIBNAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")

# DEFINE SOME VARS THAT MIGHT BE USEFUL FOR SUBMODULES
#message("msg[${MODULE_NAME}]: for debug")

get_target_property(ALL_HEADERS ${LIBNAME} PUBLIC_HEADER)

target_include_directories(${LIBNAME}
  PUBLIC
  $<INSTALL_INTERFACE:include/larlite/${MODULE_NAME}>
  ${LARLITE_INC_DIRS}
  PRIVATE
  ${PROJECT_SOURCE_DIR}
  )
target_link_libraries(${LIBNAME})

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR} )
ROOT_GENERATE_DICTIONARY(G__${LIBNAME} ${ALL_HEADERS} ${HEADERS} LINKDEF LinkDef.h)
target_sources( ${LIBNAME} PRIVATE G__${LIBNAME}.cxx )

# install libraries and headers
install(TARGETS ${LIBNAME}
  EXPORT larlite
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include/larlite/${MODULE_NAME})

# install files needed for dictionary use in CINT/CLING/PYTHON
install( FILES
  ${CMAKE_CURRENT_BINARY_DIR}/lib${LIBNAME}_rdict.pcm
  ${CMAKE_CURRENT_BINARY_DIR}/lib${LIBNAME}.rootmap
  DESTINATION lib  )
