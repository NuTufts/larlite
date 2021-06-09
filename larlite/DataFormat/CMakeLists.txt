set(MODULE_NAME DataFormat )

# DEFINE THE TARGET
set( LIBNAME larlite_${MODULE_NAME} )

set( HEADERS auxsimch.h
calorimetry.h
chstatus.h
cluster.h
cosmictag.h
crthit.h
crttrack.h
daqheadertimeuboone.h
data_base.h
DataFormatException.h
DataFormat-TypeDef.h
endpoint2d.h
event_ass.h
fifo.h
flashmatch.h
gtruth.h
hit.h
larflow3dhit.h
larflowcluster.h
larlite_dataformat_utils.h
mceventweight.h
mcflux.h
mcnu.h
mcpart.h
mcshower.h
mctrack.h
mctrajectory.h
mctree.h
mctruth.h
minos.h
mucsdata.h
mucsreco.h
opdetwaveform.h
opflash.h
ophit.h
partid.h
pcaxis.h
pfpart.h
pixelmask.h
potsummary.h
rawdigit.h
roi.h
seed.h
shower.h
simch.h
simphotons.h
spacepoint.h
sparse_vector.h
storage_manager.h
storage_store.h
swtrigger.h
t0.h
track.h
trackmom.h
treenode.h
trigger.h
user_info.h
vertex.h
wire.h )

add_library( ${LIBNAME} SHARED auxsimch.cxx
calorimetry.cxx
chstatus.cxx
cluster.cxx
cosmictag.cxx
crthit.cxx
crttrack.cxx
daqheadertimeuboone.cxx
data_base.cxx
endpoint2d.cxx
event_ass.cxx
fifo.cxx
flashmatch.cxx
gtruth.cxx
hit.cxx
larflow3dhit.cxx
larflowcluster.cxx
mceventweight.cxx
mcflux.cxx
mcnu.cxx
mcpart.cxx
mcshower.cxx
mctrack.cxx
mctrajectory.cxx
mctree.cxx
mctruth.cxx
minos.cxx
mucsdata.cxx
mucsreco.cxx
opdetwaveform.cxx
opflash.cxx
ophit.cxx
partid.cxx
pcaxis.cxx
pfpart.cxx
pixelmask.cxx
potsummary.cxx
rawdigit.cxx
roi.cxx
seed.cxx
shower.cxx
simch.cxx
simphotons.cxx
spacepoint.cxx
sparse_vector.cxx
storage_manager.cxx
storage_store.cxx
swtrigger.cxx
t0.cxx
track.cxx
trackmom.cxx
treenode.cxx
trigger.cxx
user_info.cxx
vertex.cxx
wire.cxx )

# create a property to hold our headers
set_target_properties(${LIBNAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")

# DEFINE SOME VARS THAT MIGHT BE USEFUL FOR SUBMODULES
message("python include: ${PYTHON_INCLUDE_PATH}")

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
