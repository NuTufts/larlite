import os,sys
from math import fabs,sin,cos
import ROOT as rt
from ROOT import std
from larlite import larlite

"""
******************************************************************************
*Br    0 :fDetLength : fDetLength/D                                          *
*............................................................................*
*Br    1 :fDetHalfWidth : fDetHalfWidth/D                                    *
*............................................................................*
*Br    2 :fDetHalfHeight : fDetHalfHeight/D                                  *
*............................................................................*
*Br    3 :fCryoLength : fCryoLength/D                                        *
*............................................................................*
*Br    4 :fCryoHalfWidth : fCryoHalfWidth/D                                  *
*............................................................................*
*Br    5 :fCryoHalfHeight : fCryoHalfHeight/D                                *
*............................................................................*
*Br    6 :fCryostatBoundaries : vector<double>                               *
*............................................................................*
*Br    7 :fChannelToPlaneMap : vector<unsigned char>                         *
*............................................................................*
*Br    8 :fChannelToWireMap : vector<unsigned short>                         *
*............................................................................*
*Br    9 :fPlaneWireToChannelMap : vector<vector<unsigned short> >           *
*............................................................................*
*Br   10 :fSignalType : vector<larlite::geo::SigType_t>                      *
*............................................................................*
*Br   11 :fViewType : vector<larlite::geo::View_t>                           *
*............................................................................*
*Br   12 :fPlanePitch : vector<double>                                       *
*............................................................................*
*Br   13 :fWireStartVtx : vector<vector<vector<double> > >                   *
*............................................................................*
*Br   14 :fWireEndVtx : vector<vector<vector<double> > >                     *
*............................................................................*
*Br   15 :fWirePitch : vector<double>                                        *
*............................................................................*
*Br   16 :fWireAngle : vector<double>                                        *
*............................................................................*
*Br   17 :fOpChannelVtx : vector<vector<float> >                             *
*............................................................................*
*Br   18 :fOpDetVtx : vector<vector<float> >                                 *
*............................................................................*
*Br   19 :fOpChannel2OpDet : vector<unsigned int>                            *
*............................................................................*
*Br   20 :fPlaneOriginVtx : vector<vector<double> >                          *
*............................................................................*
"""

ubfile_in = rt.TFile("larutil_microboone.root")
old = ubfile_in.Get("scanner/Geometry")
old.GetEntry(0)

outfile = rt.TFile("microboone_larlite_geodata.root","recreate")
tgeodata = rt.TTree("geodata", "MicroBooNE Geo Data")
cryo_v = std.vector("larlite::larutil::CryoGeo")()
tgeodata.Branch( "cryo_v", cryo_v )

# define planes
for iplane in range( old.fPlaneOriginVtx.size() ):
    plane_origin = old.fPlaneOriginVtx.at(iplane)
    print("[%d] "%(iplane),end='')
    print("(",end='')
    for i in range( plane_origin.size() ):
        print("%.2f "%(plane_origin[i]),end='')
    print(")")
    
for i in range( old.fWireAngle.size() ):
    print("wireangle[",i,"] ",old.fWireAngle[i])

# define cryo
#for i in range(old.fCryostatBoundaries.size()):
#    print(old.fCryostatBoundaries.at(i)," ")
#print()
cryo = larlite.larutil.CryoGeo(0)
cryo.fCenter[0] = 0.5*(old.fCryostatBoundaries[1]+old.fCryostatBoundaries[0])
cryo.fCenter[1] = 0.5*(old.fCryostatBoundaries[3]+old.fCryostatBoundaries[2])
cryo.fCenter[2] = 0.5*(old.fCryostatBoundaries[5]+old.fCryostatBoundaries[4])
cryo.fBounds.resize(2)
cryo.fBounds[0] = rt.TVector3( old.fCryostatBoundaries[0], old.fCryostatBoundaries[2], old.fCryostatBoundaries[4] )
cryo.fBounds[1] = rt.TVector3( old.fCryostatBoundaries[1], old.fCryostatBoundaries[3], old.fCryostatBoundaries[5] )
cryo.fHalfLengths.resize(3,0)
cryo.fHalfLengths[0] = 0.5*fabs(old.fCryostatBoundaries[1]-old.fCryostatBoundaries[0])
cryo.fHalfLengths[1] = 0.5*fabs(old.fCryostatBoundaries[3]-old.fCryostatBoundaries[2])
cryo.fHalfLengths[2] = 0.5*fabs(old.fCryostatBoundaries[5]-old.fCryostatBoundaries[4])

