//! WspFnParticle.cpp
// 

//#include <omp.h>

#include <wsp/dynamics/cl-particle.h>
#include <wsp/dynamics/fn-particle.h>
#include <wsp/math/_fn-math_core.h>
#include <wsp/math/_define_mathutil.h>
#include <wsp/common/_tpl_cl-stackarray.h>


//! Nearest Neighboring Search Algorithms -------------------------------------

void wsp::dyn::SearchNeighbor_AllPairSearch(
    wsp::dyn::ParticleList &io_particles, 
    double affect_radius, 
    wsp::Vector<wsp::dyn::PairLabel> *label_list
)
{
    int len = static_cast<int>(io_particles.length());
    if(label_list!=NULL){
        if(static_cast<int>(label_list->length()) != (len-1)*len/2){
            WSP_COMMON_ERROR_LOG("number of element of label_list is wrong\n"); return;
        }
    }
    double sq_affect_radius = affect_radius*affect_radius;

    //! initialize neighbors of particles
    //for(i=0; i<len; i++){
    //    (io_particles)[i]->neighbors().Clear();
    //}
    io_particles.ClearNeighbors();
    //u32 len = io_particles.length()-1;

    //for(i=0; i<len; i++){
    //    pi = (io_particles)[i];
    //    for(j=i+1; j<io_particles.length(); j++){
    //        pj = (io_particles)[j];
    //        if(pi->GetDistanceSquare(*pj) < sq_affect_radius){
    //            pi->neighbors().Append(pj);
    //            pj->neighbors().Append(pi);
    //        }
    //    }
    //}
    //wsp::Vector<wsp::PairIndex> *list;

    #ifdef _OPENMP
    #pragma omp parallel
    #endif
    {
        int i, j;
        int *label;
        wsp::Vector<wsp::dyn::PairLabel> *list;
        wsp::dyn::Particle *pi, *pj;
        if(label_list!=NULL){
            list = label_list;
            int list_len = static_cast<int>(list->length());
            for(i=0; i<list_len; i++){
            //for(i=0; i<list->length(); i++){
                //label = &(*list)[i].label[0];
                //pi = (io_particles)[label[0]];
                //pj = (io_particles)[label[1]];

                pi = (io_particles)[(*list)[i].label[0]];
                pj = (io_particles)[(*list)[i].label[1]];
                if(pi->GetDistanceSquare(*pj) < sq_affect_radius){
                    pi->neighbors().Append(pj);
                    pj->neighbors().Append(pi);
                }

            }
        }else{ //! make pair list if label_list is not given
            u32 index;
            list = WSP_NEW wsp::Vector<wsp::dyn::PairLabel>((io_particles.length()-1)*io_particles.length()/2);
            //list = WSP_NEW wsp::Vector<wsp::PairIndex>((io_particles.length()-1)*io_particles.length()/2);

            //for(i=0, index=0; i<len; i++){
            //    for(j=i+1; j<io_particles.length(); j++, index++){
            //        //(*list)[index].label[0] = i;
            //        //(*list)[index].label[1] = j;
            //        (*list)[index].index1 = i;
            //        (*list)[index].index2 = j;
            //    }
            //}
            //for(i=0; i<list->length(); i++){
            //    //pi = (io_particles)[(*list)[i].label[0]];
            //    //pj = (io_particles)[(*list)[i].label[1]];
            //    pi = (io_particles)[(*list)[i].index1];
            //    pj = (io_particles)[(*list)[i].index2];
            //    if(pi->GetDistanceSquare(*pj) < sq_affect_radius){
            //        pi->neighbors().Append(pj);
            //        pj->neighbors().Append(pi);
            //    }
            //}
            //#ifdef _OPENMP
            //#pragma omp for private(i)
            //#endif
            //for(i=0; i<len; i++){
            //    (io_particles)[i]->neighbors().SetLength(len);
            //    neighbor_cnt[i]=0;
            //}
            #ifdef _OPENMP
            #pragma omp for
            #endif
            for(i=0; i<len; i++){
                //pi = (io_particles)[i];
                for(j=i+1; j<len; j++){
                    //pj = (io_particles)[j];
                    if((io_particles)[i]->GetDistanceSquare(*(io_particles)[j]) < sq_affect_radius){
                        #ifdef _OPENMP
                        #pragma omp critical
                        #endif
                        {
                            (io_particles)[i]->neighbors().Append((io_particles)[j]);
                            (io_particles)[j]->neighbors().Append((io_particles)[i]);
                            //pi->neighbors().Append(pj);
                            //pj->neighbors().Append(pi);
                            //(io_particles)[i]->neighbors()[neighbor_cnt[i]] = (io_particles)[j];
                            //neighbor_cnt[i]++;
                            //(io_particles)[j]->neighbors()[neighbor_cnt[j]] = (io_particles)[i];
                            //neighbor_cnt[j]++;
                        }
                    }
                }
            }
            //#ifdef _OPENMP
            //#pragma omp barrier
            //#pragma omp for private(i)
            //#endif
            //for(i=0; i<len; i++){
            //    (io_particles)[i]->neighbors().SetLength(neighbor_cnt[i]);
            //}
        }
        if(list!=label_list){
            delete list;
        }
    }
}
//! TO DO
//! use old linked list for acceleration
void wsp::dyn::SearchNeighbor_LinkedListSearch(
    wsp::dyn::ParticleList &particle_list, 
    wsp::dyn::ParticleLinkedList &linked_list, 
    double affect_radius
)
{
    double grid_spacing = linked_list.cell_size()[0]*2;
    double sq_affect_radius = affect_radius*affect_radius;
    int len = static_cast<int>(particle_list.length());

    //! initialize neighbors of particles
    //for(i=0; i<particle_list.length(); i++){
    //    (particle_list)[i]->neighbors().Clear();
    //}
    linked_list.Clear();
    particle_list.ClearNeighbors();

    //! assign particles in linked list by position
    #ifdef _OPENMP
    //#pragma omp parallel
    #endif
    {
        double sq_dist;
        int i, j, k, l, cell;
        int x, y, z;
        int indices[3], cell_index, index;
        int list_len;

        #ifdef _OPENMP
        #pragma omp for
        #endif
        for(i=0; i<len; i++)
        {
            wsp::Point<double> pos = particle_list[i]->position();
            //WSP_DYNAMICS_DEBUG_LOG("%d: %f %f %f\n", i, pos[0], pos[1], pos[2]);
            indices[0] = static_cast<int>(floor(pos[0]/grid_spacing));
            indices[1] = static_cast<int>(floor(pos[1]/grid_spacing));
            indices[2] = static_cast<int>(floor(pos[2]/grid_spacing));

            wsp::Vector3<u32> num_cell = linked_list.num_of_cell();
            cell_index = indices[2]*num_cell[0]*num_cell[1] + indices[1]*num_cell[0] + indices[0];
            if(cell_index > static_cast<int>(linked_list.length())){
                //WSP_COMMON_WARNING_LOG("linked list index exceeds max size: max=%d, index=%d\n", linked_list.length(), index); 
                continue;
                //WSP_COMMON_ERROR_LOG("linked list index exceeds max size: max=%d, index=%d\n", linked_list.length(), index); 
                //particle_list.ClearNeighbors();
                //return;
            }
            #ifdef _OPENMP
            #pragma omp critical
            #endif
            {
                linked_list[cell_index].Append(particle_list[i]);
            }

            //! append to neighbor for each cell-contained particles, excepting myself
            int cell_indices[27];
            memset(cell_indices, -1, 27*sizeof(int));
            //! creating labels for iteration
            for(z = indices[2]-1, index=0; z<=indices[2]+1; z++){
                if(z<0 || z>=num_cell[2]){ continue; }
                for(y=indices[1]-1; y<=indices[1]+1; y++){
                    if(y<0 || y>=num_cell[1]){ continue; }
                    for(x=indices[0]-1; x<=indices[0]+1; x++, index++){
                        if(x<0 || x>=num_cell[0]){ continue; }
                        cell_indices[index] = z*num_cell[0]*num_cell[1] + y*num_cell[0] + x;
                    }
                }
            }
            //WSP_DYNAMICS_DEBUG_LOG("index length: %d\n", index);

            //! iterate arrounded cells to find neighbors

            for(index=0; index<27; index++){
                if(cell_indices[index]<0){ continue; }
                list_len = linked_list[cell_indices[index]].length();
                #ifdef _OPENMP
                #pragma omp critical
                #endif
                {
                    for(j=0; j<list_len; j++){
                        sq_dist = particle_list[i]->GetDistanceSquare(*linked_list[cell_indices[index]][j]);
                        if(sq_dist >= sq_affect_radius || sq_dist==0){ continue; }
                        linked_list[cell_indices[index]][j]->neighbors().Append(particle_list[i]);
                        particle_list[i]->neighbors().Append(linked_list[cell_indices[index]][j]);
                    }
                }
            }


            //len = linked_list[cell_index].length();
            //for(j=0; j<len; j++){
            //    sq_dist = particle_list[i]->GetDistanceSquare(*linked_list[cell_index][j]);
            //    if(sq_dist >= sq_affect_radius || sq_dist==0){ continue; }
            //    linked_list[cell_index][j]->neighbors().Append(particle_list[i]);
            //    particle_list[i]->neighbors().Append(linked_list[cell_index][j]);
            //}
        }
    }
}
void TreeSearchRecursive(wsp::dyn::ParticleList &particle_list, wsp::Vector3<double> &size)
{
    int i, j;
    int id;
    int tmp_id, dimension=2, num_of_split=4;
    int num_particle = static_cast<int>(particle_list.length());
    wsp::dyn::Particle *pi;
    wsp::dyn::ParticleList split_list[8];
    wsp::Point<double> pos;

    for(i=0; i<num_particle; i++){
        pi = particle_list[i];
        pos = pi->position();
        id = 0;
        for(j=0; j<dimension; j++){
            tmp_id = pos[j]>=size[j]/2.0 && pos[j]<size[j]? 1 : 0;
            id |= tmp_id<<j;
        }
        split_list[id].Append(pi);
    }
    for(i=0; i<num_of_split; i++){
        if(split_list[i].length()>1){
            //TreeSearchRecursive(split_list[i], wsp::Vector3<double>(size.operator/((double)2.0)));
        }
    }
}
void SearchNeighbor_TreeSearch(wsp::dyn::ParticleList &particle_list){
    particle_list.ClearNeighbors();
}


