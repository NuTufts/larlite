#ifndef SELECTIONTOOL_SPALGOPI0_CXX
#define SELECTIONTOOL_SPALGOPI0_CXX

#include "SPAlgoPi0.h"
#include "SPTBase/SPABookKeeper.h"
#include "GeoAlgo/GeoAlgo.h"
#include "EMShowerTools/EMShowerProfile.h"

namespace sptool {

  SPAlgoPi0::SPAlgoPi0() 
    : SPAlgoBase()
    , _pi0_pdf(nullptr)
    , _pi0_massVar(nullptr)
    , _pi0_massMean(nullptr)
    , _pi0_massSigma(nullptr)
    , _pi0_massData(nullptr)
    , _hMass_vs_LL(nullptr)
    , _hMass(nullptr)
  {
    _name     = "SPAlgoPi0";
    _energy_min = 40.;
    _energy_max = 1200.;
    _angle_min  = 0.;
    _angle_max  = 3.141592653589793;
    _vtx_dist_max  = 10.;
    _pi0_mean      = 130.;
    _pi0_mean_min  = 100.;
    _pi0_mean_max  = 160.;
    _pi0_sigma     = 10.;
    _pi0_sigma_min = 5.;
    _pi0_sigma_max = 70.;
  }

  void SPAlgoPi0::LoadParams(std::string fname, size_t version){

    // Load EMPart params
    _alg_emp.LoadParams(fname,version);

    //Load user_info
    SPTBase::LoadParams(fname,version);

    // Extract if parameters found

    if(_params.exist_darray("energy_range")){
      auto darray = _params.get_darray("energy_range");
      _energy_min = (*darray)[0];
      _energy_max = (*darray)[1];
    }

    if(_params.exist_darray("angle_range")){
      auto darray = _params.get_darray("angle_range");
      _angle_min = (*darray)[0];
      _angle_max = (*darray)[1];
    }

    if(_params.exist_double("vtx_dist_max"))
      _vtx_dist_max = _params.get_double("vtx_dist_max");
      
    if(_params.exist_darray("pi0_mean")){
      auto darray = _params.get_darray("pi0_mean");
      _pi0_mean     = (*darray)[0];
      _pi0_mean_min = (*darray)[1];
      _pi0_mean_max = (*darray)[2];
    }

    if(_params.exist_darray("pi0_sigma")){
      auto darray = _params.get_darray("pi0_sigma");
      _pi0_sigma     = (*darray)[0];
      _pi0_sigma_min = (*darray)[1];
      _pi0_sigma_max = (*darray)[2];
    }

    return;
  }

  void SPAlgoPi0::ProcessBegin()
  {
    _alg_emp.ProcessBegin();
    delete _pi0_pdf;
    delete _pi0_massVar;
    delete _pi0_massMean;
    delete _pi0_massSigma;
    delete _pi0_massData;

    double mass_min = _pi0_mean - _pi0_sigma_max * 5.;
    if(mass_min<50) mass_min = 50;
    double mass_max = _pi0_mean + _pi0_sigma_max * 5.;
    _pi0_massVar  = new RooRealVar("_pi0_massVar","Pi0 Mass Range [MeV]",
				   mass_min, mass_max);

    _pi0_massMean = new RooRealVar("_pi0_massMean","Pi0 Mass Mean [MeV]",
				   _pi0_mean, _pi0_mean_min, _pi0_mean_max);

    _pi0_massSigma = new RooRealVar("_pi0_massSigma","Pi0 Mass Sigma [MeV]",
				    _pi0_sigma, _pi0_sigma_min, _pi0_sigma_max);

    _pi0_massData = new RooDataSet("_pi0_massData", "Pi0 Mass Fit Data", RooArgSet(*_pi0_massVar));

    ShowerPdfFactory factory;
    _pi0_pdf = factory.Pi0Mass(*_pi0_massVar, *_pi0_massMean, *_pi0_massSigma);

    delete _hMass_vs_LL;
    delete _hMass;

    _hMass_vs_LL = new TH2D("hMass_vs_LL","Mass vs. Likelihood",100,0,200,100,-100,10);
    _hMass = new TH1D("hMass","Mass",100,0,200);
  }
  
