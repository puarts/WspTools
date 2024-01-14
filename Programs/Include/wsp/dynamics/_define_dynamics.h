//! WspParticle.h
// 

#ifndef __WSP_DYNAMICS_DEFINITION_H_
#define __WSP_DYNAMICS_DEFINITION_H_

#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include <wsp/common/_define_commoncore.h>
#endif


namespace wsp{ namespace dyn{

    //! Nearest Neighboring Particle Searching Algorithm
    typedef enum{
        NNPS_ALL_PAIR_SEARCH=0,
        NNPS_LINKED_LIST_SEARCH=1,
        NNPS_TREE_SEARCH=2
    }NNPSAlgorithm;

    typedef enum{
        INTERNAL_FORCE_LENNARD_JONES,
        INTERNAL_FORCE_SPH
    }InternalForceAlgorithm;

    typedef enum{
        SPH_KERNEL_CUBIC_SPLINE,
        SPH_KERNEL_GAUSS,
        SPH_KERNEL_QUINTIC
    }SPHKernelType;

}}

#endif