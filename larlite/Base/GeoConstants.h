/**
 * \file GeoConstants.h
 *
 * \ingroup Base
 * 
 * \brief defines geometry related constants
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Base

    @{*/

#ifndef LARLITE_GEOCONSTANTS_H
#define LARLITE_GEOCONSTANTS_H
#include "GeoTypes.h"

// #include <string>

// /// Namespace of everything in this framework
namespace larlite{
namespace geo {

  enum DetId_t {
    kBo,         ///< Bo id
    kArgoNeuT,   ///< ArgoNeuT id
    kMicroBooNE, ///< MicroBoone id
    kLBNE10kt,   ///< LBNE 10kt id
    kJP250L,     ///< JPARC 250 L id
    kLBNE35t,    ///< 35t prototype id
    kLBNE34kt,   ///< LBNE 34kt id
    kCSU40L,     ///< CSU 40 L id
    kLArIAT,     ///< LArIAT id
    kICARUS,     ///< ICARUS T600 id
    kSBND,       ///< SBND
    kDetIdMax 
  };
    
//     /// Enumerate the possible plane projections
//     enum View_t {
//       kU,       ///< planes which measure U
//       kV,       ///< planes which measure V
//       kW,       ///< soon to be deprecated, planes which measure W (third view for Bo, MicroBooNE, etc)
//       kZ=kW,    ///< planes which measure Z direction (ie wires are vertical)
//       k3D,      ///< 3 dimensional objects, potentially hits, clusters, prongs, etc
//       kUnknown  ///< unknown view
//     };
    
//     /// Enumerate the possible signal types
//     enum SigType_t {
//       kInduction,   ///< signal from induction planes
//       kCollection,  ///< signal from collection planes
//       kMysteryType  ///< who knows?
//     };

//     typedef enum coordinates {
//       kXCoord, ///< X coordinate.
//       kYCoord, ///< Y coordinate.
//       kZCoord  ///< Z coordinate.
//     } Coord_t;    

//     typedef enum _plane_orient {
//       kHorizontal, ///< Planes that are in the horizontal plane.
//       kVertical    ///< Planes that are in the vertical plane (e.g. ArgoNeuT).
//     } Orient_t;
    
//     typedef enum _plane_sigtype {
//       kInduction,   ///< Signal from induction planes.
//       kCollection,  ///< Signal from collection planes.
//       kMysteryType  ///< Who knows?
//     } SigType_t;
    
//   /**
//    * @brief Drift direction: positive or negative
//    *
//    * Do not use this type to distinguish different drift axes: e.g., negative
//    * x drift and negative z drift are both by `kNeg`.
//    */
//     typedef enum driftdir {
//       kUnknownDrift, ///< Drift direction is unknown.
//       kPos,          ///< Drift towards positive values.
//       kNeg,          ///< Drift towards negative values.
//       kPosX = kPos,  ///< Drift towards positive X values.
//       kNegX = kNeg   ///< Drift towards negative X values.
//     } DriftDirection_t;
    
  
//     /// Numerical description of geometry element "level".
//     /// The "detector" level is noticeably missing.
//     struct ElementLevel {
      
//       using Level_t = std::size_t;
      
//       static constexpr Level_t Cryostat   = 0U;
//       static constexpr Level_t OpDet      = 1U;
//       static constexpr Level_t TPC        = 1U;
//       static constexpr Level_t Plane      = 2U;
//       static constexpr Level_t Wire       = 3U;
//       static constexpr Level_t NLevels = 4U;
      
//     }; // struct ElementLevel
    
}
}
#endif
/** @} */ // end of doxygen group
