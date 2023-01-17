import ROOT as rt
from larlite import larlite

#print("DETID (ICARUS): ",larlite.geo.kICARUS)
#geo = larlite.larutil.Geometry.GetME( larlite.geo.kICARUS )
#geo = larlite.larutil.Geometry.GetME( larlite.geo.kMicroBooNE )
geo = larlite.larutil.Geometry.GetME( larlite.geo.kSBND )
for icryo in range( geo.Ncryostats() ):
    for itpc in range( geo.NTPCs(icryo) ):
        from ROOT import TVector3
        posmin = TVector3()
        posmax = TVector3()
        
        geo.TPCBoundaries( posmin, posmax, itpc, icryo )
        ftpcdir = geo.TPCDriftDir( itpc, icryo )

        print("CRYO[%d] TPC[%d]"%(icryo,itpc))
        print(" Min Bound: (%.2f,%.2f,%.2f)"%(posmin[0],posmin[1],posmin[2]))
        print(" Max Bound: (%.2f,%.2f,%.2f)"%(posmax[0],posmax[1],posmax[2]))
        print(" TPC drift dir: (%.2f,%.2f,%.2f)"%(ftpcdir[0],ftpcdir[1],ftpcdir[2]))
