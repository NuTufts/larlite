import ROOT as rt
from larlite import larlite
from ROOT import larutil as larutil
from ROOT import TVector3

name = "SBND"
detid = larlite.geo.kSBND
#name = "ICARUS"
#detid = larlite.geo.kICARUS

geo = larlite.larutil.Geometry.GetME( detid )

detp = larutil.DetectorProperties.GetME()
larp = larutil.LArProperties.GetME()
driftv = larp.DriftVelocity()
usec_per_tick = detp.SamplingRate()*1.0e-3
cm_per_tick = driftv*usec_per_tick
print("usec per tick=",usec_per_tick)
print("driftv=",driftv," cm/microsecond")
print("cmspertick=",cm_per_tick)

maxticks = detp.NumberTimeSamples()

#for name,detid in [("ICARUS",larlite.geo.kICARUS),("SBND",larlite.geo.kSBND),("uB",larlite.geo.kMicroBooNE)]:
#for name,detid in [("uB",larlite.geo.kMicroBooNE)]:

print("======== DETID ",name,": ",detid," ============")
for cryoid in range( geo.Ncryostats() ):
    for tpcid in range( geo.NTPCs(cryoid) ):

        posmin = TVector3()
        posmax = TVector3()
        geo.TPCBoundaries( posmin, posmax, tpcid, cryoid )
        ftpcdir = geo.TPCDriftDir( tpcid, cryoid )
        
        for planeid in range( geo.Nplanes(tpcid,cryoid) ):
            
            print("===============================")
            print("CRYOID,TPCID,PLANEID=(%d,%d,%d)"%(cryoid,tpcid,planeid))
            if ftpcdir[0]<0:
                print(" x anode: ", posmin[0] )
                print(" x cathode: ", posmax[0] )
            else:
                print(" x anode: ", posmin[0] )
                print(" x cathode: ", posmax[0] )
            print(" tpc drift dir: ",ftpcdir[0])                
            print(" xticksoffset: ",detp.GetXTicksOffset(planeid,tpcid,cryoid))
            print(" xtickscoeff: ",detp.GetXTicksCoefficient())
            print(" trigger offset: ",detp.TriggerOffset())
            print(" sampling rate: ",detp.SamplingRate())
            for ticks in [0,int(0.2*maxticks),int(0.4*maxticks),int(0.6*maxticks),int(0.8*maxticks),maxticks,detp.TriggerOffset()]:
                xconvert = detp.ConvertTicksToX(ticks,planeid,tpcid,cryoid)
                x = (ticks-detp.TriggerOffset())*cm_per_tick
                print(" tick=%d --> x=%.2f vs. %.2f"%(ticks,xconvert,x))
        
