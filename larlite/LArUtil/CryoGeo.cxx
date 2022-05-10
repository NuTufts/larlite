#include "larlite/LArUtil/CryoGeo.h"

namespace larlite {
namespace larutil {

  CryoGeo::CryoGeo()
    : cryoid(0)
  {}

  CryoGeo::CryoGeo( int cryo_id )
    : cryoid( cryo_id )
  {}
  
}
}
