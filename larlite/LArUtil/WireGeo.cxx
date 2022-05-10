#include "larlite/LArUtil/WireGeo.h"

namespace larlite {
namespace larutil {

  WireGeo::WireGeo()
    : channelid(0),
      wireid(0),
      planeid(0),
      tpcid(0),
      cryoid(0),
      fWireStartVtx( {0,0,0} ),
      fWireEndVtx( {0,0,0,} ),
      fWireDir( {0,0,0} )
  {}

  WireGeo::WireGeo( int ch_id, int wire_id, int plane_id, int tpc_id, int cryo_id,
		    TVector3 start, TVector3 end )
    : channelid(ch_id),
      wireid(wire_id),
      planeid(plane_id),
      tpcid(tpc_id),
      cryoid(cryo_id),
      fWireStartVtx( start ),
      fWireEndVtx( end ),
      fWireDir( {0,0,0} )
  {
    fWireDir = fWireEndVtx-fWireStartVtx;
    if ( fWireDir.Mag() ) {
      float mag = fWireDir.Mag();
      for (int i=0; i<3; i++)
	fWireDir[i] /= mag;
    }
  }
  
}
}
