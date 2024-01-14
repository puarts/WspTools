//! cl-sph_particle.h
// 

#ifndef __WspSphParticle_H__
#define __WspSphParticle_H__

#include <wsp/dynamics/cl-particle.h>

namespace wsp{ namespace dyn{

    class WSP_DLL_EXPORT SphParticle
        : public wsp::dyn::Particle
    {
    public:
        inline SphParticle(){}
        inline SphParticle(const wsp::Point<double> &in_position): wsp::dyn::Particle(in_position){}
        inline SphParticle(const wsp::dyn::Particle &particle)
            :wsp::dyn::Particle(particle)
        {
            this->::wsp::dyn::Particle::operator=(particle);
        }
        inline ~SphParticle(){}

        virtual void Move2D();
        virtual void Move3D();

    public:
        static double g_boxWidth;
        static double g_boxHeight;
        static double g_radius;
    };
}}

#endif