//! Internal Force Computation Algorithms --------------------------------------
void wsp::dyn::ComputeLennardJonesForce(wsp::dyn::ParticleList &particle_list, 
    double particle_radius,
    double min_replusion_radius_ratio, 
    double epsilon)
{
    double dx, dy, dist, thre_rad, thre_replusion, norm_coef, eps, sigma, force;
    u32 i, j;
    wsp::dyn::Particle *pi, *pj;

    thre_rad = particle_radius;
    sigma = thre_rad + particle_radius * min_replusion_radius_ratio;

    u32 len = particle_list.length();
    u32 nlen;
    for(i=0; i<len; i++){
        pi = particle_list[i];
        pi->force()[0] = 0.0;
        pi->force()[1] = 0.0;
        nlen = pi->neighbors().length();
        for(j=0; j<nlen; j++){
            pj = pi->neighbors()[j];

            dx = pi->position()[0] - pj->position()[0];
            dy = pi->position()[1] - pj->position()[1];
            dist = sqrt(dx*dx + dy*dy);
            if(dist<thre_rad){ dist=thre_rad; }
            force = WSP_GetLennardJonesForce(dist, epsilon, sigma);

            //! Normalize vector between 2 particles
            //! (n*x)^2+(n*y)^2 = 1
            //! n^2*(x^2+y^2) = 1
            //! n^2 = 1/(x^2+y^2)
            //! n = 1/distance
            norm_coef = 1.0/dist;
            force *= norm_coef;

            pi->force()[0] += dx * force;
            pi->force()[1] += dy * force;
        }
    }
}


