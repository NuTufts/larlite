#ifndef ERTOOL_ERALGOPI0_CXX
#define ERTOOL_ERALGOPI0_CXX

#include "ERAlgopi0.h"

namespace ertool {

	ERAlgopi0::ERAlgopi0(const std::string& name)
	: AlgoBase(name)
	, _ll_tree(nullptr)
	, _pi0_tree(nullptr)
	{
	}
	void ERAlgopi0::Reset()
	{}

	void ERAlgopi0::AcceptPSet(const ::fcllite::PSet& cfg)
	{
    // Load EMPart params
		_alg_emp.SetMode(true);
		_alg_emp.AcceptPSet(cfg);
	}

	void ERAlgopi0::ProcessBegin()
	{
		_alg_emp.ProcessBegin();
		_alg_emp.setPlot(false);

    // Initialize LL Tree
		delete _ll_tree;
		_ll_tree = new TTree("_ll_tree","LL Tree");
		_ll_tree->Branch("_dedx_A",&_dedx_A,"dedx_A/D");
		_ll_tree->Branch("_dedx_B",&_dedx_B,"dedx_B/D");
		_ll_tree->Branch("_vtxDist_A",&_vtxDist_A,"vtxDist_A/D");
		_ll_tree->Branch("_vtxDist_B",&_vtxDist_B,"vtxDist_B/D");
		_ll_tree->Branch("_vtx_IP",&_vtx_IP,"vtx_IP/D");
		_ll_tree->Branch("_mass",&_mass,"mass/D");
		_ll_tree->Branch("_angle",&_angle,"angle/D");
		_ll_tree->Branch("_E_A",&_E_A,"E_A/D");
		_ll_tree->Branch("_E_B",&_E_B,"E_B/D");
		_ll_tree->Branch("_ll_photon_A",&_ll_photon_A,"ll_photon_A/D");
		_ll_tree->Branch("_ll_photon_B",&_ll_photon_B,"ll_photon_B/D");
    // Selected Tree Tree
		delete _pi0_tree;
		_pi0_tree = new TTree("_pi0_tree","Pi0 Tree");
		_pi0_tree->Branch("_Rmass",&_Rmass,"mass/D");
		_pi0_tree->Branch("_Rmass",&_Rmass,"mass/D");
		_pi0_tree->Branch("_RvertexX",&_RvertexX,"vertexX/D");
		_pi0_tree->Branch("_RvertexY",&_RvertexY,"vertexY/D");
		_pi0_tree->Branch("_RvertexZ",&_RvertexZ,"vertexZ/D");
		_pi0_tree->Branch("_RvtxIP",&_RvtxIP,"vtxIP/D");
		_pi0_tree->Branch("_RphotonE1",&_RphotonE1,"photonE1/D");
		_pi0_tree->Branch("_RphotonE2",&_RphotonE2,"photonE2/D");
		_pi0_tree->Branch("_RphotonDist1",&_RphotonDist1,"photonDist1/D");
		_pi0_tree->Branch("_RphotonDist2",&_RphotonDist2,"photonDist2/D");
		_pi0_tree->Branch("_RvertexXshr1",&_RvertexXbshr1,"vertexXbshr1/D");
		_pi0_tree->Branch("_RvertexYbshr1",&_RvertexYbshr1,"vertexYbshr1/D");
		_pi0_tree->Branch("_RvertexZbshr1",&_RvertexZbshr1,"vertexZbshr1/D");
		_pi0_tree->Branch("_RvertexXshr2",&_RvertexXbshr2,"vertexXbshr2/D");
		_pi0_tree->Branch("_RvertexYbshr2",&_RvertexYbshr2,"vertexYbshr2/D");
		_pi0_tree->Branch("_RvertexZbshr2",&_RvertexZbshr2,"vertexZbshr2/D");
	}

