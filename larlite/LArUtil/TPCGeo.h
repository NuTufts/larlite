#ifndef __LARLITE_GEO_TPCGEO_H__
#define __LARLITE_GEO_TPCGEO_H__

#include <vector>
#include "TVector3.h"
#include "larlite/LArUtil/PlaneGeo.h"

namespace larlite {
namespace larutil {

  class TPCGeo {
  public:
    
    TPCGeo();
    TPCGeo( int tpc_id, int cryo_id );
    virtual ~TPCGeo() {};

    int tpcid;
    int cryoid;
    TVector3 fCenter;
    TVector3 fHalfLengths;
    TVector3 fDriftDirection;
    std::vector<TVector3> fBounds;
    std::vector< PlaneGeo > planes_v;
    
  };
  
}
}

#endif
