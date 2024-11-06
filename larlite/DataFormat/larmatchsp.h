#ifndef __LARLITE_DATAFORMAT_LARMATCHSP_H__
#define __LARLITE_DATAFORMAT_LARMATCHSP_H__

#include "data_base.h"
#include "larlite/Base/DataFormatConstants.h"
#include <vector>

/**
 * \file larmatchsp.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class definition for larmatchsp, which stores output of the larmatch CNN
 *
 * @author T. Wongjirad - Tufts - 11/2024
 */

/** \addtogroup DataFormat

    @{*/

namespace larlite {
  /**
     \class larmatchsp
     \brief Store larmatch CNN outputs

    Each instance of the object represents a spacepoint. For each spacepoint,
    we can associate outputs of the larmatch CNN. 
    We can also specify metadata relating the spacepoint to the 2D images.
  */
  class larmatchsp : public data_base {
    
  public:

    enum { KP_NU=0, KP_TRACKSTART, KP_TRACKEND, KP_SHOWER, KP_MICHEL, KP_DELTA, KP_NUM_CLASSES };
    enum { SSNET_ELECTRON=0, SSNET_GAMMA, SSNET_MUON, SSNET_PROTON, SSNET_PION, SSNET_NUM_CLASSES };
    enum { PLANE_U=0, PLANE_V, PLANE_Y, NUM_PLANES };

    larmatchsp()
    : data_base(data::kLArMatchSP),
      tick(0.0),
      index(0),
      truthflag(false)
    {
        clear_vectors();
    };

    virtual ~larmatchsp()
    {};

    float get_nu_keypoint_score() { return keypoint_class_score_v.at(KP_NU); };
    void clear_vectors();

    // CNN outputs
    std::vector<float> true_vs_ghost_logit;
    std::vector<float> keypoint_class_score_v;
    std::vector<float> ssnet_score_v;
    std::vector<float> particle_dir_v;
    std::vector<float> feat_v; ///< place to store the feature vector output by the u-resnet (not usually filled)

    // Detector positions
    std::vector<float> pos_v;

    // Image positions
    std::vector<float> wire_v;
    float tick;

    // image array positions
    std::vector<int> col_v;
    int row;

    // Image feature values
    std::vector<float> plane_pixval_v;

    // index number
    unsigned long index; ///< for tracking/book-keeping
    bool truthflag;

  };


  class event_larmatchsp : public std::vector<larlite::larmatchsp>, public event_base {
  public:
      
    /// Default constructor
    event_larmatchsp(std::string name="noname") : event_base(data::kLArMatchSP,name) { clear_data(); }
    
    
    /// Default copy constructor
    event_larmatchsp(const event_larmatchsp& original) : std::vector<larlite::larmatchsp>(original), event_base(original)
    {}
    
    /// Default destructor
    ~event_larmatchsp(){};
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();};
    
  private:
    
  };


}
#endif