	bool ERAlgopi0::Reconstruct(const EventData &data, ParticleGraph& graph)
	{
	// First check to see if there are atleast two showers in the event 
		if(graph.GetParticleNodes(RecoType_t::kShower).size() < 2) return true;

		auto datacpy = data;
		auto const& shrIDs = graph.GetParticleNodes(RecoType_t::kShower);
	// This is a real shitty hack even for a bad programmer 
		morepi0s:
	// First look for the best possible pi0 from all combinations of showers 
	// Then create the particle and child relationships
	// next go back and try to make another pi0 with what ever showers are left
	//best pi0 
		double bestpi0ll = -1e10;
		double Bestmass = -999;
		NodeID_t BestShr1; // Best selected showerID
		std::vector<NodeID_t> shr1Tested;// Holder for already used showerID
		NodeID_t BestShr2;// Best selected showerID
		std::vector<NodeID_t> shr2Tested;// Holder for already used showerID
		geoalgo::Point_t Bestvertex(3);
		geoalgo::Vector_t Bestmomentum(3);
		double Bestenergy_1;
		double Bestenergy_2;

		for (auto const& shrID1 : shrIDs){
			bool st1 = false;
			for(unsigned int a=0; a<shr1Tested.size(); a++){if(shr1Tested[a]==shrID1) st1=true;}// check to see if this was used 
				for (auto const& shrID2 : shrIDs){
					if (shrID2 <= shrID1) continue;
		// checks to see if this combo has already been tested
					if(st1) for(unsigned int b=0; b<shr2Tested.size(); b++){if(shr2Tested[b]==shrID2) continue;}
		// make sure that the photons are not already used 
					if ( graph.GetParticle(shrID1).RelationAssessed()|| graph.GetParticle(shrID2).RelationAssessed() ) continue;
		// make a const copy for this test
					auto const& shr1 = datacpy.Shower(graph.GetParticle(shrID1).RecoID());
					auto const& shr2 = datacpy.Shower(graph.GetParticle(shrID2).RecoID());
					double likelihood = 0.;
					double mass       = -1.;
					_angle = 0;
	// Corrected for the photon energy=========
					::EMShowerProfile shower_prof;
					::geoalgo::AABox  volume(0,-116.5,0,256.35,116.5,1036.8);
					auto xs_a = _geoAlgo.Intersection(volume, ::geoalgo::HalfLine_t(shr1.Start(),shr1.Dir()));
					auto xs_b = _geoAlgo.Intersection(volume, ::geoalgo::HalfLine_t(shr2.Start(),shr2.Dir()));
					if(!xs_a.size() || !xs_b.size()) continue;
					double menergy_1 = shr1._energy / shower_prof.EnergyContainment( shr1.Start().Dist(xs_a[0]) );
					double menergy_2 = shr2._energy / shower_prof.EnergyContainment( shr2.Start().Dist(xs_b[0]) );
					geoalgo::Point_t vert(3);
					double rdot = _geoAlgo.commonOrigin(shr1, shr2, vert, true);
					double vDist_A = vert.Dist(shr1.Start());
					double vDist_B = vert.Dist(shr2.Start());
					geoalgo::Vector_t dflipA(-1.*shr1.Dir()[0],-1.*shr1.Dir()[1],-1.*shr1.Dir()[2]);
					geoalgo::Vector_t dflipB(-1.*shr2.Dir()[0],-1.*shr2.Dir()[1],-1.*shr2.Dir()[2]);
					auto bksa = ::geoalgo::HalfLine_t(shr1.Start(),dflipA);
					auto bksb =::geoalgo::HalfLine_t(shr2.Start(),dflipB);
	//==========
		// CUT ON Any Other GOES HERE 
					geoalgo::Point_t vertex(3);
					geoalgo::Vector_t momentum(3);
					LL(shr1, shr2, likelihood, mass, vertex, momentum);

		//=======
		// Some possible cuts before doing the LL 
		//=======
					if(shr1._energy< _energy_min  || shr2._energy<_energy_min) continue;
		// CUT ON IP GOES HERE 
					if(pow(_geoAlgo.SqDist(bksa,bksb),0.5)>_IPmax) continue;
		// CUT ON LL GOES HERE 
					if(likelihood<_LLmin) continue;
		// Cut on mass if desired 
					if(mass<_minMass || mass>_maxMass) continue;
		// Cut on min total photon energy
					if(menergy_1+menergy_2<_minSumE) continue;

		// check if the ll is better... if it is... then save the shower info and ll
					if(bestpi0ll<likelihood){
						BestShr1 = shrID1;
						BestShr2 = shrID2;
						bestpi0ll = likelihood;
						Bestmass = mass;
						Bestvertex = vertex;
						Bestmomentum = Bestmomentum;
						Bestenergy_1 = menergy_1;
						Bestenergy_2 = menergy_2;
					}
				}// shr2id 
					}// shr1id 

		// We have a good pi0 let's make a particle 
					if(bestpi0ll!= -1e10){
			// Set the best 
						shr1Tested.push_back(BestShr1);
						shr2Tested.push_back(BestShr2);
						_angle = 0;
						auto const& bshr1 = datacpy.Shower(graph.GetParticle(BestShr1).RecoID());
						auto const& bshr2 = datacpy.Shower(graph.GetParticle(BestShr2).RecoID());
						double E_A = bshr1._energy;
						double E_B = bshr2._energy;
			// this gets angle and sets the vertex origin
			geoalgo::Point_t vert(3);// this is done twice... not needed
			double dot = _geoAlgo.commonOrigin(bshr1, bshr2, vert, true);
			// this is the distange from start of shower to vertex point
			double vtxDist_A = vert.Dist(bshr1.Start());
			double vtxDist_B = vert.Dist(bshr2.Start());
			double vtx_IP = -999;
			geoalgo::Vector_t sdflipA(-1.*bshr1.Dir()[0],-1.*bshr1.Dir()[1],-1.*bshr1.Dir()[2]);
			geoalgo::Vector_t sdflipB(-1.*bshr2.Dir()[0],-1.*bshr2.Dir()[1],-1.*bshr2.Dir()[2]);
			auto bkshrA = ::geoalgo::HalfLine_t(bshr1.Start(),sdflipA);
			auto bkshrB =::geoalgo::HalfLine_t(bshr2.Start(),sdflipB);
			vtx_IP = pow(_geoAlgo.SqDist(bkshrA, bkshrB),0.5);

          // specify info for 1st gamma
			auto const& gamma1vtx = bshr1.Start();
			auto const& gamma1mom = bshr1.Dir()*Bestenergy_1;
			//auto const& gamma1mom = bshr1.Dir()*bshr1._energy;
			graph.GetParticle(BestShr1).SetParticleInfo(22,0,gamma1vtx,gamma1mom);
          // specify info for 2nd gamma
			auto const& gamma2vtx = bshr2.Start();
			auto const& gamma2mom = bshr2.Dir()*Bestenergy_2;
			graph.GetParticle(BestShr2).SetParticleInfo(22,0,gamma2vtx,gamma2mom);
          // create pi0
			Particle& pi0 = graph.CreateParticle();
			pi0.SetParticleInfo(111,Bestmass,Bestvertex,Bestmomentum);
	  // fill out the selected pi0 into the tree... this can be more elegant later instead of just copying things 
			_Rmass = Bestmass;
			_RvertexX = Bestvertex[0];
			_RvertexY = Bestvertex[1];
			_RvertexZ = Bestvertex[2];
			_RvtxIP = vtx_IP;
			_RphotonE1 =  Bestenergy_1;
			_RphotonE2 =  Bestenergy_2;
			_RphotonDist1= vtxDist_A;
			_RphotonDist2= vtxDist_B;
			_RvertexXbshr1 = bshr1.Start()[0];
			_RvertexYbshr1 = bshr1.Start()[1];
			_RvertexZbshr1 = bshr1.Start()[2];
			_RvertexXbshr2 = bshr2.Start()[0];
			_RvertexYbshr2 = bshr2.Start()[1];
			_RvertexZbshr2 = bshr2.Start()[2];
	// Fill the pi0 tree 
			_pi0_tree->Fill();
          // Add relationships
			graph.SetParentage(pi0.ID(),BestShr1);
			graph.SetParentage(pi0.ID(),BestShr2);
			graph.SetSiblings(BestShr1,BestShr2);
	// DAM using a go to... 
			goto morepi0s;
	}// If Bestpi0ll



	return true;}

