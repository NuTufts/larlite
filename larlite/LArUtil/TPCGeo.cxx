#include "larlite/LArUtil/TPCGeo.h"

namespace larlite {
namespace larutil {

  TPCGeo::TPCGeo()
    : tpcid(0),
      cryoid(0)
  {}

  TPCGeo::TPCGeo(int tpc_id, int cryo_id)
    : tpcid(tpc_id),
      cryoid(cryo_id)
  {}
  
  
}
}
