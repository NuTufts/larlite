#include "larmatchsp.h"


namespace larlite {

    void larmatchsp::clear_vectors()
    {

        keypoint_class_score_v.clear();
        true_vs_ghost_logit.clear();
        ssnet_score_v.clear();
        particle_dir_v.clear();
        pos_v.clear();
        wire_v.clear();
        plane_pixval_v.clear();

        keypoint_class_score_v.resize( KP_NUM_CLASSES, 0.0 );
        ssnet_score_v.resize( SSNET_NUM_CLASSES, 0.0 );
        particle_dir_v.resize( 3, 0.0 );
        true_vs_ghost_logit.resize(2,0.0);
        pos_v.resize( 3, 0.0 );
        wire_v.resize( NUM_PLANES, 0.0 );
        plane_pixval_v.resize( NUM_PLANES, 0.0 );

    }


}