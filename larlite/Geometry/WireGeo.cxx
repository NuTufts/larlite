////////////////////////////////////////////////////////////////////////
/// \file  larcorealg/Geometry/WireGeo.cxx
/// \brief Encapsulate the geometry of a wire
///
/// \author  brebel@fnal.gov
////////////////////////////////////////////////////////////////////////

// class header
#include "larlite/Geometry/WireGeo.h"

// LArSoft libraries
//#include "larcorealg/Geometry/geo_vectors_utils.h" // geo::vect
//#include "larcoreobj/SimpleTypesAndConstants/PhysicalConstants.h" // util ns
#include "larlite/Geometry/geo_vectors_utils.h"
#include "larlite/Base/PhysicalConstants.h" // larlite::util

// framework
//#include "messagefacility/MessageLogger/MessageLogger.h"

// ROOT
//#include "TGeoTube.h"
//#include "TGeoNode.h"

// C/C++ libraries
#include <algorithm> // std::clamp()...
#include <cmath> // std::cos(), std::isfinite()...
#include <sstream>
#include <cassert>

namespace larlite {
namespace geo{

  //-----------------------------------------
  // WireGeo::WireGeo(TGeoNode const& node, geo::TransformationMatrix&& trans)
  //   : fWireNode(&node)
  //   , fTrans(std::move(trans))
  //   , flipped(false)
  // {
  //   fHalfL    = ((TGeoTube*)fWireNode->GetVolume()->GetShape())->GetDZ();

  //   // uncomment the following to check the paths to the wires
  //   //   std::string p(base);
  //   //   for(int i = 0; i <= depth; ++i){
  //   //     p += "/";
  //   //     p += path[i]->GetName();
  //   //   }
  //   //   std::cout << p.c_str() << std::endl;

  //   // determine the orientation of the wire
  //   auto lp = geo::origin<LocalPoint_t>();
  //   fCenter = toWorldCoords(lp);

  //   lp.SetZ(fHalfL);
  //   auto end = toWorldCoords(lp);

  //   fThetaZ = std::acos(std::clamp((end.Z() - fCenter.Z())/fHalfL, -1.0, +1.0));

  //   // check to see if it runs "forward" or "backwards" in z
  //   // check is made looking at the y position of the end point
  //   // relative to the center point because you want to know if
  //   // the end point is above or below the center of the wire in
  //   // the yz plane
  //   if(end.Y() < fCenter.Y()) fThetaZ *= -1.;

  //   //This ensures we are looking at the angle between 0 and Pi
  //   //as if the wire runs at one angle it also runs at that angle +-Pi
  //   if(fThetaZ < 0) fThetaZ += util::pi();
    
  //   assert(std::isfinite(fThetaZ));

  // } // geo::WireGeo::WireGeo()

  //......................................................................
  /**
   * @brief default constructor
   */
  WireGeo::WireGeo()
    : fThetaZ(0),
      fHalfL(0),
      fCenter( {0.0,0.0,0.0} ),
      flipped(false)
  {};

  //......................................................................
  /**
   * @brief simple direct fill constructor
   */
  WireGeo::WireGeo( float thetaz, float halflen, larlite::geo::Point_t& center )
    : fThetaZ(thetaz),
      fHalfL(halflen),
      fCenter( center ),
      flipped(false)
  {};
  
  //......................................................................
  // void WireGeo::GetCenter(double* xyz, double localz) const
  // {
  //   if (localz == 0.) { // if no dislocation is requested, we already have it
  //     larlite::geo::vect::fillCoords(xyz, fCenter);
  //     return;
  //   }

  //   double locz = relLength(localz);
  //   if (std::abs(locz) > fHalfL) {
  //     mf::LogWarning("WireGeo") << "asked for position along wire that"
  //       " extends beyond the wire, returning position at end point";
  //     locz = relLength((locz < 0)? -fHalfL: fHalfL);
  //   }
  //   const double local[3] = { 0., 0., locz };
  //   LocalToWorld(local, xyz);
  // }

  //......................................................................
  // double geo::WireGeo::RMax() const
  // { return ((TGeoTube*)fWireNode->GetVolume()->GetShape())->GetRmax(); }


  // //......................................................................
  // double geo::WireGeo::RMin() const
  //   { return ((TGeoTube*)fWireNode->GetVolume()->GetShape())->GetRmin(); }

  //......................................................................
  double WireGeo::ThetaZ(bool degrees) const
  { return degrees? larlite::util::RadiansToDegrees(fThetaZ): fThetaZ; }

  //......................................................................
  std::string WireGeo::WireInfo
    (std::string indent /* = "" */, unsigned int verbosity /* = 1 */) const
  {
    std::ostringstream sstr;
    PrintWireInfo(sstr, indent, verbosity);
    return sstr.str();
  } // WireGeo::WireInfo()


  //......................................................................
  double WireGeo::DistanceFrom(geo::WireGeo const& wire) const {
    //
    // The algorithm assumes that picking any point on the wire will do,
    // that is, that the wires are parallel.
    //

    if (!isParallelTo(wire)) return 0;

    // a vector connecting to the other wire
    auto const toWire = wire.GetCenter() - GetCenter();

    // the distance is that vector, times the sine of the angle with the wire
    // direction; we get that in a generic way with a cross product.
    // We don't even care about the sign here
    // (if we did, we would do a dot-product with the normal to the plane,
    // and we should get a positive distance if the other wire has larger wire
    // coordinate than this one).
    return toWire.Cross(Direction()).Mag();

  } // WireGeo::DistanceFrom()


  //......................................................................
  void WireGeo::UpdateAfterSorting(geo::WireID const&, bool flip) {

    // flip, if asked
    if (flip) Flip();

  } // WireGeo::UpdateAfterSorting()

  //......................................................................
  void WireGeo::Flip() {
    // we don't need to do much to flip so far:
    // - ThetaZ() is defined in [0, pi], invariant to flipping
    // - we don't change the transformation matrices, that we want to be
    //   untouched and coherent with the original geometry source
    // - center is invariant for flipping
    // - start and end are computed on the fly (taking flipping into account)
    // - ... and we chose to leave half length unsigned and independent

    // change the flipping bit
    flipped = !flipped;

  } // WireGeo::Flip()

  //......................................................................

}
} //namespace larlite
////////////////////////////////////////////////////////////////////////
