/** 
 * @file cl-sph_simulator.cpp
 * 
 */

#include <wsp/common/tpl_cl-vector3.h>
#include <wsp/dynamics/cl-sph_simulator.h>
#include <wsp/dynamics/cl-particle.h>
#include <wsp/dynamics/fn-particle.h>
#include <wsp/dynamics/cl-particle.h>
#include <wsp/common/fn-debug.h>

#include <wsp/dynamics/fn-dynamics_log.h>
#include <wsp/dynamics/cl-sph_particle.h>

#include <math.h>

void wsp::dyn::SphSimulator::EmitParticles2D(int pourPoint_x, int pourPoint_y, int nParticle)
{
    int i, j, x, y;
    int pourWidth = (int)GetParticleRadius()+3;
    int len = (int)sqrt((double)nParticle);
    int start= -len/2; 
    int end = start+len;

    for(i=start; i<end; i++){
        x = pourPoint_x + i*pourWidth;
        for(j=start; j<end; j++){
            y = pourPoint_y + j*pourWidth;
            wsp::dyn::SphParticle *p = WSP_NEW wsp::dyn::SphParticle(wsp::Point<double>(x, y));
            //cout<<wsp::Point<double>(x, y)<<endl;

            particles_->Append(p);
            //(*particles_)[(int)particles_->length()-1]->velocity()[0] = i*pourWidth * 0.01;
            //(*particles_)[(int)particles_->length()-1]->velocity()[1] = j*pourWidth * 0.01;
            //cout<<(*particles_)[(int)particles_->length()-1]->position()[0]<<endl;
        }
    }
    WSP_DYNAMICS_DEBUG_LOG("Emitting particle has done %d", particles_->length());

    //! making pair_list for iteration of comparison of particle pair
    int index;
    len = static_cast<int>(particles_->length());
    pair_list_.SetLength((particles_->length()-1)*particles_->length()/2);
    for(i=0, index=0; i<len; i++){
        for(j=i+1; j<len; j++, index++){
            pair_list_[index].label[0] = i;
            pair_list_[index].label[1] = j;
        }
    }
}



//! calculate particle position
void wsp::dyn::SphSimulator::Move2D(bool usePrevNeighbors)
{
    int i, j;
    double dist;
    wsp::dyn::Particle *pi, *pj;
    double dx, dy;
    double weight, pressureWeight, viscosityWeight;

    int nParticles = (int)particles_->length();
    if(nParticles==0){
        return;
    }

    //! nearest neighboring particle searching -------------
    if(usePrevNeighbors==false){
        switch(searching_algorithm_)
        {
        case wsp::dyn::NNPS_ALL_PAIR_SEARCH:
            //wsp::dyn::SearchNeighbor_AllPairSearch(*particles_, GetAffectedRadius(), &pair_list_);
            wsp::dyn::SearchNeighbor_AllPairSearch(*particles_, GetAffectedRadius(), NULL);
            break;
        case wsp::dyn::NNPS_LINKED_LIST_SEARCH:
            {
                double width = GetAffectedRadius()*2;
                wsp::Vector3<u32> num_grid(static_cast<u32>(ceil(GetSimulationAreaWidth()/width)), static_cast<u32>(ceil(GetSimulationAreaHeight()/width)), static_cast<u32>(1)); 
                wsp::dyn::ParticleLinkedList linked_list(num_grid, wsp::Vector3<double>(width,width,width));
                wsp::dyn::SearchNeighbor_LinkedListSearch(*particles_, linked_list, GetAffectedRadius());
                break;
            }
        case wsp::dyn::NNPS_TREE_SEARCH:
            WSP_FATAL( "Tree search algorithm is not implemented" );
            break;
        default:
            WSP_FATAL("Unknown nearest neighboring particles searching algorithm");
        }
    }

    //! internal force computation -------------------
    switch(int_force_algorithm_)
    {
    case wsp::dyn::INTERNAL_FORCE_SPH:
        wsp::dyn::ComputeSPHForce(*particles_, GetAffectedRadius(), GetDensity(), GetParticleRadius(), GetPressureCoef(), GetViscosityCoef());
        break;
    case wsp::dyn::INTERNAL_FORCE_LENNARD_JONES:
        wsp::dyn::ComputeLennardJonesForce(*particles_, GetParticleRadius(), 0.9);
        break;
    default:
        WSP_FATAL("Unknown internal force computation algorithm");
    }
    
    
    for(i=0; i<nParticles; i++){
        reinterpret_cast<wsp::dyn::SphParticle*>((*particles_)[i])->Move2D();
    }
}


// =================================
//! wsp::dyn::SphSimulator3D
// =================================


