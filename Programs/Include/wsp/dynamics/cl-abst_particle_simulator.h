//! WspAbstractParticleSimulator.h
// 


#ifndef __WSP_PARTICLE_SIMULATOR_H__
#define __WSP_PARTICLE_SIMULATOR_H__

#include "_define_dynamics.h"
#include <wsp/dynamics/cl-particle.h>
#include <wsp/common/_cl-noncopyable.hpp>


namespace wsp{ namespace dyn{

    class WSP_DLL_EXPORT AbstractParticleSimulator
        : public wsp::NonCopyable
    {
    public:
        AbstractParticleSimulator();
        virtual ~AbstractParticleSimulator();

        /* -----------------------
                 Getter 
           ----------------------- */
        inline wsp::dyn::NNPSAlgorithm searching_algorithm() const{ return searching_algorithm_; }
        inline wsp::dyn::ParticleList* particles(){ return particles_; }

        static f32 GetSimulationAreaWidth ();
        static f32 GetSimulationAreaHeight();
        static f32 GetSimulationAreaDepth ();
        static double GetParticleRadius();
        static double GetGravity();
        static double GetViscosityCoef();
        static double GetAffectedRadius();
        static double GetAffectedRadiusSquare();
        static double GetDensity();
        static double GetPressureCoef();

        /* -----------------------
                 Setter 
           ----------------------- */
        void SetSimulationAreaWidth (f32 width) ;
        void SetSimulationAreaHeight(f32 height);
        void SetSimulationAreaDepth (f32 depth) ;
        void SetParticleRadius      ( double value );
        void SetGravity             ( double value );
        void SetViscosityCoef       ( double value );
        void SetAffectedRadius      ( double value );
        void SetDensity             ( double value );
        void SetPressureCoef        ( double value );
        void SetParticleSearchAlgorithm( wsp::dyn::NNPSAlgorithm alg );
        void SetInternalForceAlgorithm( wsp::dyn::InternalForceAlgorithm int_force_alg );


        // 2D simulation methods
        virtual void EmitParticles2D(int x, int y, int nParticle)=0;
        virtual void Move2D(bool usePrevNeighbors=false)=0;

        // 3D simulation methods
        virtual void EmitParticles3D(int emitPosX, int emitPosY, int emitPosZ, int nParticle)=0;
        virtual void Move3D(bool usePrevNeighbors=false)=0;


    protected:
        wsp::dyn::NNPSAlgorithm          searching_algorithm_;
        wsp::dyn::InternalForceAlgorithm int_force_algorithm_;
        wsp::dyn::ParticleList           *particles_;
        wsp::Vector<wsp::dyn::PairLabel> pair_list_;
    };
}}


#endif