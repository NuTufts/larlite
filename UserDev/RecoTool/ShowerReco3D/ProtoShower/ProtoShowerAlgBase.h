/**
 * \file ProtoShowerAlgBase.h
 *
 * \ingroup ProtoShower
 *
 * \brief Class def header for a class ProtoShowerAlgBase
 *
 * @author david caratelli
 */

/** \addtogroup ProtoShower

    @{*/
#ifndef PROTOSHOWERALGBASE_H
#define PROTOSHOWERALGBASE_H

#include <iostream>
#include "DataFormat/storage_manager.h"
// #include "ShowerReco3D/Base/ShowerRecoException.h"


#include "ProtoShower.h"

/**
   \class ProtoShowerAlgBase
   User defined class ProtoShowerAlgBase ... these comments are used to generate
   doxygen documentation!
 */

namespace protoshower {

class ProtoShowerAlgBase {

public:

  /// Default constructor
  ProtoShowerAlgBase() { _name = "ProtoShowerAlgBase"; _debug = false;}

  /// Default destructor
  virtual ~ProtoShowerAlgBase() {}

  virtual void GenerateProtoShower(::larlite::storage_manager* storage,
                                   ::larlite::event_pfpart* ev_pfpart,
                                   const size_t proto_shower_pfpart,
                                   protoshower::ProtoShower & proto_shower) = 0;


  std::string name() { return _name; }

  bool debug() const { return _debug; }

  void setDebug(bool on) { _debug = on; }

protected:

  std::string _name;

  bool _debug;

};

}// namespace

#endif
/** @} */ // end of doxygen group

