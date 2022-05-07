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
#pragma link C++ class larlite::geo::WireGeo+;
#pragma link C++ class larlite::geo::PlaneGeo+;
#pragma link C++ class larlite::geo::TPCGeo+;
#pragma link C++ class larlite::geo::CryoGeo+;
#pragma link C++ class std::vector<larlite::geo::WireGeo>+;
#pragma link C++ class std::vector<larlite::geo::PlaneGeo>+;
#pragma link C++ class std::vector<larlite::geo::TPCGeo>+;
#pragma link C++ class std::vector<larlite::geo::CryoGeo>+;
//ADD_NEW_CLASS ... do not change this line
#endif








