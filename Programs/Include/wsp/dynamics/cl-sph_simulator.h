//! WspSphSimulator.h
// 

#ifndef _WSP_SPH_SIMULATOR_H_
#define _WSP_SPH_SIMULATOR_H_

#include <wsp/dynamics/cl-abst_particle_simulator.h>

//namespace wsp{ namespace dyn{
namespace wsp{ namespace dyn{
    
    class SphParticle;

    class WSP_DLL_EXPORT SphSimulator
        : public wsp::dyn::AbstractParticleSimulator
    {
    public:
        inline SphSimulator(){}

        //! 2D simulation methods
        void EmitParticles2D(int x, int y, int nParticle);
        void Move2D(bool usePrevNeighbors=false);

        //! 3D simulation methods
        void EmitParticles3D(int emitPosX, int emitPosY, int emitPosZ, int nParticle);
        void Move3D(bool usePrevNeighbors=false);

    };
}}
//}}



#endif
