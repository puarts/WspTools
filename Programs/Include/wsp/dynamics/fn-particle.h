//! WspFnParticle.h
// 

#ifndef __WSP_FN_PARTICLE_H__
#define __WSP_FN_PARTICLE_H__

#include <wsp/dynamics/cl-particle.h>
#include <wsp/common/tpl_cl-grid_cell.h>
namespace wsp{ namespace dyn{
    typedef wsp::GridCellList<wsp::dyn::Particle*> ParticleLinkedList;
}}

namespace wsp{ namespace dyn{

    //! Nearest Neighboring Searching Algorithm ----------------------------
    WSP_DLL_EXPORT void SearchNeighbor_AllPairSearch(
            wsp::dyn::ParticleList &particle_list, 
            double affect_radius, 
            wsp::Vector<wsp::dyn::PairLabel> *label_list=NULL
        );

    WSP_DLL_EXPORT void SearchNeighbor_LinkedListSearch(wsp::dyn::ParticleList &particle_list, wsp::dyn::ParticleLinkedList &linked_list, double affect_radius);
    WSP_DLL_EXPORT void SearchNeighbor_TreeSearch(wsp::dyn::ParticleList &particle_list);

    //! Energy computation --------------------------------------------
    WSP_DLL_EXPORT void ComputeLennardJonesForce(wsp::dyn::ParticleList &particle_list, 
        double particle_radius, double min_replusion_radius_ratio=0.5, double epsilon = 0.0001);
    WSP_DLL_EXPORT void ComputeSPHForce(wsp::dyn::ParticleList &particle_list, 
        double affect_radius, double min_density, double particle_radius, double pressure_coef, double viscosity_coef);

    //! Other ---------------------------------------------------
    WSP_DLL_EXPORT void SPH();
    // ----------------------------------------------------------------------
    //! Calculate the smoothing kernel and its derivatives(gradient).
    //! if kernel_type = 1, cubic spline kernel by W4 - Spline (Monaghan 1985)
    //!               2, Gauss kernel (Gingold and Monaghan 1981)
    //!               3, Qu32ic kernel (Morris 1997)
    //! [in]
    //! r  : Distance between particles i and j
    //! dx : Vector that is (xi - xj)
    //! h : Smoothing length
    //! 
    //! [out]
    //! w : Kernel Value wij
    //! grad_w : gradient value of Kernel function between particles i and j
    // -----------------------------------------------------------------------
    WSP_DLL_EXPORT wsp::State GetSPHKernel(double &o_w, double *o_dwdx, double dist, double *dx, double h, u32 dimension=3, 
        wsp::dyn::SPHKernelType kernel_type=wsp::dyn::SPH_KERNEL_CUBIC_SPLINE);
}}
//}}


#endif