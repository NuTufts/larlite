#include "DataFormat/wrapper.h"
#include "lardataobj/RecoBase/Hit.h"
#include "lardataobj/RecoBase/Cluster.h"
#include "lardataobj/MCBase/MCShower.h"
#include "lardataobj/MCBase/MCTrack.h"

namespace DataFormats_Common {
  struct dictionary {
    larlite::wrapper<std::vector<recob::Hit> > dummywh;
    larlite::wrapper<std::vector<recob::Cluster> > dummywc;
    larlite::wrapper<std::vector<sim::MCShower> > dummyws;
    larlite::wrapper<std::vector<sim::MCTrack> > dummywt;
  };
}

