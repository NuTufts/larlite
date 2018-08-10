/**
 * \file ERToolHelper.h
 *
 * \ingroup LArLite_ERTool
 *
 * \brief Class def header for a class ERToolHelper
 *
 * @author kazuhiro
 */

/** \addtogroup LArLite_ERTool

    @{*/
#ifndef LARLITE_ERTOOLHELPER_H
#define LARLITE_ERTOOLHELPER_H

#include <iostream>
#include <map>
#include <TRandom.h>
#include "DataFormat/DataFormat-TypeDef.h"
#include "ERTool/Base/EmptyInput.h"
#include "ERToolHelperUtil.h"
#include "LArUtil/ElecClock.h"
#include "LArUtil/Geometry.h"

//#include "ERTool/Base/EventData.h"
//#include "ERTool/Base/ParticleGraph.h"

//Use ShowerProfile to calculate Height & Radius from Energy
#include "EMShowerTools/EMShowerProfile.h"

namespace larlite {

/**
   \class ERToolHelper
   @brief A collection of helper functions to generate SPAData to be used in SPT fmwk.
*/
class ERToolHelper {

public:

    /// Default constructor
    ERToolHelper() {
        // _minEDep = 1.e-10;
        _hutil = new ERToolHelperUtil();
    };

    /// Default destructor
    virtual ~ERToolHelper() {};

    /// Create MC EventData and ParticleSet
    void FillMCInfo(const event_mcflux&    mcf_v,
                    const event_mctruth&   mci_v,
                    const event_mcshower&  mcs_v,
                    const event_mctrack&   mct_v,
                    ::ertool::io::EmptyInput& strm) const;

    /// Create MC EventData and ParticleSet, if no mcflux is present (wrapper)
    void FillMCInfo(const event_mctruth&   mci_v,
                    const event_mcshower&  mcs_v,
                    const event_mctrack&   mct_v,
                    ::ertool::io::EmptyInput& strm) const;

    /// Fill Track Info from MC
    void FillTracks( const event_mctrack& mct_v,
                     ::ertool::io::EmptyInput& strm) const;

    /// Fill Track Info from RECO
    void FillTracks ( const event_track&        trk_v,
                      storage_manager&          storage,
                      ::ertool::io::EmptyInput& strm) const;

    /// Fill Shower Info from MC
    void FillShowers ( const event_mcshower&  mcs_v,
                       ::ertool::io::EmptyInput& mgr) const;

    /// Fill Shower Info from RECO
    void FillShowers ( const event_shower&    shw_v,
                       storage_manager&       storage,
                       ::ertool::io::EmptyInput& mgr) const;

    /// Fill Flash info from RECO
    void FillFlashes ( const event_opflash& flash_v,
                       ::ertool::io::EmptyInput& strm) const;

    /// MC Cheater for single showers
    void SingleShowerCheater(const event_mcshower& mcs_v,
                             ::ertool::io::EmptyInput& strm ) const;

    /// Set minimum EDep amount for shower to be added to EventData
    void SetMinEDep(double E) { if (E < 1.e-10) E = 1.e-10; _hutil->SetMinEDep(E);}// _minEDep = E; }

    /// Setter to disable xshift
    void setDisableXShift(bool flag) { _hutil->setDisableXShift(flag); }

private:

    // Minimum deposited energy for showers in order to
    // be added to EventData
    // THIS IS NOW STORED IN ERTOOLHELPERUTIL
    // double _minEDep;

    EMShowerProfile _shrProfiler;

    const ::larutil::Geometry *geom = ::larutil::Geometry::GetME();

    ERToolHelperUtil *_hutil;
};
}

#endif
/** @} */ // end of doxygen group

