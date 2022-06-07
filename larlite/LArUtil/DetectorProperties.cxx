#ifndef LARLITE_DETECTORPROPERTIES_CXX
#define LARLITE_DETECTORPROPERTIES_CXX

#include "DetectorProperties.h"

namespace larutil {

  DetectorProperties* DetectorProperties::_me = 0;

  DetectorProperties::DetectorProperties(bool default_load) : LArUtilBase()
  {
    _name = "DetectorProperties";
    if(default_load) {
      _file_name = Form("%s/LArUtil/dat/%s",
			getenv("LARLITE_COREDIR"),
			kUTIL_DATA_FILENAME[LArUtilConfig::Detector()].c_str());
      _tree_name = kTREENAME_DETECTORPROPERTIES;
      if ( LArUtilConfig::Detector()==larlite::geo::kMicroBooNE ) {
	_tree_name = "scanner/"+kTREENAME_DETECTORPROPERTIES;
      }
      LoadData();
    }
  }

  void DetectorProperties::ClearData()
  {
    fSamplingRate = larlite::data::kINVALID_DOUBLE;
    fTriggerOffset = larlite::data::kINVALID_INT;
    fElectronsToADC = larlite::data::kINVALID_DOUBLE;
    fNumberTimeSamples = larlite::data::kINVALID_UINT;
    fReadOutWindowSize = larlite::data::kINVALID_UINT;

    fTimeOffsetU = larlite::data::kINVALID_DOUBLE;
    fTimeOffsetV = larlite::data::kINVALID_DOUBLE;
    fTimeOffsetZ = larlite::data::kINVALID_DOUBLE;

    fXTicksCoefficient = larlite::data::kINVALID_DOUBLE;


    fXTicksOffsets_cryoid.clear();
    fXTicksOffsets_tpcid.clear();
    fXTicksOffsets_planeid.clear();            
    fXTicksOffsets.clear();    

  }

