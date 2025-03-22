set(MODULE_NAME DataFormat )

# DEFINE THE TARGET
set( LIBNAME larlite_${MODULE_NAME} )

set( HEADERS
  larlite_dataformat_utils.h  
  DataFormatException.h
  data_base.h  
  event_ass.h
  gtruth.h
  mcpart.h
  mcnu.h
  mctruth.h
  mcflux.h
  mcshower.h
  mctrack.h
  mctrajectory.h
  treenode.h
  mctree.h
  mceventweight.h
  simch.h
  simphotons.h
  potsummary.h
  rawdigit.h
  daqheadertimeuboone.h
  sparse_vector.h
  wire.h 
  hit.h
  crthit.h
  crttrack.h
  cosmictag.h
  ophit.h
  opflash.h
  cluster.h
#  auxsimch.h
#  calorimetry.h
#  chstatus.h
# DataFormat-TypeDef.h
# endpoint2d.h
# fifo.h
# flashmatch.h
# larflow3dhit.h
# larflowcluster.h
# larmatchsp.h
# minos.h
# mucsdata.h
# mucsreco.h
# opdetwaveform.h
# partid.h
# pcaxis.h
# pfpart.h
# pixelmask.h
# roi.h
# seed.h
# shower.h
# spacepoint.h
# #storage_manager.h
# #storage_store.h
# swtrigger.h
# t0.h
# track.h
# trackmom.h
# trigger.h
# user_info.h
# vertex.h
)

add_library( ${LIBNAME} SHARED
  data_base.cxx
  event_ass.cxx
  gtruth.cxx
  mcpart.cxx
  mcnu.cxx
  mctruth.cxx
  mcflux.cxx
  mcshower.cxx
  mctrack.cxx
  mctrajectory.cxx
  treenode.cxx
  mctree.cxx
  mceventweight.cxx
  simch.cxx
  simphotons.cxx
  potsummary.cxx
  rawdigit.cxx
  daqheadertimeuboone.cxx
  sparse_vector.cxx
  wire.cxx
  hit.cxx
  crthit.cxx
  crttrack.cxx
  cosmictag.cxx
  ophit.cxx
  opflash.cxx
  cluster.cxx
#  auxsimch.cxx
#  calorimetry.cxx
#  chstatus.cxx
# endpoint2d.cxx
# fifo.cxx
# flashmatch.cxx
# larflow3dhit.cxx
# larflowcluster.cxx
# larmatchsp.cxx
# minos.cxx
# mucsdata.cxx
# mucsreco.cxx
# opdetwaveform.cxx
# partid.cxx
# pcaxis.cxx
# pfpart.cxx
# pixelmask.cxx
# roi.cxx
# seed.cxx
# shower.cxx
# spacepoint.cxx
# #storage_manager.cxx
# #storage_store.cxx
# swtrigger.cxx
# t0.cxx
# track.cxx
# trackmom.cxx
# trigger.cxx
# user_info.cxx
# vertex.cxx )
)

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
target_link_libraries(${LIBNAME} larlite_LArUtil )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR} )
ROOT_GENERATE_DICTIONARY(G__${LIBNAME} ${ALL_HEADERS} LINKDEF LinkDef.h)
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