void wsp::dyn::SphSimulator::EmitParticles3D(int pourPoint_x, int pourPoint_y, int pourPoint_z, int nParticle)
{
    int i, j, k, x, y, z;
    int pourWidth = (int)GetParticleRadius()+3;
    int len = (int)pow((double)nParticle, 1/3.0);
    int start= -len/2; 
    int end = start+len;
    #ifdef _OPENMP
    #pragma omp parallel for private(i)
    #endif
    for(i=start; i<=end; i++){
        x = pourPoint_x + i*pourWidth;
        for(j=start; j<=end; j++){
            y = pourPoint_y + j*pourWidth;
            //wsp::dyn::SphParticle *p = WSP_NEW wsp::dyn::SphParticle(x, y); 
            //particles_->Append(p);
            for(k=start; k<=end; k++){
                z = pourPoint_z + k*pourWidth;
                wsp::dyn::SphParticle *p = WSP_NEW wsp::dyn::SphParticle(wsp::Point<double>(x, y, z));
                particles_->Append(p);
                //(*particles_)[(int)particles_->length()-1]->velocity()[0] = i*pourWidth * 0.01;
                //(*particles_)[(int)particles_->length()-1]->velocity()[1] = j*pourWidth * 0.01;
                //cout<<(*particles_)[(int)particles_->length()-1]->position()[0]<<endl;
            }
        }
    }
}

void wsp::dyn::SphSimulator::Move3D(bool usePrevNeighbors)
{
    int nParticles=(int)particles_->length();

    //#ifdef _OPENMP
    //#pragma omp parallel
    //#endif
    {
        int i, j, k;
        double dist;
        double dx, dy, dz;
        double weight, pressureWeight, viscosityWeight;
        wsp::dyn::Particle *pi, *pj;

        if(usePrevNeighbors==false){
            //! initialize neighbors of particles
            //#ifdef _OPENMP
            //#pragma omp for private(i)
            //#endif
            for(i=0; i<nParticles; i++){
                pi = (*particles_)[i];
                pi->neighbors().Clear();
            }
            
            //#ifdef _OPENMP
            //#pragma omp for private(i, j)
            //#endif
            for(i=0; i<nParticles; i++){
                pi = (*particles_)[i];
                for(j=i+1; j<nParticles; j++){
                    pj = (*particles_)[j];
                    if(pi->GetDistanceSquare(*pj) < GetAffectedRadiusSquare()){
                        pi->neighbors().Append(pj);
                        pj->neighbors().Append(pi);
                    }
                }
            }
        }

        //! calculation of density and pressure for each particles
        //#ifdef _OPENMP
        //#pragma omp for private(i, j)
        //#endif
        for(i=0; i<nParticles; i++){
            pi = (*particles_)[i];
            pi->set_density(0.0);
            for(j=0; j<(int)pi->neighbors().length(); j++){
                pj = reinterpret_cast<wsp::dyn::SphParticle*>(pi->neighbors()[j]);
                dist = pi->GetDistance(*pj);
                pi->set_density( pi->density() + (1 - dist/GetAffectedRadius())*(1 - dist/GetAffectedRadius()));
            }
            if(pi->density()<GetDensity()){
                pi->set_density( GetDensity() );
            }
            pi->set_pressure( pi->density() - GetDensity());
        }
        //#ifdef _OPENMP
        //#pragma omp parallel for private(i, j)
        //#endif
        for(i=0; i<nParticles; i++){
            pi = (*particles_)[i];
            pi->force()[0] = 0.0;
            pi->force()[1] = 0.0;
            pi->force()[2] = 0.0;
            for(j=0; j<(int)pi->neighbors().length(); j++){
                pj=reinterpret_cast<wsp::dyn::SphParticle*>(pi->neighbors()[j]);

                //! calculate linear weight from distance with neighbors ---------
                dx = pi->position()[0] - pj->position()[0];
                dy = pi->position()[1] - pj->position()[1];
                dz = pi->position()[1] - pj->position()[1];
                dist = sqrt(dx*dx + dy*dy + dz*dz);
                weight = 1 - dist/GetAffectedRadius();
                // -------------------------------------------------------

        //        pressureWeight = ((pi->m_pressure+pj->m_pressure)/(2*pj->m_density)) * wsp::dyn::ParticleSimulator::GetPressureCoef() * weight;
        //        dist = 1/dist;
        //        dx*=dist;
        //        dy*=dist;
        //        dz*=dist;
        //        pi->force()[0] += dx * pressureWeight;
        //        pi->force()[1] += dy * pressureWeight;
        //        pi->force()[2] += dz * pressureWeight;

                viscosityWeight = (weight / pj->density()) * GetViscosityCoef();
        //        dx = pi->velocity()[0] - pj->velocity()[0];
        //        dy = pi->velocity()[1] - pj->velocity()[1];
        //        dz = pi->velocity()[2] - pj->velocity()[2];
                //viscosityWeight = 0.01;
                pi->force()[0] -= dx * viscosityWeight;
                pi->force()[1] -= dy * viscosityWeight;
                pi->force()[2] -= dz * viscosityWeight;

            }
        }
        //#ifdef _OPENMP
        //#pragma omp for private(i)
        //#endif
        for(i=0; i<nParticles; i++){
            (*particles_)[i]->Move3D();
        }
    }
}

