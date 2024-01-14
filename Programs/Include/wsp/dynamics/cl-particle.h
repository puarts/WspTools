/**
 * @file WspParticle.h
 * 
 * Any type of particle class header
 */


#ifndef __WSP_PARTICLE_H__
#define __WSP_PARTICLE_H__

#include "_define_dynamics.h"
#ifndef __WSP_COMMONUTIL_TEMPLATE_CLASS_VECTOR_H__
#include <wsp/common/tpl_cl-vector.h>
#endif
#include <wsp/common/tpl_cl-vector3.h>

#define INIT_WSP_PARTICLE density_(0),pressure_(0),smoothing_length_(1.0e-3/40.0),mass_(1.0e-3/1600.0)


namespace wsp{ namespace dyn{
    struct PairLabel{
        s32 label[2];
    };

    //! base particle class
    class WSP_DLL_EXPORT Particle
    {
    public:
        inline Particle(): INIT_WSP_PARTICLE{}
        inline Particle(const Point<double> &in_position)
            : INIT_WSP_PARTICLE
        {
            position_ = in_position;
            //position_.::wsp::VectorN<double>::operator=(in_position);
        }
        inline Particle(const wsp::dyn::Particle &in_particle)
            : INIT_WSP_PARTICLE
        {
            operator=(in_particle);
        }
        inline ~Particle(){}

        // Getter ------------------
        inline const wsp::Point<double>          position () const{ return position_; }
        inline const wsp::Vector3<double>        velocity () const{ return velocity_; }
        inline const wsp::Vector3<double>        force    () const{ return force_; }
        inline wsp::Vector3<double>&             force    ()      { return force_; }
        inline const double                      density  () const{ return density_; }
        inline double&                           density  ()      { return density_; }
        inline const double                      pressure () const{ return pressure_; }
        inline double&                           pressure ()      { return pressure_; }
        inline const Vector<wsp::dyn::Particle*> neighbors() const{ return neighbors_; }
        inline wsp::Vector<wsp::dyn::Particle*>& neighbors()      { return neighbors_; }
        inline const wsp::Color<float>&          color    () const{ return color_; }
        inline double                            smoothing_length() const{ return smoothing_length_; }
        inline double                            mass     () const{ return mass_; }

        inline double GetDistanceSquare(const wsp::dyn::Particle &in_prt) const{
            double dist_sq = (position_.x()-in_prt.position().x())*(position_.x()-in_prt.position().x())
                            +(position_.y()-in_prt.position().y())*(position_.y()-in_prt.position().y())
                            +(position_.z()-in_prt.position().z())*(position_.z()-in_prt.position().z());
            return dist_sq;
        }
        inline double GetDistance(const wsp::dyn::Particle &in_prt) const{
            return sqrt(GetDistanceSquare(in_prt));
        }
        //! mutator -----------------
        inline void set_density(double density){ density_=density; }
        inline void set_pressure(double pressure){ pressure_=pressure; }
        inline void set_mass(double mass){ mass_=mass; }
        inline void set_smoothing_length(double smoothing_length){ smoothing_length_=smoothing_length; }
        inline void set_color(const wsp::Color<float> &color){ color_ = color; }


        virtual void Move2D()=0;
        virtual void Move3D()=0;

        // Operator ----------------------------------------
        inline wsp::dyn::Particle& operator=(const wsp::dyn::Particle &rhs){
            position_ = rhs.position();
            velocity_ = rhs.velocity();
            force_ = rhs.force();
            density_ = rhs.density();
            pressure_ = rhs.pressure();
            neighbors_ = rhs.neighbors();
            smoothing_length_ = rhs.smoothing_length();
            mass_ = rhs.mass();
            return *this;
        }
        inline bool operator==(const wsp::dyn::Particle &rhs) const{
            bool equal = true;
            equal &= position_==rhs.position();
            equal &= velocity_==rhs.velocity();
            equal &= force_==rhs.force();
            equal &= density_==rhs.density();
            equal &= pressure_==rhs.pressure();  
            equal &= neighbors_==rhs.neighbors();
            equal &= smoothing_length_==rhs.smoothing_length();
            equal &= mass_==rhs.mass();
            return equal;
        }
        inline bool operator!=(const wsp::dyn::Particle &other) const{
            return !(*this==other);
        }
    protected:
        wsp::Point<double>               position_;
        wsp::Vector3<double>             velocity_;
        wsp::Vector3<double>             force_;
        double                           density_;
        double                           pressure_;
        wsp::Vector<wsp::dyn::Particle*> neighbors_;
        wsp::Color<float>                color_;
        double                           mass_;

        //! SPH parameters
        double smoothing_length_;
    };


    #define INIT_WSP_PARTICLE_LIST group_id_(-1)

    class ParticleList: public wsp::Vector<wsp::dyn::Particle*>
    {
    public:
        inline ParticleList():INIT_WSP_PARTICLE_LIST{}

        // Getter
        inline int group_id()const{ return group_id_; }

        // Mutators
        inline void set_group_id(int id){ group_id_ = id; }
        inline void ClearNeighbors(){
            for(u32 i=0; i<length_; i++){
                data_[i]->neighbors().Clear();
            }
        }
    private:
        int group_id_;

    };



}}

//inline std::ostream& operator<<(std::ostream& lhs, const wsp::dyn::Particle &rhs)
//{
//    lhs<<rhs.position();
//    return lhs;
//}
//inline std::ostream& operator<<(std::ostream& lhs, const wsp::dyn::Particle *rhs)
//{
//    lhs<<*rhs;
//    return lhs;
//}
#endif