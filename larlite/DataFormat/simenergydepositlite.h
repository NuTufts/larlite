//////////////////////////////////////////////////////////////////////
/// \file  lardataobj/Simulation/SimEnergyDepositLite.h
/// \brief contains information for a single step in the detector simulation
///         (pared down in size to the essential information)
/// \authors Laura Domine
////////////////////////////////////////////////////////////////////////
#ifndef LARLITE_LARDATAOBJ_SIMULATION_SIMENERGYDEPOSITLITE_H
#define LARLITE_LARDATAOBJ_SIMULATION_SIMENERGYDEPOSITLITE_H

// LArSoft includes
// Define the LArSoft standard geometry types and methods.
//#include "larcoreobj/SimpleTypesAndConstants/geo_vectors.h"
//#include "lardataobj/Simulation/SimEnergyDeposit.h"
#include "larlite/Base/geo_vectors.h"
#include "larlite/DataFormat/simenergydeposit.h"

// C++ includes
#include <vector>

namespace larlite {
  /**
   * @brief Energy deposition in the active material (lite version).
   *
   * The detector simulation (presently LArG4, which invokes Geant4)
   * propagates particles through the detector in intervals of "steps".
   * These are usually stored in sim::SimEnergyDeposit objects. For
   * filesize concerns we pare it down to the essential information.
   */
  class SimEnergyDepositLite {
  public:
    using Length_t = float;
    using Point_t = geo::Point_t;

    SimEnergyDepositLite(double e = 0.,
                         geo::Point_t middle = {0., 0., 0.},
                         double t = 0.,
                         int id = 0)
      : edep(e), middlePos(middle), middleTime(t), trackID(id)
    {}

    double Energy() const { return edep; }
    geo::Point_t const& Position() const noexcept { return middlePos; }
    geo::Point_t const& MidPoint() const noexcept
    {
      return Position();
    } ///< Just an alias for compatibility with SED
    double Time() const { return middleTime; }
    int TrackID() const { return trackID; }

    geo::Length_t X() const { return middlePos.X(); }
    geo::Length_t Y() const { return middlePos.Y(); }
    geo::Length_t Z() const { return middlePos.Z(); }
    double T() const { return middleTime; }
    double E() const { return edep; }

    bool operator<(const SimEnergyDepositLite& rhs) const
    {
      if (trackID < rhs.trackID) return true;
      if (trackID > rhs.trackID) return false;
      if (middleTime < rhs.middleTime) return true;
      if (middleTime > rhs.middleTime) return false;
      if (middlePos.Z() < rhs.Z()) return true;
      if (middlePos.Z() > rhs.Z()) return false;
      if (middlePos.Y() < rhs.Y()) return true;
      if (middlePos.Y() > rhs.Y()) return false;
      if (middlePos.X() < rhs.X()) return true;
      if (middlePos.X() > rhs.X()) return false;
      return (edep > rhs.edep); // sort by _decreasing_ energy
    }

    // Need to be aware that information is obviously not complete
    // (e.g. no exact start/end point or time, only middle point or time)
    operator larlite::SimEnergyDeposit() const
    {
      return larlite::SimEnergyDeposit(
        0, 0, 0, edep, middlePos, middlePos, middleTime, middleTime, trackID, 0);
    }

  private:
    float edep;             ///< energy deposition (MeV)
    geo::Point_t middlePos; ///< position in (cm)
    double middleTime;      ///< (ns)
    int trackID;            ///< simulation track id
  };

  typedef std::vector<SimEnergyDepositLite> SimEnergyDepositLiteCollection;
} // namespace sim
#endif // LARDATAOBJ_SIMULATION_SIMENERGYDEPOSITLITE_H
