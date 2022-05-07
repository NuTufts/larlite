#ifndef __LARLITE_GEO_CRYOGEO_H__
#define __LARLITE_GEO_CRYOGEO_H__

#include <vector>
#include "TVector3.h"
#include "larlite/Geometry/TPCGeo.h"
#include "larlite/Geometry/OpDetGeo.h"

namespace larlite {
namespace geo {

  class CryoGeo {
    
  public:
    CryoGeo();
    CryoGeo( int cryo_id );
    virtual ~CryoGeo() {};

    int cryoid;
    TVector3 fCenter;
    std::vector<double> fHalfLengths;
    std::vector<TVector3> fBounds;
    std::vector<TPCGeo> tpc_v;
    std::vector<OpDetGeo> opdet_v;
    
  };
  
}
}

#endif