wsp::State wsp::dyn::GetSPHKernel(double &o_w, double *o_dwdx, double r, double *dx, double hsml, u32 dimension, wsp::dyn::SPHKernelType kernel_type){
    if(dimension>3 || dimension==0){
        WSP_COMMON_ERROR_LOG("dimension must be from 1 to 3\n"); return WSP_STATE_FAILURE;
    }
    int d;
    double q,factor,weight;

    q = r/hsml;
    weight = 0.0;
    if(o_dwdx!=NULL){
        for(d=0; d<dimension; d++){
            o_dwdx[d] = 0.0;
        }
    }

    switch(kernel_type){
    case wsp::dyn::SPH_KERNEL_CUBIC_SPLINE:
        //! cubic spline kernel
        switch(dimension){
        case 1: factor = 1.0/hsml; break;
        case 2: factor = 15.0/(7.0*wsp::math::kPI*hsml*hsml); break;
        case 3: factor = 3.0/(2.0*wsp::math::kPI*hsml*hsml*hsml); break;
        default: WSP_COMMON_ERROR_LOG("wrong dimension: DIM = %d", dimension); return WSP_STATE_FAILURE;
        }

        if(q>=0.0 && q<=1.0){
            weight = factor * (2.0/3.0 -q*q + q*q*q/2.0);
            if(o_dwdx!=NULL){
                for(d=0; d<dimension; d++){
                    o_dwdx[d] = factor * (-2.0 + 3.0/2.0*q)/(hsml*hsml) * dx[d];
                }
            }
        }else if(q>1.0 && q<=2.0){
            weight = factor * 1.0/6.0*(2.0 - q)*(2.0 - q)*(2.0 - q);
            if(o_dwdx!=NULL){
                for(d=0; d<dimension; d++){
                    o_dwdx[d] = -factor * 1.0/2.0*(2.0 - q)*(2.0 - q)/hsml * (dx[d]/r);
                }
            }
        }else{
            weight = 0.0;
            if(o_dwdx!=NULL){
                for(d=0; d<dimension; d++){
                    o_dwdx[d] = 0.0;
                }
            }
        }

        o_w = weight;
        break;
    case wsp::dyn::SPH_KERNEL_GAUSS:
        //! Gauss kernel
        break;
    case wsp::dyn::SPH_KERNEL_QUINTIC:
        //! Qu32ic kernel
        break;
    default:
        WSP_COMMON_ERROR_LOG("Unknown kernel type\n"); return WSP_STATE_FAILURE;
    }

    return WSP_STATE_SUCCESS;
}


