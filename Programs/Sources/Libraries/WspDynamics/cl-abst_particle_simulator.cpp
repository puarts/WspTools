
#include <wsp/dynamics/cl-particle.h>
#include <wsp/dynamics/cl-abst_particle_simulator.h>
#include <wsp/dynamics/fn-dynamics_log.h>

//! default params

namespace{
    f32 g_sim_area_width  = 465;
    f32 g_sim_area_height = 465;
    f32 g_sim_area_depth  = 465;

    double g_particleRadius = 20.0;
    double g_gravity        = 0.00098000;
    double g_affectRad      = g_particleRadius + 7.0; //! effective radius
    double g_affectRadSq    = g_affectRad*g_affectRad; //! square of effective radius
    double g_affectRadSq3D  = g_affectRad*g_affectRad*g_affectRad;
    double g_density        = 2.00; //! density of fluid
    double g_pressureCoef   = 2.00; //! coeficient of pressure
    double g_viscosityCoef  = 0.0750; //! coeficient of viscosity
}

//g_particleRadius(20.0),\
//g_gravity(0.00098000),\
//g_affectRad(wsp::dyn::AbstractParticleSimulator::g_particleRadius+7.0),\
//g_affectRadSq(g_affectRad*g_affectRad),\
//g_density(2.00),\
//g_pressureCoef(2.00),\
//g_viscosityCoef(0.0750)



#define INIT_WSP_PARTICLE_SIMULATOR \
    searching_algorithm_(wsp::dyn::NNPS_ALL_PAIR_SEARCH),\
    int_force_algorithm_(wsp::dyn::INTERNAL_FORCE_SPH),\
    particles_(new wsp::dyn::ParticleList())



wsp::dyn::AbstractParticleSimulator::AbstractParticleSimulator()
    :INIT_WSP_PARTICLE_SIMULATOR
{}

wsp::dyn::AbstractParticleSimulator::~AbstractParticleSimulator()
{ 
    if(particles_!=NULL){
        for(u32 i=0; i<particles_->length(); i++){
            delete (*particles_)[i];
        }
    }
    delete particles_; 
}

f32 wsp::dyn::AbstractParticleSimulator::GetSimulationAreaWidth () { return g_sim_area_width; }
f32 wsp::dyn::AbstractParticleSimulator::GetSimulationAreaHeight() { return g_sim_area_height; }
f32 wsp::dyn::AbstractParticleSimulator::GetSimulationAreaDepth () { return g_sim_area_depth; } 
void wsp::dyn::AbstractParticleSimulator::SetSimulationAreaWidth (f32 width) { g_sim_area_width = width; }
void wsp::dyn::AbstractParticleSimulator::SetSimulationAreaHeight(f32 height){ g_sim_area_height= height; }
void wsp::dyn::AbstractParticleSimulator::SetSimulationAreaDepth (f32 depth) { g_sim_area_depth = depth; } 
void wsp::dyn::AbstractParticleSimulator::SetParticleSearchAlgorithm(wsp::dyn::NNPSAlgorithm alg)
{
    searching_algorithm_ = alg;
}
void wsp::dyn::AbstractParticleSimulator::SetInternalForceAlgorithm( wsp::dyn::InternalForceAlgorithm int_force_alg )
{
    int_force_algorithm_ = int_force_alg;
}
double wsp::dyn::AbstractParticleSimulator::GetParticleRadius()
{
    return g_particleRadius;
}

double wsp::dyn::AbstractParticleSimulator::GetGravity()
{
    return g_gravity;
}

double wsp::dyn::AbstractParticleSimulator::GetViscosityCoef()
{
    return g_viscosityCoef;
}

double wsp::dyn::AbstractParticleSimulator::GetAffectedRadius()
{
    return g_affectRad;
}

double wsp::dyn::AbstractParticleSimulator::GetAffectedRadiusSquare()
{
    return g_affectRadSq;
}

double wsp::dyn::AbstractParticleSimulator::GetDensity()
{
    return g_density;
}

double wsp::dyn::AbstractParticleSimulator::GetPressureCoef()
{
    return g_pressureCoef;
}

void wsp::dyn::AbstractParticleSimulator::SetParticleRadius( double value )
{
    g_particleRadius = value;
}
void wsp::dyn::AbstractParticleSimulator::SetGravity( double value )
{
    g_gravity = value;
}
void wsp::dyn::AbstractParticleSimulator::SetViscosityCoef( double value )
{
    g_viscosityCoef = value;
}
void wsp::dyn::AbstractParticleSimulator::SetAffectedRadius( double value )
{
    g_affectRad = value;
    g_affectRadSq = value*value;
    g_affectRadSq3D = value*value*value;
}
void wsp::dyn::AbstractParticleSimulator::SetDensity( double value )
{
    g_density = value;
}
void wsp::dyn::AbstractParticleSimulator::SetPressureCoef( double value )
{
    g_pressureCoef = value;
}
