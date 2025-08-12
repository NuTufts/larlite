#ifndef __SPACECHARGEMICROBOONE_CXX__
#define __SPACECHARGEMICROBOONE_CXX__
////////////////////////////////////////////////////////////////////////
// \file SpaceChargeMicroBooNE.C
//
// \brief implementation of class for storing/accessing space charge distortions for MicroBooNE
//
// \author mrmooney@bnl.gov
// 
////////////////////////////////////////////////////////////////////////

// C++ language includes
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "math.h"
#include "stdio.h"

#include "TFile.h"

// LArSoft includes
#include "SpaceChargeMicroBooNE.h"

#include "SpaceChargeMicroBooNEMCC8.h"
#include "SpaceChargeMicroBooNEMCC9.h"

namespace larutil{
  

  //-----------------------------------------------
  SpaceChargeMicroBooNE::SpaceChargeMicroBooNE( Version_t version, std::string filename)
    : _version(version)
  {
    Configure(version,filename);    
  }

  //-----------------------------------------------
  SpaceChargeMicroBooNE::~SpaceChargeMicroBooNE() 
  {
    delete _sce;
    _sce = nullptr;
  }  
    
  //-----------------------------------------------
  bool SpaceChargeMicroBooNE::Configure( Version_t version, std::string filename)
  {
    _version = version;
    
    std::string sce_file_path;

    if ( filename!="" ) {
      // override default
      sce_file_path = filename;
    }
    else {
      switch( _version ) {
      case kMCC8_E273:
        sce_file_path = Form("%s/larlite/LArUtil/dat/SCEoffsets_MicroBooNE_E273.root",getenv("LARLITE_BASEDIR"));
        break;
      case kMCC8_E227:
        sce_file_path = Form("%s/larlite/LArUtil/dat/SCEoffsets_MicroBooNE_E227.root",getenv("LARLITE_BASEDIR"));
        break;
      case kMCC8_E500:
        sce_file_path = Form("%s/larlite/LArUtil/dat/SCEoffsets_MicroBooNE_E500.root",getenv("LARLITE_BASEDIR"));
        break;
      case kMCC9_Backward:
        sce_file_path = Form("%s/larlite/LArUtil/dat/SCEoffsets_dataDriven_combined_bkwd_Jan18.root",getenv("LARLITE_BASEDIR"));
        break;
      case kMCC9_Forward:
      default:
        sce_file_path = Form("%s/larlite/LArUtil/dat/SCEoffsets_dataDriven_combined_fwd_Jan18.root",getenv("LARLITE_BASEDIR"));
        break;
      }
    }

    switch (_version) {
    case kMCC8_E273:
    case kMCC8_E227:
    case kMCC8_E500:
      _sce = new SpaceChargeMicroBooNEMCC8(sce_file_path);
      break;
    case kMCC9_Forward:
    case kMCC9_Backward:
    default:
      _sce = new SpaceChargeMicroBooNEMCC9(sce_file_path);
      break;
    }
    
    return true;
  }

  /**
   * @brief Apply space charge effect
   * 
   */
  std::vector<double> SpaceChargeMicroBooNE::ApplySpaceChargeEffect( double x, double y, double z, bool& applied )
  {

    std::vector<double> out = { x, y, z };

    if ( _version==kMCC9_Forward ) {
      // we assume we have the true energy deposit location and
      // APPLY the space charge effect to get the
      // "observable" position

      if ( !static_cast<SpaceChargeMicroBooNEMCC9*>(_sce)->IsInsideBoundaries(x,y,z) ) {
        // do not modify position
        applied = false;
        return out; 
      }

      std::vector<double> s_offset = _sce->GetPosOffsets(x,y,z);
      out[0] = out[0] - s_offset[0] + 0.7;
      out[1] = out[1] + s_offset[1];
      out[2] = out[2] + s_offset[2];
      applied = true;
    }
    else if ( _version==kMCC9_Backward ) {
      // we now assume a reconstruced position inside the detector
      // and now calculate the shift back to the true position

       if ( !static_cast<SpaceChargeMicroBooNEMCC9*>(_sce)->IsInsideBoundaries(x,y,z) ) {
        // do not modify position
        applied = false;
        return out; 
      }

      std::vector<double> s_offset = _sce->GetPosOffsets(x,y,z);
      out[0] = out[0] + s_offset[0];
      out[1] = out[1] + s_offset[1];
      out[2] = out[2] + s_offset[2];
      applied = true;
    }
    else {
      throw std::runtime_error("Version not yet implemented for SpaceChargeMicroBooNE::ApplySpaceChargeEffect");
    }

    return out;
  }

  //----------------------------------------------------------------------------
  /// Primary working method of service that provides position offsets to be
  /// used in ionization electron drift
  std::vector<double> SpaceChargeMicroBooNE::GetPosOffsets(double xVal, double yVal, double zVal) const
  {
    return _sce->GetPosOffsets(xVal,yVal,zVal);
  }

  //----------------------------------------------------------------------------
  /// Primary working method of service that provides position offsets to be
  /// used in ionization electron drift
  std::vector<double> SpaceChargeMicroBooNE::GetEfieldOffsets(double xVal, double yVal, double zVal) const
  {
    return _sce->GetEfieldOffsets(xVal,yVal,zVal);
  }
  
  float SpaceChargeMicroBooNE::tickoffset_forward_hack( float tick ) const {
    return 6 + (tick-3200)*0.014;
  }

  float SpaceChargeMicroBooNE::tickoffset_backward_hack( float tick ) const {
    return (tick-6+3200*0.014)/1.014 - tick;
  }
  
}

#endif
