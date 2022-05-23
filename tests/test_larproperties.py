import ROOT as rt
from larlite import larlite
from ROOT import larutil as larutil

print("DETID (ICARUS): ",larlite.geo.kICARUS)
geo = larlite.larutil.Geometry.GetME( larlite.geo.kICARUS )
#geo = larlite.larutil.Geometry.GetME( larlite.geo.kMicroBooNE )
larp = larutil.LArProperties.GetME()

planeid=0
tpcid=0
cryoid=0
#for ticks in [0,1000,2000,3000,4000,detp.TriggerOffset()]:
#    xconvert = detp.ConvertTicksToX(ticks,planeid,tpcid,cryoid)
#    print("CRYOID,TPCID,PLANEID=(%d,%d,%d)"%(cryoid,tpcid,planeid))
#    print(" tick=%d --> x=%.2f"%(ticks,xconvert))