# define tpc
tpc  = larlite.larutil.TPCGeo(0,0) # tpc_id=0, cryo_id=0
tpc.fBounds.resize(2)
tpc.fHalfLengths = rt.TVector3( 0.5*256.0, 117.0, 518.50 )
tpc.fCenter      = rt.TVector3( 0.5*256.0, 0.97, 518.50 )
tpc.fBounds[0]   = rt.TVector3(   0.0, 0.97-117.0, 0.0 )
tpc.fBounds[1]   = rt.TVector3( 256.0, 0.97+117.0, 1037.0 )
tpc.fDriftDirection = rt.TVector3( -1.0, 0.0, 0.0 )
tpc.planes_v.clear()

for iplane in range(3):
    print("[",iplane,"] sigtype=",old.fSignalType[iplane]," viewtype=",old.fViewType[iplane])
    plane = larlite.larutil.PlaneGeo( iplane, 0, 0, old.fViewType[iplane], old.fSignalType[iplane] )
    plane_origin = old.fPlaneOriginVtx.at(iplane)    
    plane.fCenter = rt.TVector3( plane_origin[0], plane_origin[1], plane_origin[2] )
    plane.fNormToCathode = rt.TVector3(1.0,0,0)
    plane.fWirePitchLen = old.fWirePitch[iplane]
    wire_angle = old.fWireAngle.at(iplane)
    plane.fWirePitchDir = rt.TVector3( 0, -cos(wire_angle), sin(wire_angle) )
    if iplane==2:
        plane.fWirePitchDir[1] = 0.0
    print("   wire-dir=(",plane.fWirePitchDir[0],",",plane.fWirePitchDir[1],",",plane.fWirePitchDir[2],") mag=",plane.fWirePitchDir.Mag())
    plane.fBoundingBox.resize(2)
    plane.fBoundingBox[0] = rt.TVector3( plane_origin[0], plane_origin[0]-117.0, plane_origin[2]-518.50 )
    plane.fBoundingBox[1] = rt.TVector3( plane_origin[0], plane_origin[0]+117.0, plane_origin[2]+518.50 )
    tpc.planes_v.push_back( plane )

# define the wires
for iplane in range(3):
    print("[plane ",iplane,"]")
    print("  plane num wire starts: ",old.fWireStartVtx[iplane].size())
    print("  plane num wire ends:   ",old.fWireEndVtx[iplane].size())
    nwires = old.fWireStartVtx[iplane].size()
    tpc.planes_v[iplane].fWires_v.clear()
    for iwire in range(nwires):
        ch = old.fPlaneWireToChannelMap[iplane][iwire]
        #print("  plane-wire[",iwire,"] ch=",ch)
        wstart = old.fWireStartVtx[iplane][iwire]
        wend   = old.fWireEndVtx[iplane][iwire]
        vstart = rt.TVector3( wstart[0], wstart[1], wstart[2] )
        vend   = rt.TVector3( wend[0],   wend[1],   wend[2] )
        wiregeo = larlite.larutil.WireGeo( ch, iwire, iplane, 0, 0, vstart, vend )
        tpc.planes_v[iplane].fWires_v.push_back( wiregeo )

# add the tpc to the cryostart
cryo.tpc_v.clear()
cryo.tpc_v.push_back( tpc )
    
# define opdets
nopdets = old.fOpDetVtx.size()
cryo.opdet_v.clear()
print("num opdets: ",nopdets)
for iopdetch in range(nopdets):
    opdetvtx = old.fOpDetVtx[iopdetch]
    opdet = larlite.larutil.OpDetGeo( iopdetch, 0, rt.TVector3(opdetvtx[0],opdetvtx[1],opdetvtx[2]) )
    cryo.opdet_v.push_back( opdet )
cryo_v.push_back( cryo )
tgeodata.Fill()

tgeodata.Write()
outfile.Close()
