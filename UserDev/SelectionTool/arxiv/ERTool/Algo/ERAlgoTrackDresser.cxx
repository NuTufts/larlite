#ifndef ERTOOL_ERALGOTRACKDRESSER_CXX
#define ERTOOL_ERALGOTRACKDRESSER_CXX

#include "ERAlgoTrackDresser.h"

namespace ertool {

	ERAlgoTrackDresser::ERAlgoTrackDresser(const std::string& name)
		: AlgoBase(name)
	{}

	void ERAlgoTrackDresser::Reset()
	{
		std::cout << "\t\t" << __FUNCTION__ << " Reset " << std::endl;
	}

	void ERAlgoTrackDresser::AcceptPSet(const ::fcllite::PSet& cfg)
	{ return;  }

	void ERAlgoTrackDresser::ProcessBegin()
	{ return;  }

	bool ERAlgoTrackDresser::Reconstruct(const EventData &data, ParticleGraph& graph)
	{
		//Step 1: Grab a Shower
		//Step 2: Compare it against all the tracks
		//Step 3: Avoid the start points
		//Step 4: Don't avoid the ends, tagging me some Michele's
		//Step 5: Compare a shower to the trunck of the track
		//Step 6: Find the track that the shower is closest to
		//Step 7: Attach the shower to that track

		//Iterate through all the showers
		for (auto const& s : graph.GetParticleNodes(RecoType_t::kShower)) {

			if (graph.GetParticle(s).Descendant()) continue;

			auto const& show = data.Shower(graph.GetParticle(s).RecoID());

			geoalgo::Point_t showStart(3);
			showStart = show.Start();

			//Keep track of the closest muon
			double min_dist = 9999;
			int trk_ID = -99;

			for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack)) {

				auto const& trk = data.Track(graph.GetParticle(t).RecoID());

				if (trk.Length() < 0.3) continue;

				double showStart_trk_sqdist = _geoAlgo.SqDist(showStart, trk);
				double showStart_trkStart_sqdist = showStart.SqDist(trk.front());
				double showStart_trkEnd_sqdist = showStart.SqDist(trk.back());

				//If the shower and track are:
				//1) Too far apart, OR
				//2) coming from the same point
				// Continue...
				if (showStart_trk_sqdist > (_minDist * _minDist) ||
				        showStart_trkStart_sqdist < (_strtDist * _strtDist) ) continue;

				//Now we check to see if the shower could be :
				//1) a delta-ray
				//2) a michele
				// if so store the distance and then we will check to make sure no
				// other muon track is a better match for that muon
				if (showStart_trk_sqdist < (_deltaDist * _deltaDist) ||
				        showStart_trkEnd_sqdist < (_endDist * _endDist) ) {

					//is it a delta?
					if ( showStart_trk_sqdist < min_dist * min_dist ) {
						min_dist = sqrt(showStart_trk_sqdist);
						trk_ID = graph.GetParticle(t).ID();
					}

					//is it a michele
					if ( showStart_trkEnd_sqdist < min_dist * min_dist ) {
						min_dist = sqrt(showStart_trkEnd_sqdist);
						trk_ID = graph.GetParticle(t).ID();
					}

				} //Possible delta or michele

			}//iterate through all the tracks

			if (trk_ID != -99) {
				graph.SetParentage(trk_ID, graph.GetParticle(s).ID());
			}

		}//iterate through all the showers

		return true;

	}

	void ERAlgoTrackDresser::ProcessEnd(TFile* fout)
	{}

}

#endif
