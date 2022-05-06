//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace larlite::geo;

#pragma link C++ struct larlite::geo::WireID+;
#pragma link C++ class larlite::geo::WireGeo+;
#pragma link C++ class larlite::geo::BoundingBoxGeo+; 
//ADD_NEW_CLASS ... do not change this line
#endif








