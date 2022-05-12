#ifndef LARLITE_LARUTIL_GEOMETRY_CXX
#define LARLITE_LARUTIL_GEOMETRY_CXX

#include "Geometry.h"

#include "TFile.h"

#include "InvalidWireError.h"
#include "LArUtilException.h"
#include "larlite/Base/DataFormatConstants.h"

namespace larlite {
namespace larutil {

  Geometry* Geometry::_me = 0; ///< pointer to current instance
  std::vector<Geometry*> Geometry::_detector_geo_v; ///< repository of different geometries
  
  Geometry::Geometry( larlite::geo::DetId_t detid, bool force_reload )
  {
    _name = "Geometry";
    LoadData(detid,force_reload);
  }

  bool Geometry::LoadData( larlite::geo::DetId_t detid, bool force_reload)
  {

    // Load the maps
    ReadTree();

    // determine max channel, collect PlaneIDs
    int max_channel = 0;
    fSimplePlaneIDToPlaneID.clear();
    for (auto const& cryogeo :  fCryo_v ) {
      for (auto const& tpcgeo : cryogeo.tpc_v ) {
	for ( auto const& planegeo : tpcgeo.planes_v ) {
	  larlite::geo::PlaneID planeid( planegeo.cryoid, planegeo.tpcid, planegeo.planeid );
	  fSimplePlaneIDToPlaneID.push_back( planeid );
	  for ( auto const& wiregeo : planegeo.fWires_v ) {
	    std::vector< int > ptc_id = { planegeo.planeid, planegeo.tpcid, planegeo.cryoid };
	    int ch = wiregeo.channelid;
	    if ( ch>max_channel )
	      max_channel = ch;
	  }
	}
      }
    }
    print(larlite::msg::kNORMAL, __FUNCTION__,Form(" max channel numbers: %d",max_channel));
    std::sort( fSimplePlaneIDToPlaneID.begin(), fSimplePlaneIDToPlaneID.end() );
    for (int i=0; i<fSimplePlaneIDToPlaneID.size(); i++) {
      fPlaneIDToSimplePlaneID[ fSimplePlaneIDToPlaneID[i] ] = i;
    }
    
    // make channelid -> planeid map
    fChannelToPlaneMap.resize( max_channel+1 );
    fChannelToWireMap.resize( max_channel+1 );
    fChannelToWireID.resize( max_channel+1 );
    for (auto const& cryogeo :  fCryo_v ) {
      for (auto const& tpcgeo : cryogeo.tpc_v ) {
	for ( auto const& planegeo : tpcgeo.planes_v ) {
	  for ( auto const& wiregeo : planegeo.fWires_v ) {
	    int ch = wiregeo.channelid;
	    fChannelToPlaneMap[ch] = larlite::geo::PlaneID( planegeo.cryoid, planegeo.tpcid, planegeo.planeid );
	    fChannelToWireMap[ch]  = (UShort_t)wiregeo.wireid;
	    std::vector<int> wid = {planegeo.cryoid, planegeo.tpcid, planegeo.planeid, wiregeo.wireid};
	    fWireIDToChannel[ wid ] = ch;
	    fChannelToWireID[ ch ]  = wid;
	  }
	}
      }
    }
    
    print(larlite::msg::kNORMAL, __FUNCTION__," geometry loaded successfully.");
    return true;
  }
      
  void Geometry::ClearData()
  {
    fChannelToPlaneMap.clear();
    fChannelToWireMap.clear();
    fChannelToWireID.clear();
    fWireIDToChannel.clear();
    fSimplePlaneIDToPlaneID.clear();
    fPlaneIDToSimplePlaneID.clear();
    
    // fPlaneWireToChannelMap.clear();
    // fOpChannel2OpDet.clear();
  }
  
  bool Geometry::ReadTree()
  {
    ClearData();

    std::string filename = Form("%s/LArUtil/dat/%s",
				getenv("LARLITE_COREDIR"),
				::larutil::kGEO_DATA_FILENAME[::larutil::LArUtilConfig::Detector()].c_str());

    print(larlite::msg::kNORMAL, __FUNCTION__,
	  Form("Loading geo data for DetID=%d ...\n file=%s ", ::larutil::LArUtilConfig::Detector(), filename.c_str()));
    
    TFile rfile = TFile(filename.c_str());
    std::vector< larlite::larutil::CryoGeo >* _p_cryo_v = 0;
    TTree* geodata = (TTree*)rfile.Get("geodata");
    geodata->SetBranchAddress( "cryo_v", &_p_cryo_v );
    geodata->GetEntry(0);
    
    fCryo_v = *_p_cryo_v; // a copy
    
    rfile.Close();
    
    return true;
  }