  void SPAlgoPi0::LL(const SPAShower& shower_a,
		     const SPAShower& shower_b,
		     double& ll,
		     double& mass)
  {
    ll   = -1.e-10;
    mass = -1.;
    
    // Sanity check
    if(shower_a._energy<0 || shower_b._energy<0) return;

    // Pi0 Vtx point
    ::geoalgo::GeoAlgo geo_alg;
    ::geoalgo::Line_t   line_a( shower_a.Start(), shower_a.Start() + shower_a.Dir() );
    ::geoalgo::Line_t   line_b( shower_b.Start(), shower_b.Start() + shower_b.Dir() );
    ::geoalgo::Vector_t pt_a(3), pt_b(3);
    double vtx_dist = sqrt(geo_alg.SqDist(line_a, line_b, pt_a, pt_b));
    if(vtx_dist > _vtx_dist_max) return;

    // Opening angle
    double angle = shower_a.Dir().Angle(shower_b.Dir());
    if(angle > 3.14159265358979323846/2.) return;

    // Corrected energies
    ::EMShowerProfile shower_prof;
    ::geoalgo::AABox  volume(0,-116.5,0,256.35,116.5,1036.8);

    auto xs_a = geo_alg.Intersection(volume, ::geoalgo::HalfLine_t(shower_a.Start(),shower_a.Dir()));
    auto xs_b = geo_alg.Intersection(volume, ::geoalgo::HalfLine_t(shower_b.Start(),shower_b.Dir()));

    if(!xs_a.size() || !xs_b.size()) return;

    double energy_a = shower_a._energy / shower_prof.EnergyContainment( shower_a.Start().Dist(xs_a[0]) );
    double energy_b = shower_b._energy / shower_prof.EnergyContainment( shower_b.Start().Dist(xs_b[0]) );

    // Compute mass
    mass = sqrt(4 * energy_a * energy_b * pow(sin(angle/2.),2));

    _pi0_massVar->setVal(mass);
    auto vtx = pt_a + ((pt_b - pt_a) / 2.);
    ll  = log(_pi0_pdf->getVal(*_pi0_massVar));
    ll += _alg_emp.LL(false,shower_a._dedx,vtx.Dist(shower_a.Start()));
    ll += _alg_emp.LL(false,shower_b._dedx,vtx.Dist(shower_b.Start()));
    
    //std::cout<< log(_pi0_pdf->getVal(*_pi0_massVar))<<" : "
	     //<<_alg_emp.LL(false,shower_a._dedx,vtx.Dist(shower_a.Start()))<<" : "
	     //<<_alg_emp.LL(false,shower_b._dedx,vtx.Dist(shower_b.Start()))<<std::endl;

    return;
  }
  
  SPArticleSet SPAlgoPi0::Reconstruct(const SPAData &data)
  { 
    SPArticleSet res;
    if(!data._vtxs.size() || data._showers.size()<2) return res;

    auto shower_combinations = data.Combination(data._showers.size(),2);

    SPABookKeeper bk;
    for(auto const& comb : shower_combinations) {

      double likelihood = 0.;
      double mass       = -1.;
      LL(data._showers[comb[0]],
	 data._showers[comb[1]],
	 likelihood,
	 mass);
      
      bk.book(likelihood,comb);

      _hMass_vs_LL->Fill(mass,likelihood);
      _hMass->Fill(mass);

      if( 0 < mass &&
	  50 < mass &&
	  (_pi0_mean - _pi0_sigma_max * 5.) < mass &&
	  mass < (_pi0_mean + _pi0_sigma_max * 5.) ) {
	_pi0_massVar->setVal(mass);
	_pi0_massData->add(RooArgSet(*_pi0_massVar));
	//std::cout<<_pi0_massData->sumEntries()<<" entries... last: "<<_pi0_massData->get(_pi0_massData->sumEntries()-1)->getRealValue("_pi0_massVar")<<std::endl;
      }
    }

    return res;
  }

  void SPAlgoPi0::ProcessEnd(TFile* fout)
  {

    if(_training_mode) {
      RooFitResult* fit_res = _pi0_pdf->fitTo(*_pi0_massData,RooFit::Save());
      fit_res->Print();
      
      RooPlot* frame_mass = _pi0_massVar->frame();
      _pi0_massData->plotOn(frame_mass);
      _pi0_pdf->plotOn(frame_mass);
      
      TCanvas* c = new TCanvas("c","",600,500);
      frame_mass->Draw();
      c->SaveAs("pi0_mass.png");
      delete c;
    }

    if(fout) {
      fout->cd();
      _hMass_vs_LL->Write();
      _hMass->Write();
    }else{
      delete _hMass_vs_LL;
      delete _hMass;
    }

    return;
  }

}

#endif