	void ERAlgopi0::ProcessEnd(TFile* fout)
	{
		if(fout) {
			fout->cd();
			//_ll_tree->Write();
			//_pi0_tree->Write();
			_alg_emp.ProcessEnd(fout);
	}//
}

//===========================
	// make a LL if shower combo is pi0 like
//===========================
void ERAlgopi0::LL(const Shower& shower_a,
	const Shower& shower_b,
	double& ll,
	double& mass,
	geoalgo::Point_t& vtx,
	geoalgo::Vector_t& mom)
{
	// First define the LL to something we know
	ll   = -1.e-15;
	_mass = -1.;
	// Define each shower
	_E_A = shower_a._energy;
	_E_B = shower_b._energy;
	_dedx_A = shower_a._dedx;
	_dedx_B = shower_b._dedx;

	// make sure each shower energy is above 0
	if( (_E_A < 0) || (_E_B < 0) ) return;		
	if( (_E_A <  _energy_min) ||(_E_B <  _energy_min)||(_E_A >  _energy_max)||(_E_B >  _energy_max)) return;		
	
	// this gets angle and sets the vertex origin
	_dot = _geoAlgo.commonOrigin(shower_a, shower_b, vtx, true);
	
	// this is the distange from start of shower to vertex point
	_vtxDist_A = vtx.Dist(shower_a.Start());
	_vtxDist_B = vtx.Dist(shower_b.Start());

	//_angle = shower_a.Dir().Angle(shower_b.Dir());
	
	// Corrected for the photon energy
	::EMShowerProfile shower_prof;
	::geoalgo::AABox  volume(0,-116.5,0,256.35,116.5,1036.8);

	
	auto xs_a = _geoAlgo.Intersection(volume, ::geoalgo::HalfLine_t(shower_a.Start(),shower_a.Dir()));
	auto xs_b = _geoAlgo.Intersection(volume, ::geoalgo::HalfLine_t(shower_b.Start(),shower_b.Dir()));

	if(!xs_a.size() || !xs_b.size()) return;

	// 
	double energy_a = shower_a._energy / shower_prof.EnergyContainment( shower_a.Start().Dist(xs_a[0]) );
	double energy_b = shower_b._energy / shower_prof.EnergyContainment( shower_b.Start().Dist(xs_b[0]) );

	_angle = shower_a.Dir().Angle(shower_b.Dir());
    // Compute mass
	mass = sqrt(2 * energy_a * energy_b *(1.-cos(_angle)));
	_mass = mass;

    // calculate momentum:
	geoalgo::Vector_t energy(shower_a.Dir() * energy_a + shower_b.Dir() * energy_b);
	energy.Normalize();
	double mom2 = sqrt(pow(energy_a,2)+pow(energy_b,2)+2*energy_a*energy_b*cos(_angle));
	mom = energy*mom2;

    // get the shower dedx value
	_dedx_A = shower_a._dedx;
	_dedx_B = shower_b._dedx;

    // calculate IP  using back going showers!
	_vtx_IP = -999;
	geoalgo::Vector_t dirflipA(-1.*shower_a.Dir()[0],-1.*shower_a.Dir()[1],-1.*shower_a.Dir()[2]);
	geoalgo::Vector_t dirflipB(-1.*shower_b.Dir()[0],-1.*shower_b.Dir()[1],-1.*shower_b.Dir()[2]);
	auto bkshra = ::geoalgo::HalfLine_t(shower_a.Start(),dirflipA);
	auto bkshrb =::geoalgo::HalfLine_t(shower_b.Start(),dirflipB);
	_vtx_IP = pow(_geoAlgo.SqDist(bkshra, bkshrb),0.5);

    // likelihood of shower to be a gamma is based on dedx and vtx distance.
    // if vtx distance is nan then use just the dedx
	if(_vtxDist_A==_vtxDist_A){_ll_photon_A = _alg_emp.LL(false,shower_a._dedx,_vtxDist_A);}
	else{ _ll_photon_A = _alg_emp.LL(false,shower_a._dedx,-1);}
	if(_vtxDist_B==_vtxDist_B){ _ll_photon_B = _alg_emp.LL(false,shower_b._dedx,_vtxDist_B);}
	else{_ll_photon_B = _alg_emp.LL(false,shower_b._dedx,-1);}

    // sum up the likelihoods
	_ll += _ll_photon_A;
	_ll += _ll_photon_B;
	_ll = ll;
	_ll_tree->Fill();
		}// LL for pi0

	}

#endif
