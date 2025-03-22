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
  auxsimch.h
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
  seed.h
  spacepoint.h
  track.h
  shower.h
  vertex.h
  endpoint2d.h
  calorimetry.h
  partid.h
  pfpart.h
  user_info.h
  trigger.h
  minos.h
  fifo.h
  pcaxis.h
  flashmatch.h
  trackmom.h
  opdetwaveform.h
  mucsdata.h
  mucsreco.h
  vertex.h
  roi.h
  t0.h
  swtrigger.h
  larflow3dhit.h
  larflowcluster.h
  pixelmask.h
  larmatchsp.h
#  chstatus.h
# DataFormat-TypeDef.h
# #storage_manager.h
# #storage_store.h
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
  auxsimch.cxx
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
  seed.cxx
  spacepoint.cxx
  track.cxx
  shower.cxx
  vertex.cxx
  endpoint2d.cxx
  calorimetry.cxx
  partid.cxx
  pfpart.cxx
  user_info.cxx
  trigger.cxx
  minos.cxx
  fifo.cxx
  pcaxis.cxx
  flashmatch.cxx
  trackmom.cxx
  opdetwaveform.cxx
  mucsdata.cxx
  mucsreco.cxx
  vertex.cxx
  roi.cxx
  t0.cxx
  swtrigger.cxx
  larflow3dhit.cxx
  larflowcluster.cxx
  pixelmask.cxx
  larmatchsp.cxx
#  chstatus.cxx
# #storage_manager.cxx
# #storage_store.cxx
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
