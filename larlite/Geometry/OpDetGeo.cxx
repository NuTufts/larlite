#include "larlite/Geometry/OpDetGeo.h"

namespace larlite {
namespace geo {

  OpDetGeo::OpDetGeo()
    : opdetid(0),
      cryoid(0),
      fCenter( {0,0,0} )
  {};

  OpDetGeo::OpDetGeo( int op_id, int cryo_id, const TVector3& center )
    : opdetid( op_id ),
      cryoid( cryo_id ),
      fCenter(center)
  {
  }


  
}
}
