import ROOT as rt
from larlite import larlite
from ROOT import larutil as larutil

for name,detid in [("ICARUS",larlite.geo.kICARUS),("SBND",larlite.geo.kSBND),("uB",larlite.geo.kMicroBooNE)]:
    print("======== DETID ",name,": ",detid," ============")
    geo = larlite.larutil.Geometry.GetME( detid )

    larp = larutil.LArProperties.GetME()

    planeid=0
    tpcid=0
    cryoid=0


