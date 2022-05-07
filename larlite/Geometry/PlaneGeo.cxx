#include "larlite/Geometry/PlaneGeo.h"

namespace larlite {
namespace geo {

  PlaneGeo::PlaneGeo()
    : planeid(0),
      cryoid(0),
      view_type(larlite::geo::kUnknown),
      signal_type(larlite::geo::kMysteryType)
  {}

  PlaneGeo::PlaneGeo( int plane_id, int cryo_id, larlite::geo::View_t view, larlite::geo::SigType_t sigtype )
    : planeid(plane_id),
      cryoid(cryo_id),
      view_type(view),
      signal_type(sigtype)
  {
  }

  

}
}
