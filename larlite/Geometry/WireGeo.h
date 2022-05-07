#ifndef __LARLITE_GEOMETRY_WIREGEO_H__
#define __LARLITE_GEOMETRY_WIREGEO_H__

// ROOT
#include "TVector3.h"

namespace larlite {
namespace geo {

  class WireGeo {

  public:
    
    WireGeo();
    WireGeo( int ch_id, int wire_id,
	     int plane_id, int tpc_id, int cryo_id,
	     TVector3 start, TVector3 end );
    
    virtual ~WireGeo() {};

    int channelid; // unique number within detector
    int wireid;    // wire index within plane
    int planeid;   // plane id (index within tpc)
    int tpcid;     // tpc id (index within cryo)
    int cryoid;    // cryo id (index within detector)
    TVector3 fWireStartVtx;
    TVector3 fWireEndVtx;
    TVector3 fWireDir;
    
  };
  
}
}

#endif
