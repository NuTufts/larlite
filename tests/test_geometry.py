import ROOT as rt
from larlite import larlite

print("DETID (ICARUS): ",larlite.geo.kICARUS)
#geo = larlite.larutil.Geometry.GetME( larlite.geo.kICARUS )
geo = larlite.larutil.Geometry.GetME( larlite.geo.kMicroBooNE )