  /**
   * @brief Check if a combination of plane, tpc, and cryostat id is valid
   *
   */
  bool Geometry::IsValid( int plane_id, int tpc_id, int cryo_id ) const
  {
    if ( cryo_id<0 || cryo_id>=(int)fCryo_v.size() )
      return false;

    auto const& cryogeo = fCryo_v[cryo_id];

    if ( tpc_id<0 || tpc_id>=(int)cryogeo.tpc_v.size() )
      return false;

    auto const& tpcgeo = cryogeo.tpc_v[tpc_id];

    if ( plane_id<0 || plane_id>=tpcgeo.planes_v.size() )
      return false;

    return true;
  }  

  /*
   * @brief return cryo stat bounds
   *
   * returns 6 pairs of numbers
   * the numbers are ordered as (x_min, x_max, y_min, y_max, z_min, z_max)
   *
   */
  void Geometry::CryostatBoundaries(Double_t* boundaries, int cryo_id) const
  {
    if ( cryo_id<0 || cryo_id>=(int)fCryo_v.size() )
      throw LArUtilException(Form("Invalid cryostat id: %d", cryo_id));
    auto const& cryogeo = fCryo_v[cryo_id];
    for (int i=0; i<3; i++) {
      boundaries[2*i]   = cryogeo.fBounds[0][i];
      boundaries[2*i+1] = cryogeo.fBounds[1][i];
    }
  }
  
  /**
   * @brief Get the number of wires on a plane
   *
   */
  UInt_t Geometry::Nwires(UInt_t plane_id, UInt_t tpc_id, UInt_t cryo_id ) const
  {

    if ( !IsValid(plane_id, tpc_id, cryo_id) ) {
      throw LArUtilException(Form("Invalid plane,tpc,cryo ID :%d, %d, %d", plane_id, tpc_id, cryo_id));
      return larlite::data::kINVALID_UINT;      
    }

    auto const& cryogeo  = fCryo_v[cryo_id];
    auto const& tpcgeo   = cryogeo.tpc_v[tpc_id];
    auto const& planegeo = tpcgeo.planes_v[plane_id];
    
    return planegeo.fWires_v.size();
  }

  /**
   * @brief Using a channel number, get the PlaneID struct
   */
  larlite::geo::PlaneID  Geometry::ChannelToPlane(const UInt_t ch) const
  {
    if (ch >= fChannelToPlaneMap.size()) {
      throw LArUtilException(Form("Invalid channel number: %d", ch));
    }
    return fChannelToPlaneMap.at(ch);
  }

  /**
   * @brief Given a channel number, get the wire number in the same plane
   *
   * for turning channel number into plane wire index, the latter
   * often being the index in the data array.
   *
   */
  UInt_t Geometry::ChannelToWire(const UInt_t ch)const
  {
    if (ch >= fChannelToWireMap.size()) {
      throw LArUtilException(Form("Invalid channel number: %d", ch));
      return larlite::data::kINVALID_CHAR;
    }
    
    return fChannelToWireMap.at(ch);
  }

