#ifndef __LARLITE_LARUTIL_OPDETGEO_H__
#define __LARLITE_LARUTIL_OPDETGEO_H__

// ROOT
#include "TVector3.h"

namespace larlite {
namespace larutil {

  class OpDetGeo {
  public:

    OpDetGeo();
    OpDetGeo( int op_ch, int cryo_id, const TVector3& center );
    virtual ~OpDetGeo() {};

    int opdetid;
    int cryoid;
    TVector3 fCenter;
    
  };

}
}

#endif
