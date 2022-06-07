import ROOT as rt
from larlite import larlite
from ROOT import larutil as larutil

#for name,detid in [("ICARUS",larlite.geo.kICARUS),("SBND",larlite.geo.kSBND),("uB",larlite.geo.kMicroBooNE)]:
for name,detid in [("uB",larlite.geo.kMicroBooNE)]:
    print("======== DETID ",name,": ",detid," ============")
    geo = larlite.larutil.Geometry.GetME( detid )
    detp = larutil.DetectorProperties.GetME()

    planeid=0
    tpcid=0
    cryoid=0

    print("CRYOID,TPCID,PLANEID=(%d,%d,%d)"%(cryoid,tpcid,planeid))    
    print(" xticksoffset: ",detp.GetXTicksOffset(planeid,tpcid,cryoid))
    print(" xtickscoeff: ",detp.GetXTicksCoefficient())
    print(" trigger offset: ",detp.TriggerOffset())
    for ticks in [0,1000,2000,3000,4000,detp.GetXTicksOffset(planeid,tpcid,cryoid)]:
        xconvert = detp.ConvertTicksToX(ticks,planeid,tpcid,cryoid)
        print(" tick=%d --> x=%.2f"%(ticks,xconvert))
        