  larlite::geo::WireID Geometry::ChannelToWireID(const UInt_t ch)const
  {
    if (ch >= fChannelToWireMap.size()) {
      throw LArUtilException(Form("Invalid channel number: %d", ch));
      return larlite::geo::WireID();
    }

    auto const& widv = fChannelToWireID.at(ch);
    
    larlite::geo::WireID wireID( widv[0], widv[1], widv[2], widv[3] );
    
    return wireID;
  }

// larlite::geo::SigType_t Geometry::SignalType(const UInt_t ch) const
// {
//   if (ch >= fChannelToPlaneMap.size()) {
//     throw LArUtilException(Form("Invalid Channel number :%d", ch));
//     return larlite::geo::kMysteryType;
//   }

//   return fSignalType.at(fChannelToPlaneMap.at(ch));
// }

// larlite::geo::SigType_t Geometry::PlaneToSignalType(const UChar_t plane) const
// {
//   if (plane >= fSignalType.size()) {
//     throw LArUtilException(Form("Invalid Plane number: %d", plane));
//     return larlite::geo::kMysteryType;
//   }

//   return fSignalType.at(plane);
// }

// larlite::geo::View_t Geometry::View(const UInt_t ch) const
// {
//   if (ch >= fChannelToPlaneMap.size()) {
//     throw LArUtilException(Form("Invalid Channel number :%d", ch));
//     return larlite::geo::kUnknown;
//   }

//   return fViewType.at(fChannelToPlaneMap.at(ch));
// }

// larlite::geo::View_t Geometry::PlaneToView(const UChar_t plane) const
// {
//   if (plane >= fViewType.size()) {
//     throw LArUtilException(Form("Invalid Plane number: %d", plane));
//     return larlite::geo::kUnknown;
//   }

//   return fViewType.at(plane);
// }

// std::set<larlite::geo::View_t> const Geometry::Views() const
// {
//   std::set<larlite::geo::View_t> views;
//   for (auto const v : fViewType) views.insert(v);
//   return views;
// }

// UInt_t Geometry::PlaneWireToChannel(const UInt_t plane,
//                                     const UInt_t wire) const
// {

//   if (plane >= Nplanes() || fPlaneWireToChannelMap.at(plane).size() <= wire) {
//     throw LArUtilException(Form("Invalid (plane, wire) = (%d, %d)", plane, wire));
//     return larlite::data::kINVALID_UINT;
//   }
//   return fPlaneWireToChannelMap.at(plane).at(wire);
// }

// UInt_t Geometry::NearestChannel(const Double_t worldLoc[3],
//                                 const UInt_t PlaneNo) const
// {
//   return PlaneWireToChannel(PlaneNo, NearestWire(worldLoc, PlaneNo));
// }

// UInt_t Geometry::NearestChannel(const std::vector<Double_t> &worldLoc,
//                                 const UInt_t PlaneNo) const
// {
//   return PlaneWireToChannel(PlaneNo, NearestWire(worldLoc, PlaneNo));
// }

// UInt_t Geometry::NearestChannel(const TVector3 &worldLoc,
//                                 const UInt_t PlaneNo) const
// {
//   return PlaneWireToChannel(PlaneNo, NearestWire(worldLoc, PlaneNo));
// }

// UInt_t Geometry::NearestWire(const Double_t worldLoc[3],
//                              const UInt_t PlaneNo) const
// {
//   TVector3 loc(worldLoc);
//   return NearestWire(loc, PlaneNo);
// }

// UInt_t Geometry::NearestWire(const std::vector<Double_t> &worldLoc,
//                              const UInt_t PlaneNo) const
// {
//   TVector3 loc(&worldLoc[0]);
//   return NearestWire(loc, PlaneNo);
// }

// UInt_t Geometry::NearestWire(const TVector3 &worldLoc,
//                              const UInt_t PlaneNo) const
// {
//   int NearestWireNumber = int(nearbyint(worldLoc[1] * fOrthVectorsY.at(PlaneNo)
//                                         + worldLoc[2] * fOrthVectorsZ.at(PlaneNo)
//                                         - fFirstWireProj.at(PlaneNo)));

//   unsigned int wireNumber = (unsigned int) NearestWireNumber;

//   if (NearestWireNumber < 0 ||
//       NearestWireNumber >= (int)(this->Nwires(PlaneNo)) ) {

//     if (NearestWireNumber < 0) wireNumber = 0;
//     else wireNumber = this->Nwires(PlaneNo) - 1;

//     larutil::InvalidWireError err(Form("Can't find nearest wire for (%g,%g,%g)",
// 				       worldLoc[0], worldLoc[1], worldLoc[2]));
//     err.better_wire_number = wireNumber;

//     throw err;
//   }
//   /*
//   std::cout<<"NearestWireID"<<std::endl;
//   std::cout<<Form("(%g,%g,%g) position ... using (%g,%g,%g) ... Wire %d Plane %d",
//                   worldLoc[0],worldLoc[1],worldLoc[2],
//                   fOrthVectorsY[PlaneNo],
//                   fOrthVectorsZ[PlaneNo],
//                   fFirstWireProj[PlaneNo],
//                   wireNumber,PlaneNo)
//            << std::endl;
//   */
//   return wireNumber;
// }

// /// exact wire coordinate (fractional wire) to input world coordinates
// Double_t Geometry::WireCoordinate(const Double_t worldLoc[3],
//                                   const UInt_t   PlaneNo) const
// {
//   TVector3 loc(worldLoc);
//   return WireCoordinate(loc, PlaneNo);
// }

// /// exact wire coordinate (fractional wire) to input world coordinate
// Double_t Geometry::WireCoordinate(const std::vector<Double_t> &worldLoc,
//                                   const UInt_t  PlaneNo) const
// {
//   TVector3 loc(&worldLoc[0]);
//   return WireCoordinate(loc, PlaneNo);
// }

// /// exact wire coordinate (fractional wire) to input world coordinates
// Double_t Geometry::WireCoordinate(const TVector3& worldLoc,
//                                   const UInt_t PlaneNo) const
// {

//   Double_t NearestWireNumber = worldLoc[1] * fOrthVectorsY.at(PlaneNo)
//                                + worldLoc[2] * fOrthVectorsZ.at(PlaneNo)
//                                - fFirstWireProj.at(PlaneNo);


//   /*
//   std::cout<<"NearestWireID"<<std::endl;
//   std::cout<<Form("(%g,%g,%g) position ... using (%g,%g,%g) ... Wire %d Plane %d",
//                   worldLoc[0],worldLoc[1],worldLoc[2],
//                   fOrthVectorsY[PlaneNo],
//                   fOrthVectorsZ[PlaneNo],
//                   fFirstWireProj[PlaneNo],
//                   wireNumber,PlaneNo)
//            << std::endl;
//   */
//   return NearestWireNumber;
// }


// // distance between planes p1 < p2
// Double_t Geometry::PlanePitch(const UChar_t p1, const UChar_t p2) const
// {
//   if ( p1 == p2 ) return 0;
//   else if ( (p1 == 0 && p2 == 1) || (p1 == 1 && p2 == 0) ) return fPlanePitch.at(0);
//   else if ( (p1 == 1 && p2 == 2) || (p1 == 2 && p2 == 1) ) return fPlanePitch.at(1);
//   else if ( (p1 == 0 && p2 == 2) || (p1 == 2 && p2 == 0) ) return fPlanePitch.at(2);
//   else {
//     throw LArUtilException("Plane number > 2 not supported!");
//     return larlite::data::kINVALID_DOUBLE;
//   }
// }

// Double_t Geometry::WirePitch(const UInt_t  w1,
//                              const UInt_t  w2,
//                              const UChar_t plane) const
// {
//   if ( w1 > w2 && w1 >= fPlaneWireToChannelMap.at(plane).size() ) {
//     throw LArUtilException(Form("Invalid wire number: %d", w1));
//     return larlite::data::kINVALID_DOUBLE;
//   }
//   if ( w2 > w1 && w2 >= fPlaneWireToChannelMap.at(plane).size() ) {
//     throw LArUtilException(Form("Invalid wire number: %d", w2));
//     return larlite::data::kINVALID_DOUBLE;
//   }

//   return ( w1 < w2 ? (w2 - w1) * (fWirePitch.at(fViewType.at(plane))) : (w1 - w2) * (fWirePitch.at(fViewType.at(plane))));
// }

// /// assumes all planes in a view have the same pitch
// Double_t   Geometry::WirePitch(const larlite::geo::View_t view) const
// {
//   if ((size_t)view > Nviews()) {
//     throw LArUtilException(Form("Invalid view: %d", view));
//     return larlite::data::kINVALID_DOUBLE;
//   }

//   return fWirePitch.at((size_t)view);
// }

// Double_t   Geometry::WireAngleToVertical(larlite::geo::View_t view) const
// {
//   if ((size_t)view > Nviews()) {
//     throw LArUtilException(Form("Invalid view: %d", view));
//     return larlite::data::kINVALID_DOUBLE;
//   }

//   return fWireAngle.at((size_t)view);
// }


// void Geometry::WireEndPoints(const UChar_t plane,
//                              const UInt_t wire,
//                              Double_t *xyzStart, Double_t *xyzEnd) const
// {

//   if (plane >= fWireStartVtx.size())  {
//     throw LArUtilException(Form("Plane %d invalid!", plane));
//     return;
//   }
//   if (wire >= fWireStartVtx.at(plane).size()) {
//     throw LArUtilException(Form("Wire %d invalid!", wire));
//     return;
//   }

//   xyzStart[0] = fWireStartVtx.at(plane).at(wire).at(0);
//   xyzStart[1] = fWireStartVtx.at(plane).at(wire).at(1);
//   xyzStart[2] = fWireStartVtx.at(plane).at(wire).at(2);
//   xyzEnd[0]   = fWireEndVtx.at(plane).at(wire).at(0);
//   xyzEnd[1]   = fWireEndVtx.at(plane).at(wire).at(1);
//   xyzEnd[2]   = fWireEndVtx.at(plane).at(wire).at(2);

// }

// bool Geometry::ChannelsIntersect(const UInt_t c1,
//                                  const UInt_t c2,
//                                  Double_t &y, Double_t &z) const
// {
//   if (c1 == c2) {
//     throw LArUtilException("Same channel does not intersect!");
//     return false;
//   }

//   if ( c1 >= fChannelToPlaneMap.size() || c2 >= fChannelToPlaneMap.size() ) {
//     throw LArUtilException(Form("Invalid channels : %d and %d", c1, c2));
//     return false;
//   }
//   if ( fViewType.at(fChannelToPlaneMap.at(c1)) == fViewType.at(fChannelToPlaneMap.at(c2)) ) {
//     return false;
//   }

//   UInt_t w1 = fChannelToWireMap.at(c1);
//   UInt_t w2 = fChannelToWireMap.at(c2);

//   UChar_t p1 = fChannelToPlaneMap.at(c1);
//   UChar_t p2 = fChannelToPlaneMap.at(c2);

//   larlite::geo::View_t v1 = fViewType.at(p1);
//   larlite::geo::View_t v2 = fViewType.at(p2);

//   Double_t start1[3] = {0.};
//   Double_t start2[3] = {0.};
//   Double_t end1[3] = {0.};
//   Double_t end2[3] = {0.};

//   WireEndPoints(p1, w1, start1, end1);
//   WireEndPoints(p2, w2, start2, end2);

//   // if endpoint of one input wire is within range of other input wire in
//   // BOTH y AND z, wires overlap
//   bool overlapY = (ValueInRange(start1[1], start2[1], end2[1]) || ValueInRange(end1[1], start2[1], end2[1]));
//   bool overlapZ = (ValueInRange(start1[2], start2[2], end2[2]) || ValueInRange(end1[2], start2[2], end2[2]));

//   bool overlapY_rev = (ValueInRange(start2[1], start1[1], end1[1]) || ValueInRange(end2[1], start1[1], end1[1]));
//   bool overlapZ_rev = (ValueInRange(start2[2], start1[2], end1[2]) || ValueInRange(end2[2], start1[2], end1[2]));

//   // override y overlap checks if a vertical plane exists:
//   if ( fWireAngle.at(v1) == TMath::Pi() / 2 || fWireAngle.at(v2) == TMath::Pi() / 2 ) {
//     overlapY     = true;
//     overlapY_rev = true;
//   }

//   //catch to get vertical wires, where the standard overlap might not work, Andrzej
//   if (std::abs(start2[2] - end2[2]) < 0.01) overlapZ = overlapZ_rev;


//   if (overlapY && overlapZ) {
//     IntersectionPoint(w1, w2, p1, p2,
//                       start1, end1,
//                       start2, end2,
//                       y, z);
//     return true;
//   }

//   else if (overlapY_rev && overlapZ_rev) {
//     this->IntersectionPoint(w2, w1, p2, p1,
//                             start2, end2,
//                             start1, end1,
//                             y, z);
//     return true;
//   }

//   return false;

// }

// void Geometry::IntersectionPoint(const UInt_t  wire1,  const UInt_t  wire2,
//                                  const UChar_t plane1, const UChar_t plane2,
//                                  Double_t start_w1[3], Double_t end_w1[3],
//                                  Double_t start_w2[3], Double_t end_w2[3],
//                                  Double_t &y, Double_t &z) const
// {

//   larlite::geo::View_t v1 = fViewType.at(plane1);
//   larlite::geo::View_t v2 = fViewType.at(plane2);
//   //angle of wire1 wrt z-axis in Y-Z plane...in radians
//   Double_t angle1 = fWireAngle.at(v1);
//   //angle of wire2 wrt z-axis in Y-Z plane...in radians
//   Double_t angle2 = fWireAngle.at(v2);

//   if (angle1 == angle2) return; //comparing two wires in the same plane...pointless.

//   //coordinates of "upper" endpoints...(z1,y1) = (a,b) and (z2,y2) = (c,d)
//   double a = 0.;
//   double b = 0.;
//   double c = 0.;
//   double d = 0.;
//   double angle = 0.;
//   double anglex = 0.;

//   // below is a special case of calculation when one of the planes is vertical.
//   angle1 < angle2 ? angle = angle1 : angle = angle2;//get angle closest to the z-axis

//   // special case, one plane is vertical
//   if (angle1 == TMath::Pi() / 2 || angle2 == TMath::Pi() / 2) {
//     if (angle1 == TMath::Pi() / 2) {

//       anglex = (angle2 - TMath::Pi() / 2);
//       a = end_w1[2];
//       b = end_w1[1];
//       c = end_w2[2];
//       d = end_w2[1];
//       // the if below can in principle be replaced by the sign of anglex (inverted)
//       // in the formula for y below. But until the geometry is fully symmetric in y I'm
//       // leaving it like this. Andrzej
//       if ((anglex) > 0 ) b = start_w1[1];

//     }
//     else if (angle2 == TMath::Pi() / 2) {
//       anglex = (angle1 - TMath::Pi() / 2);
//       a = end_w2[2];
//       b = end_w2[1];
//       c = end_w1[2];
//       d = end_w1[1];
//       // the if below can in principle be replaced by the sign of anglex (inverted)
//       // in the formula for y below. But until the geometry is fully symmetric in y I'm
//       // leaving it like this. Andrzej
//       if ((anglex) > 0 ) b = start_w2[1];
//     }

//     y = b + ((c - a) - (b - d) * tan(anglex)) / tan(anglex);
//     z = a;   // z is defined by the wire in the vertical plane

//     return;
//   }

//   // end of vertical case
//   z = 0; y = 0;

//   if (angle1 < (TMath::Pi() / 2.0)) {
//     c = end_w1[2];
//     d = end_w1[1];
//     a = start_w2[2];
//     b = start_w2[1];
//   }
//   else {
//     c = end_w2[2];
//     d = end_w2[1];
//     a = start_w1[2];
//     b = start_w1[1];
//   }

//   //Intersection point of two wires in the yz plane is completely
//   //determined by wire endpoints and angle of inclination.
//   z = 0.5 * ( c + a + (b - d) / TMath::Tan(angle) );
//   y = 0.5 * ( b + d + (a - c) * TMath::Tan(angle) );

//   return;

// }

// // Added shorthand function where start and endpoints are looked up automatically
// //  - whether to use this or the full function depends on optimization of your
// //    particular algorithm.  Ben J, Oct 2011
// //--------------------------------------------------------------------
// void Geometry::IntersectionPoint(const UInt_t  wire1,  const UInt_t  wire2,
//                                  const UChar_t plane1, const UChar_t plane2,
//                                  Double_t &y, Double_t &z) const

// {
//   double WireStart1[3] = {0.};
//   double WireStart2[3] = {0.};
//   double WireEnd1[3]   = {0.};
//   double WireEnd2[3]   = {0.};

//   this->WireEndPoints(plane1, wire1, WireStart1, WireEnd1);
//   this->WireEndPoints(plane2, wire2, WireStart2, WireEnd2);
//   this->IntersectionPoint(wire1, wire2, plane1, plane2,
//                           WireStart1, WireEnd1,
//                           WireStart2, WireEnd2, y, z);
// }

// UInt_t Geometry::GetClosestOpDet(const Double_t *xyz) const
// {
//   Double_t dist2      = 0;
//   Double_t min_dist2  = larlite::data::kINVALID_DOUBLE;
//   UInt_t   closest_ch = larlite::data::kINVALID_UINT;
//   for (size_t ch = 0; ch < fOpDetVtx.size(); ++ch) {

//     dist2 =
//       pow(xyz[0] - fOpDetVtx.at(ch).at(0), 2) +
//       pow(xyz[1] - fOpDetVtx.at(ch).at(1), 2) +
//       pow(xyz[2] - fOpDetVtx.at(ch).at(2), 2);

//     if ( dist2 < min_dist2 ) {

//       min_dist2 = dist2;
//       closest_ch = ch;

//     }
//   }

//   return closest_ch;
// }

// UInt_t Geometry::GetClosestOpDet(const Double_t *xyz, Double_t &dist) const
// {
//   Double_t min_dist2  = larlite::data::kINVALID_DOUBLE;
//   UInt_t   closest_ch = larlite::data::kINVALID_UINT;
//   for (size_t ch = 0; ch < fOpDetVtx.size(); ++ch) {

//     dist =
//       pow(xyz[0] - fOpDetVtx.at(ch).at(0), 2) +
//       pow(xyz[1] - fOpDetVtx.at(ch).at(1), 2) +
//       pow(xyz[2] - fOpDetVtx.at(ch).at(2), 2);

//     if ( dist < min_dist2 ) {

//       min_dist2 = dist;
//       closest_ch = ch;

//     }
//   }
//   dist = sqrt(dist);
//   return closest_ch;
// }

// UInt_t Geometry::OpDetFromOpChannel(UInt_t ch) const
// {
//   if (ch >= fOpChannel2OpDet.size())
//     throw LArUtilException(Form("Invalid OpChannel: %d", ch));
//   return fOpChannel2OpDet[ch];
// }

// void Geometry::GetOpChannelPosition(const UInt_t i, Double_t *xyz) const
// {
//   if ( i >= fOpChannelVtx.size() ) {
//     throw LArUtilException(Form("Invalid PMT channel number: %d", i));
//     xyz[0] = larlite::data::kINVALID_DOUBLE;
//     xyz[0] = larlite::data::kINVALID_DOUBLE;
//     xyz[0] = larlite::data::kINVALID_DOUBLE;
//     return;
//   }

//   xyz[0] = fOpChannelVtx.at(i).at(0);
//   xyz[1] = fOpChannelVtx.at(i).at(1);
//   xyz[2] = fOpChannelVtx.at(i).at(2);
//   return;
// }

// void Geometry::GetOpDetPosition(const UInt_t i, Double_t *xyz) const
// {
//   if ( i >= fOpDetVtx.size() ) {
//     throw LArUtilException(Form("Invalid PMT channel number: %d", i));
//     xyz[0] = larlite::data::kINVALID_DOUBLE;
//     xyz[0] = larlite::data::kINVALID_DOUBLE;
//     xyz[0] = larlite::data::kINVALID_DOUBLE;
//     return;
//   }

//   xyz[0] = fOpDetVtx.at(i).at(0);
//   xyz[1] = fOpDetVtx.at(i).at(1);
//   xyz[2] = fOpDetVtx.at(i).at(2);
//   return;
// }

// void Geometry::GetOpChannelPosition(const UInt_t i, std::vector<double>& xyz) const
// {
//   xyz.resize(3,0.);
//   GetOpChannelPosition(i,&xyz[0]);
//   return;
// }

// void Geometry::GetOpDetPosition(const UInt_t i, std::vector<double>& xyz) const
// {
//   xyz.resize(3,0.);
//   GetOpDetPosition(i,&xyz[0]);
//   return;
// }

  //const std::vector<Double_t>& Geometry::PlaneOriginVtx(UChar_t plane)
  //{
//   if (plane >= fPlaneOriginVtx.size()) {
//     throw LArUtilException(Form("Invalid plane number: %d", plane));
//     fPlaneOriginVtx.push_back(std::vector<Double_t>(3, larlite::data::kINVALID_DOUBLE));
//     return fPlaneOriginVtx.at(this->Nplanes());
//   }

//   return fPlaneOriginVtx.at(plane);
// }

  void Geometry::PlaneOriginVtx(UInt_t plane, Double_t *vtx, UInt_t tpc, UInt_t cryo) const
  {

    if ( !IsValid( plane, tpc, cryo ) )
      throw LArUtilException(Form("Invalid plane,tpc,cryo ID :%d, %d, %d", plane, tpc, cryo));

    auto const& cryogeo  = fCryo_v[cryo];
    auto const& tpcgeo   = cryogeo.tpc_v[tpc];
    auto const& planegeo = tpcgeo.planes_v[plane];
    
    for (int i=0; i<3; i++)
      vtx[0] = planegeo.fCenter[i];
  }

}
}

#endif
