/**
 * @file   larcoreobj/SimpleTypesAndConstants/geo_types.cxx
 * @brief  Definition of data types for geometry description (implementation).
 * @see    larcoreobj/SimpleTypesAndConstants/geo_types.h
 * @ingroup Geometry
 *
 * This library is currently header-only and depends only on standard C++.
 *
 */

// header library
#include "GeoTypes.h"

// C++ standard libraries
#include <stdexcept> // std::logic_error


// -----------------------------------------------------------------------------
namespace larlite {
namespace geo {
  std::string SignalTypeName(larlite::geo::SigType_t sigType) {
    switch (sigType) {
    case kInduction:   return "induction";
    case kCollection:  return "collection";
    case kMysteryType: return "unknown";
    } // switch
    throw std::logic_error("geo::SignalTypeName(): unexpected signal type #"
			   + std::to_string(static_cast<int>(sigType))
			   );
  } // geo::SignalTypeName()
}
}

// -----------------------------------------------------------------------------

