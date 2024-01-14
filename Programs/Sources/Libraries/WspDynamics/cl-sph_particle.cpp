//! WspSphParticle.cpp

#include <wsp/dynamics/cl-sph_particle.h>
#include <wsp/dynamics/cl-sph_simulator.h>



//! ======================================================================
//!                   SPH Particle calculation 
//! ======================================================================
double wsp::dyn::SphParticle::g_boxWidth = 512;
double wsp::dyn::SphParticle::g_boxHeight = 512;

void wsp::dyn::SphParticle::Move2D()
{
    //int rad = (int)g_radius;
    int rad = static_cast<int>(wsp::dyn::AbstractParticleSimulator::GetParticleRadius());
    double mass = 1.00;
    double dt = 0.1;
    double wall_friction=1.0;

    //if(position_[0]>rad && position_[0]<wsp::dyn::AbstractParticleSimulator::GetSimulationAreaWidth()-rad && position_[1]>rad && position_[1] <wsp::dyn::AbstractParticleSimulator::GetSimulationAreaHeight()-rad){
        velocity_[1] += wsp::dyn::AbstractParticleSimulator::GetGravity()/mass;
    //}

    velocity_[0] += force_[0]/mass;
    velocity_[1] += force_[1]/mass;


    position_[0] += velocity_[0]*dt;
    position_[1] += velocity_[1]*dt;


    if(position_[0]<rad){
        velocity_[0] += rad - position_[0];
        velocity_[0] *=wall_friction;
        //velocity_[1] *=wall_friction;
    }else if(position_[0]>wsp::dyn::AbstractParticleSimulator::GetSimulationAreaWidth()-rad){ 
        velocity_[0] += wsp::dyn::AbstractParticleSimulator::GetSimulationAreaWidth() - rad - position_[0];
        velocity_[0] *=wall_friction;
        //velocity_[1] *=wall_friction;
    }

    if(position_[1]<rad){
        velocity_[1] += rad - position_[1];
        //velocity_[0] *= wall_friction;
        velocity_[1] *=wall_friction;
    }else if(position_[1]>wsp::dyn::AbstractParticleSimulator::GetSimulationAreaHeight()-rad){
        velocity_[1] += wsp::dyn::AbstractParticleSimulator::GetSimulationAreaHeight() - rad - position_[1];
        //velocity_[0] *= wall_friction;
        velocity_[1] *=wall_friction;
    }
}



void wsp::dyn::SphParticle::Move3D()
{
    int rad = (int)wsp::dyn::SphSimulator::GetParticleRadius();
    double mass = 1;
    double dt = 0.01;
    velocity_[1] += wsp::dyn::SphSimulator::GetGravity();

    velocity_[0] += force_[0]/mass;
    velocity_[1] += force_[1]/mass;
    velocity_[2] += force_[2]/mass;

    position_[0] += velocity_[0]*dt;
    position_[1] += velocity_[1]*dt;
    position_[2] += velocity_[2]*dt;

    if(position_[0]<rad){
        velocity_[0] += (rad - position_[0]);
    }else if(position_[0]>wsp::dyn::AbstractParticleSimulator::GetSimulationAreaWidth()-rad){
        velocity_[0] += wsp::dyn::AbstractParticleSimulator::GetSimulationAreaWidth() - rad - position_[0];

    }

    if(position_[1]<rad){ 
        velocity_[1] += rad - position_[1]; 
    }else if(position_[1]>wsp::dyn::AbstractParticleSimulator::GetSimulationAreaHeight()-rad){
        velocity_[1] += wsp::dyn::AbstractParticleSimulator::GetSimulationAreaHeight() - rad - position_[1];
    }

    if(position_[2]<rad){ 
        velocity_[2] += rad - position_[2]; 
    }else if(position_[2]>wsp::dyn::AbstractParticleSimulator::GetSimulationAreaDepth()-rad){
        velocity_[2] += wsp::dyn::AbstractParticleSimulator::GetSimulationAreaDepth() - rad - position_[2];
    }
}
