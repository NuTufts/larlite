#ifndef __LARLITE_GEO_PLANEGEO_H__
#define __LARLITE_GEO_PLANEGEO_H__

#include "larlite/Base/GeoTypes.h"
#include <vector>
#include "TVector3.h"
#include "larlite/Geometry/WireGeo.h"

namespace larlite {
namespace geo {

  class PlaneGeo {
  public:

    PlaneGeo();
    PlaneGeo( int plane_id, int cryo_id, larlite::geo::View_t view, larlite::geo::SigType_t sigtype );
    virtual ~PlaneGeo() {};

    int planeid;
    int cryoid;
    larlite::geo::View_t    view_type;
    larlite::geo::SigType_t signal_type;
    TVector3 fCenter; 
    TVector3 fNormToCathode; // direction towards the cathode
    TVector3 fWirePitchDir;
    double fWirePitchLen;
    std::vector< TVector3 > fBoundingBox;
    std::vector< WireGeo > fWires_v;
      
  };

}
}

#endif
