/**
 * \file ChargeAnalytical.h
 *
 * \ingroup Algorithms
 * 
 * \brief Class def header for a class ChargeAnalytical
 *
 * @author matthias
 */

/** \addtogroup Algorithms

    @{*/
#ifndef CHARGEANALYTICAL_H
#define CHARGEANALYTICAL_H

#include "OpT0Finder/Base/BaseFlashMatch.h"


#include <iostream>

/**
   \class ChargeAnalytical
   User defined class ChargeAnalytical ... these comments are used to generate
   doxygen documentation!
 */
namespace flashana {
    class ChargeAnalytical : public BaseFlashMatch {

    public:

        /// Default constructor
        ChargeAnalytical() { }

        /// Default destructor
        ~ChargeAnalytical() { }

        void FillEstimate(const QCluster_t&, Flash_t&);
    };
}

#endif
/** @} */ // end of doxygen group 