  bool DetectorProperties::ReadTree()
  {

    ClearData();
    TChain *ch = new TChain(_tree_name.c_str());
    ch->AddFile(_file_name.c_str());

    std::string error_msg("");
    if(!(ch->GetBranch("fSamplingRate")))      error_msg += "      fSamplingRate\n";
    if(!(ch->GetBranch("fTriggerOffset")))     error_msg += "      fTriggerOffset\n";
    if(!(ch->GetBranch("fElectronsToADC")))    error_msg += "      fElectronsToADC\n";
    if(!(ch->GetBranch("fNumberTimeSamples"))) error_msg += "      fNumberTimeSamples\n";
    if(!(ch->GetBranch("fReadOutWindowSize"))) error_msg += "      fReadOutWindowSize\n";
    if(!(ch->GetBranch("fTimeOffsetU")))       error_msg += "      fTimeOffsetU\n";
    if(!(ch->GetBranch("fTimeOffsetV")))       error_msg += "      fTimeOffsetV\n";
    if(!(ch->GetBranch("fTimeOffsetZ")))       error_msg += "      fTimeOffsetZ\n";
    if(!(ch->GetBranch("fXTicksCoefficient"))) error_msg += "      fXTicksCoefficient\n";
    if ( LArUtilConfig::Detector()==larlite::geo::kMicroBooNE ) {
      std::cout << "old fXTicksOffsets branch used for MicroBooNE" << std::endl;
      if(!(ch->GetBranch("fXTicksOffsets")))     error_msg += "      fXTicksOffsets\n";
    }
    else {
      if(!(ch->GetBranch("fXTicksOffsets_cryoid")))   error_msg += "      fXTicksOffsets_cryoid\n";
      if(!(ch->GetBranch("fXTicksOffsets_tpcid")))    error_msg += "      fXTicksOffsets_tpcid\n";
      if(!(ch->GetBranch("fXTicksOffsets_planeid")))  error_msg += "      fXTicksOffsets_planeid\n";
      if(!(ch->GetBranch("fXTicksOffsets_offset")))   error_msg += "      fXTicksOffsets_offset\n";      
    }
    if(!error_msg.empty()) {

      throw larlite::larutil::LArUtilException(Form("Missing following TBranches...\n%s",error_msg.c_str()));

      return false;
    }

    ch->SetBranchAddress("fSamplingRate",&fSamplingRate);
    ch->SetBranchAddress("fTriggerOffset",&fTriggerOffset);
    ch->SetBranchAddress("fElectronsToADC",&fElectronsToADC);
    ch->SetBranchAddress("fNumberTimeSamples",&fNumberTimeSamples);
    ch->SetBranchAddress("fReadOutWindowSize",&fReadOutWindowSize);
    ch->SetBranchAddress("fTimeOffsetU",&fTimeOffsetU);
    ch->SetBranchAddress("fTimeOffsetV",&fTimeOffsetV);
    ch->SetBranchAddress("fTimeOffsetZ",&fTimeOffsetZ);
    ch->SetBranchAddress("fXTicksCoefficient",&fXTicksCoefficient);

    std::vector<Int_t>    *pXTicksOffsets_cryoid=nullptr;
    std::vector<Int_t>    *pXTicksOffsets_tpcid=nullptr;
    std::vector<Int_t>    *pXTicksOffsets_planeid=nullptr;
    std::vector<Double_t> *pXTicksOffsets_offset=nullptr;    
    if ( LArUtilConfig::Detector()==larlite::geo::kMicroBooNE ) {
      ch->SetBranchAddress("fXTicksOffsets",&pXTicksOffsets_offset);
    }
    else {
      ch->SetBranchAddress("fXTicksOffsets_cryoid", &pXTicksOffsets_cryoid);      
      ch->SetBranchAddress("fXTicksOffsets_tpcid",  &pXTicksOffsets_tpcid);      
      ch->SetBranchAddress("fXTicksOffsets_planeid",&pXTicksOffsets_planeid);      
      ch->SetBranchAddress("fXTicksOffsets_offset", &pXTicksOffsets_offset);
    }

    ch->GetEntry(0);

    // HACK: update values for MCC9
    if ( LArUtilConfig::Detector()==larlite::geo::kMicroBooNE ) {
      fXTicksCoefficient = 0.5*larutil::kDriftVelMCC9;
      pXTicksOffsets_offset->at(0) = 3200; // location of trigger
    }

    for(size_t i=0; i<pXTicksOffsets_offset->size(); ++i) {
      if ( LArUtilConfig::Detector()==larlite::geo::kMicroBooNE) {
	// to do: homogenize microboone with other detectors
	std::vector<int> ctp = { 0, 0, (int)i };
	fCTP_to_offsetindex[ ctp ] = i;
	fXTicksOffsets_cryoid.push_back(  0 );
	fXTicksOffsets_tpcid.push_back(   0 );
	fXTicksOffsets_planeid.push_back( (int)i );
      }
      else {
	std::vector<int> ctp = { pXTicksOffsets_cryoid->at(i),
	  pXTicksOffsets_tpcid->at(i),
	  pXTicksOffsets_planeid->at(i) };
	fCTP_to_offsetindex[ ctp ] = i;
	fXTicksOffsets_cryoid.push_back(  pXTicksOffsets_cryoid->at(i) );
	fXTicksOffsets_tpcid.push_back(   pXTicksOffsets_tpcid->at(i) );
	fXTicksOffsets_planeid.push_back( pXTicksOffsets_planeid->at(i) );	
      }
      fXTicksOffsets.push_back(pXTicksOffsets_offset->at(i));
    }
    
    delete ch;
    return true;
  }

  /**
   * @brief Get Offset between start of image and trigger tick
   *
   */
  Double_t DetectorProperties::GetXTicksOffset(Int_t p, Int_t tpc, Int_t cryo) const {
    std::vector< int > ctp = { cryo, tpc, p };
    auto it=fCTP_to_offsetindex.find(ctp);
    if ( it==fCTP_to_offsetindex.end() ) {
      throw larlite::larutil::LArUtilException(Form("Bad (cryo,tpc,plane) number"));
    }

    return fXTicksOffsets.at( it->second );
  }

}

#endif
