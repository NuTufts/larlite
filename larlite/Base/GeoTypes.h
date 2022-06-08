/**
 * \file GeoTypes.h
 *
 * \ingroup Base
 * 
 * \brief defines geometry types
 *
 * @author Kazu - Nevis 2014
 */

/** \addtogroup Base

    @{*/

#ifndef LARLITE_GEOTYPES_H
#define LARLITE_GEOTYPES_H
#include <string>
#include <climits>

/// Namespace of everything in this framework
namespace larlite{
  
  namespace geo {

    // --- BEGIN -- Geometry enumerators -----------------------------------------
    // enums from larsoft version of GeoTypes.h
    typedef enum coordinates {
      kXCoord, ///< X coordinate.
      kYCoord, ///< Y coordinate.
      kZCoord  ///< Z coordinate.
    } Coord_t;
    
    /// Enumerate the possible plane projections
    typedef enum _plane_proj {
      kU,       ///< Planes which measure U.
      kV,       ///< Planes which measure V.
      kW,       ///< Planes which measure W (third view for Bo, MicroBooNE, etc).
      kZ=kW,    ///< Planes which measure Z direction.
      kY,       ///< Planes which measure Y direction.
      kX,       ///< Planes which measure X direction.
      k3D,      ///< 3-dimensional objects, potentially hits, clusters, prongs, etc.
      kUnknown  ///< Unknown view.
    } View_t;
    
    typedef enum _plane_orient {
      kHorizontal, ///< Planes that are in the horizontal plane.
      kVertical    ///< Planes that are in the vertical plane (e.g. ArgoNeuT).
    } Orient_t;
    
    typedef enum _plane_sigtype {
      kInduction,   ///< Signal from induction planes.
      kCollection,  ///< Signal from collection planes.
      kMysteryType  ///< Who knows?
    } SigType_t;

   /**
   * @brief Drift direction: positive or negative
   *
   * Do not use this type to distinguish different drift axes: e.g., negative
   * x drift and negative z drift are both by `kNeg`.
   */
  typedef enum driftdir {
    kUnknownDrift, ///< Drift direction is unknown.
    kPos,          ///< Drift towards positive values.
    kNeg,          ///< Drift towards negative values.
    kPosX = kPos,  ///< Drift towards positive X values.
    kNegX = kNeg   ///< Drift towards negative X values.
  } DriftDirection_t;
  
  
  /// Numerical description of geometry element "level".
  /// The "detector" level is noticeably missing.
  struct ElementLevel {
    
    using Level_t = std::size_t;
    
    static constexpr Level_t Cryostat   = 0U;
    static constexpr Level_t OpDet      = 1U;
    static constexpr Level_t TPC        = 1U;
    static constexpr Level_t Plane      = 2U;
    static constexpr Level_t Wire       = 3U;
    static constexpr Level_t NLevels = 4U;
    
  }; // struct ElementLevel
  
  
  /// @}
  // --- END -- Geometry enumerators -------------------------------------------
    
    // The data type to uniquely identify a TPC
    struct TPCID {
      TPCID(): isValid(false), Cryostat(UINT_MAX), TPC(UINT_MAX) {}

      TPCID(unsigned int c, unsigned int t): isValid(true), Cryostat(c), TPC(t) {}

      bool         isValid;  ///< whether this ID points to a valid TPC
      unsigned int Cryostat; ///< index of cryostat the TPC belongs
      unsigned int TPC;      ///< index of the TPC within its cryostat

      /// Returns true if the ID is valid
      operator bool() const { return isValid; }

      /// Returns true if the ID is not valid
      bool operator! () const { return !isValid; }

      bool operator== (const TPCID& pid) const
      { return ((Cryostat == pid.Cryostat) && (TPC == pid.TPC)); }

      bool operator!= (const TPCID& pid) const
      { return ((Cryostat != pid.Cryostat) || (TPC != pid.TPC)); }

      // Order TPCID in increasing Cryo, then TPC
      bool operator<( const TPCID& tpcid ) const
      {
        if(      Cryostat != tpcid.Cryostat ) return Cryostat < tpcid.Cryostat;
        else if(      TPC != tpcid.TPC      ) return TPC      < tpcid.TPC;
        else return false;
      }
    };

    /**
       \class PlaneID
       The data type to uniquely identify a Plane
    */
    struct PlaneID {
      PlaneID()
        : Cryostat(UINT_MAX)
        , TPC(UINT_MAX)
        , Plane(UINT_MAX)
        , isValid(false)
      {}

      PlaneID(unsigned int c,
              unsigned int t,
              unsigned int p)
        : Cryostat(c)
        , TPC(t)
        , Plane(p)
        , isValid(true)
      {}

      PlaneID(const TPCID& tpcid, unsigned int p)
        : Cryostat(tpcid.Cryostat), TPC(tpcid.TPC), Plane(p)
        , isValid(tpcid.isValid)
      {}

      unsigned int Cryostat;
      unsigned int TPC;
      unsigned int Plane;
      bool         isValid;

      bool operator==( const PlaneID& pid ) const {
        return ( Cryostat == pid.Cryostat &&
                 TPC      == pid.TPC      &&
                 Plane    == pid.Plane      );
      }

      bool operator!=( const PlaneID& pid ) const { return !((*this)==pid); }

      bool operator<( const PlaneID& pid ) const {
        if(      Cryostat != pid.Cryostat ) return Cryostat < pid.Cryostat;
        else if(      TPC != pid.TPC      ) return TPC      < pid.TPC;
        else if(    Plane != pid.Plane    ) return Plane    < pid.Plane;
        else return false;
      }
    };

    /**
       \class WireID
       The data type to uniquely identify a code wire segment.
    */
    struct WireID {
      WireID()
        : Cryostat(UINT_MAX)
        , TPC(UINT_MAX)
        , Plane(UINT_MAX)
        , Wire(UINT_MAX)
        , isValid(false)
      {}

      WireID(unsigned int c,
             unsigned int t,
             unsigned int p,
             unsigned int w)
        : Cryostat(c)
        , TPC(t)
        , Plane(p)
        , Wire(w)
        , isValid(true)
      {}

      WireID(const PlaneID& planeid, unsigned int w)
        : Cryostat(planeid.Cryostat), TPC(planeid.TPC), Plane(planeid.Plane)
        , Wire(w)
        , isValid(planeid.isValid)
      {}

      unsigned int Cryostat;
      unsigned int TPC;
      unsigned int Plane;
      unsigned int Wire;
      bool         isValid;

      PlaneID planeID() const { return PlaneID(Cryostat, TPC, Plane); }
      
      bool operator==( const WireID& wid ) const {
        return ( Cryostat == wid.Cryostat &&
                 TPC      == wid.TPC      &&
                 Plane    == wid.Plane    &&
                 Wire     == wid.Wire        );
      }

      bool operator!=( const WireID& wid ) const { return !((*this)==wid);}

      bool operator<( const WireID& wid ) const {
        if(      Cryostat != wid.Cryostat ) return Cryostat < wid.Cryostat;
        else if(      TPC != wid.TPC      ) return TPC      < wid.TPC;
        else if(    Plane != wid.Plane    ) return Plane    < wid.Plane;
        else if(     Wire != wid.Wire     ) return Wire     < wid.Wire;
        else return false;
      }
    };

  std::string SignalTypeName(SigType_t sigType);
    
  }//namespace geo


}
#endif
/** @} */ // end of doxygen group