void wsp::dyn::ComputeSPHForce(wsp::dyn::ParticleList &particle_list, 
    double affect_radius, 
    double min_density, 
    double particle_radius, 
    double pressure_coef, 
    double viscosity_coef)
{
    //! computation of pressure for each particles
    int len = static_cast<int>(particle_list.length());
    //#ifdef _OPENMP
    //#pragma omp parallel
    //#endif
    {
        double dist, weight, pressure_weight, viscosity_weight;
        double dx[2];
        int i, j;
        int nlen;
        wsp::dyn::Particle *pi, *pj;
        //#ifdef _OPENMP
        //#pragma omp for
        //#endif
        for(i=0; i<len; i++){
            pi = particle_list[i];
            pi->set_density(0.0);
            //WSP_DYNAMICS_DEBUG_LOG("neighbors: %d\n",pi->neighbors().length() );
            nlen = static_cast<int>(pi->neighbors().length());
            for(j=0; j<nlen; j++){
                pj = pi->neighbors()[j];
                dist = pi->GetDistance(*pj);
                //#ifdef _OPENMP
                //#pragma omp critical
                //#endif
                {
                    pi->density() += (1 - dist/affect_radius)*(1 - dist/affect_radius);
                }
            }
            if(pi->density() < min_density){
                pi->set_density(min_density);
            }
            pi->set_pressure( pi->density() - min_density);
        }
        //WSP_DYNAMICS_DEBUG_LOG("pressure: %f\n", pi->pressure());

        //! computation of force for each particles
        //#ifdef _OPENMP
        //#pragma omp for
        //#endif
        for(i=0; i<len; i++){
            pi = particle_list[i];
            pi->force()[0] = 0.0;
            pi->force()[1] = 0.0;
            nlen = pi->neighbors().length();
            for(j=0; j<nlen; j++){
                pj = pi->neighbors()[j];

                //! calculate weight from distance with neighbors ---------
                dx[0] = pi->position()[0] - pj->position()[0];
                dx[1] = pi->position()[1] - pj->position()[1];
                dist = sqrt(dx[0]*dx[0] + dx[1]*dx[1]);
                //wsp::dyn::GetSPHKernel(weight, dx, dist, dx, affect_radius, 2U);
                weight = 1 - dist/affect_radius;
                // -------------------------------------------------------

                pressure_weight = ((pi->pressure()+pj->pressure())/(2*pj->density())) * pressure_coef * weight;
                //WSP_DYNAMICS_DEBUG_LOG("pressureWeight: %f\n", pressureWeight);

                dist = 1/dist;
                dx[0]*=dist;
                dx[1]*=dist;
                pi->force()[0] += dx[0] * pressure_weight;
                pi->force()[1] += dx[1] * pressure_weight;
                viscosity_weight = (weight / pj->density()) * viscosity_coef;

                dx[0] = pi->velocity()[0] - pj->velocity()[0];
                dx[1] = pi->velocity()[1] - pj->velocity()[1];
                pi->force()[0] -= dx[0] * viscosity_weight;
                pi->force()[1] -= dx[1] * viscosity_weight;

            }
            //WSP_DYNAMICS_DEBUG_LOG("force: %f %f\n", pi->force()[0], pi->force()[1]);
        }
    }
}