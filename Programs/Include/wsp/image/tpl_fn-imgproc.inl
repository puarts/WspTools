/**
 * @file tpl_fn-imgproc.inl
 * 
 */

#ifndef WspImage_WspFnImgProc_tpl_INL_
#define WspImage_WspFnImgProc_tpl_INL_

//#include <omp.h>
#include <vector>
#include <cassert>

#include "_tpl_fn-imgproc.hpp"

#include "tpl_fn-imgproc.hpp"


namespace {
    struct PointObj{
        float x, y;
        PointObj(float ex, float why){
            x = ex;
            y = why;
        }
    };
#ifdef USE_QT
    class Branch
    {
    public:
        Branch(
            int lev, 
            std::vector<PointObj*>* points, 
            const struct wsp::img::SutcliffePolygonParam &param
        )
            : max_levels_  (param.max_levels)
            , strut_factor_(param.strut_factor)
            , my_branches_(WSP_NEW std::vector<Branch*> )
            , outer_points_(WSP_NEW std::vector<PointObj*>() )
        {
            level_ = lev;
            //num = n;
            *outer_points_ = *points;
            mid_points_   = calcMidPoints();
            proj_points_  = calcStrutPoints();
            if((level_+1) < max_levels_){
                Branch* childBranch = WSP_NEW Branch(level_+1, proj_points_, param);
                my_branches_->push_back( childBranch );

                for(int k=0; k<outer_points_->size(); ++k){
                    int nextk = k - 1;
                    if(nextk < 0){
                        nextk += outer_points_->size(); 
                    }
                    std::vector<PointObj*> newPoints;
                    newPoints.push_back( (*proj_points_) [k] );
                    newPoints.push_back( (*mid_points_)  [k] );
                    newPoints.push_back( (*outer_points_)[k] );
                    newPoints.push_back( (*mid_points_)  [nextk] );
                    newPoints.push_back( (*proj_points_) [nextk] );
                    childBranch = WSP_NEW Branch( level_+1, &newPoints, param);
                    my_branches_->push_back( childBranch );
                }
            }
        }
        ~Branch()
        {
            delete outer_points_;
            {
                int num_elems = mid_points_->size();
                for( int i=0; i<num_elems; ++i )
                {
                    delete (*mid_points_)[i];
                }
                delete mid_points_;
            }
            {
                int num_elems = proj_points_->size();
                for( int i=0; i<num_elems; ++i )
                {
                    delete (*proj_points_)[i];
                }
                delete proj_points_;
            }
            {
                int num_elems = my_branches_->size();
                for( int i=0; i<num_elems; ++i )
                {
                    delete (*my_branches_)[i];
                }
                delete my_branches_;
            }
        }

        void drawMe( QPainter &painter )
        {
            int point_size_ = 3;
            {
                QPen pen;
                QBrush brush;

                pen.setWidthF( max_levels_-level_ );
                painter.setPen( pen );

                for( int i=0; i<outer_points_->size(); ++i )
                {
                    int nexti = i+1;
                    if(nexti == outer_points_->size()){ nexti = 0; }
                    painter.drawLine((*outer_points_)[i]->x, (*outer_points_)[i]->y,
                        (*outer_points_)[nexti]->x, (*outer_points_)[nexti]->y);
                }
            }

            {
                QPen pen;
                QBrush brush;

                pen.setWidthF( 0.5f );
                brush.setColor( QColor( 255, 255, 255, 150 ) );
                painter.setPen( pen );
                painter.setBrush( brush );
                for(int j=0; j<mid_points_->size(); ++j)
                {
                    painter.drawEllipse( (*mid_points_)[j]->x, (*mid_points_)[j]->y, point_size_, point_size_ );
                }

                for(int j=0; j<mid_points_->size(); ++j)
                {
                    painter.drawEllipse((*mid_points_)[j]->x, (*mid_points_)[j]->y, point_size_, point_size_);
                    painter.drawLine((*mid_points_)[j]->x, (*mid_points_)[j]->y, (*proj_points_)[j]->x, (*proj_points_)[j]->y);
                    painter.drawEllipse((*proj_points_)[j]->x, (*proj_points_)[j]->y, point_size_, point_size_);
                }
            }

            for( int k=0; k<my_branches_->size(); ++k )
            {
                (*my_branches_)[k]->drawMe( painter );
            }
        }

        std::vector<PointObj*>* calcMidPoints()
        {
            std::vector<PointObj*>* mpArray = WSP_NEW std::vector<PointObj*>();
            mpArray->resize( outer_points_->size() );
            for( int i=0; i<outer_points_->size(); ++i )
            {
                int nexti = i + 1;
                if ( nexti == outer_points_->size() ){ nexti = 0; }
                PointObj* thisMP = calcMidPoint(*(*outer_points_)[i], *(*outer_points_)[nexti]);
                (*mpArray)[i] = thisMP;
            }
            return mpArray;
        }

        PointObj* calcMidPoint(PointObj &end1, PointObj &end2)
        {
            float mx, my;
            if( end1.x > end2.x )
            {
                mx = end2.x + ((end1.x - end2.x)/2);
            }
            else
            {
                mx = end1.x + ((end2.x - end1.x)/2);
            }
            if( end1.y > end2.y )
            {
                my = end2.y + ((end1.y - end2.y)/2);
            }
            else
            {
                my = end1.y + ((end2.y - end1.y)/2);
            }
            return WSP_NEW PointObj(mx, my);
        }

        std::vector<PointObj*>* calcStrutPoints(){
            std::vector<PointObj*>* strutArray = WSP_NEW std::vector<PointObj*>();
            strutArray->resize( mid_points_->size() );
            for( int i=0; i<mid_points_->size(); ++i)
            {
                int nexti = i+3;
                if( nexti >= mid_points_->size()){ nexti -= mid_points_->size(); }
                //(*strutArray)[i] = calcProjPoint( *(*mid_points_)[i], *(*outer_points_)[i] );
                (*strutArray)[i] = calcProjPoint( *(*mid_points_)[i], *(*outer_points_)[nexti] );
            }
            return strutArray;
        }

        PointObj* calcProjPoint(PointObj &mid_pt, PointObj &out_pt)
        {
            float px, py;

            //float a0 = (out_pt.y-mid_pt.y)/(float)(out_pt.x-mid_pt.x);
            //float a1 = -1.0f/a0;
            //float b  = mid_pt.y - a1*mid_pt.x;
            //px = mid_pt.x + strut_factor_// * (out_pt.x-mid_pt.x)/fabs(out_pt.x-mid_pt.x)
            //    *(out_pt.y-mid_pt.y)/fabs(out_pt.y-mid_pt.y);
            //py = px * a1 + b;

            //float vx0 = out_pt.x-mid_pt.x;
            //float vy0 = out_pt.y-mid_pt.y;
            //float vx1, vy1;
            //0 = vx0*vx1 + vy0*vy1

            /* calculation of triangle method */
            float adj, opp;
            if(out_pt.x > mid_pt.x){
                opp = out_pt.x - mid_pt.x;
            }else{
                opp = mid_pt.x - out_pt.x;
            }
            if(out_pt.y>mid_pt.y){
                adj = out_pt.y - mid_pt.y;
            }else{
                adj = mid_pt.y - out_pt.y;
            }


            if(out_pt.x>mid_pt.x){
                px = mid_pt.x + (opp * strut_factor_);
            }else{
                px = mid_pt.x - (opp * strut_factor_);
            }
            if(out_pt.y > mid_pt.y){
                py = mid_pt.y + (adj * strut_factor_);
            }else{
                py = mid_pt.y - (adj * strut_factor_);
            }
            return WSP_NEW PointObj(px, py);
        }

    private:
        u32 level_;
        u32 num_;
        std::vector<PointObj*>* outer_points_;
        std::vector<PointObj*>* mid_points_  ;
        std::vector<PointObj*>* proj_points_ ;
        std::vector<Branch*>*   my_branches_ ;
        float strut_factor_;
        u32   max_levels_;

    };


    float radians( float rot )
    {
        return rot*M_PI/180.0f;
    }

    class FractalRoot{
    public:
        FractalRoot(
                float startAngle, 
                float centX, float centY, 
                const struct wsp::img::SutcliffePolygonParam &param 
        )
            : point_size_  (param.point_size)
            , num_sides_   (param.num_sides)
            , point_array_(WSP_NEW std::vector<PointObj*>() )
        {
            float angleStep = 360.0f/num_sides_;
            for( int i=0; i<360; i+=angleStep )
            {
                float x = centX + (400*cos(radians(startAngle+i)));
                float y = centY + (400*sin(radians(startAngle+i)));
                point_array_->push_back(WSP_NEW PointObj(x, y) );
            }
            root_branch_ = WSP_NEW Branch( 0, point_array_, param );
        }
        ~FractalRoot()
        {
            int num_elems = point_array_->size();
            for( int i=0; i<num_elems; ++i )
            {
                delete (*point_array_)[i];
            }
            delete point_array_;

            delete root_branch_;
        }


        void drawShape( QPainter &painter )
        {
            root_branch_->drawMe( painter );
        }

    private:
        std::vector<PointObj*>* point_array_;
        Branch* root_branch_;

        int   point_size_;
        int   num_sides_;
    };
#endif
}




#include <wsp/math/cl-perlin_noise.h>
#ifdef USE_QT
template<typename T>
void wsp::img::DrawSutcliffePolygons( 
    wsp::Image<T> &draw_image, 
    const struct wsp::img::SutcliffePolygonParam &param 
)
{
    int width = draw_image.width();
    int height = draw_image.height();
    int cx = width/2;
    int cy = height/2;

    QImage qimage;
    wsp::img::qtWspImageToQImage( qimage, NULL, draw_image );

    /* draw */
    QPainter painter( &qimage );

    /* set brush */
    {
        QBrush brush;
        brush.setColor( QColor( 255, 255, 255, 200 ) );
        painter.setBrush( brush );
    }

    /* set pen */
    {
        QPen pen;
        pen.setColor( QColor( 0, 0, 0, 50 ) );
        painter.setPen( pen );
    }

    painter.setCompositionMode( QPainter::CompositionMode_SourceOver );

    //wsp::PerlinNoise pn( time(NULL) );
    //float strutNoise = (rand()/(float)RAND_MAX)*10;
    //strutNoise +=0.01;
    //float strutFactor = pn.noise(strutNoise, 0, 0)*3 - 1;
    int frameCount = 0;

    FractalRoot pentagon(frameCount, cx, cy, param);
    pentagon.drawShape( painter );
    

    wsp::img::qtQImageToWspImage( draw_image, qimage );
}
#endif


template<typename T, typename U> 
inline wsp::State wsp::img::GetCharacteristicMask(wsp::Image<T> &o_mask, const wsp::Image<U> &in_src, wsp::img::CharacteristicMaskParam<T> &params)
{
    wsp::Image<u32> block_size_map(in_src.width(), in_src.height(), 1);
    block_size_map.Reset(params.block_size);
    //printf("GetCharacteristicMask wrapper: zero=%d, one=%d\n", params.mask_zero_value, params.mask_one_value);
    return wsp::img::GetCharacteristicMask(o_mask, in_src, block_size_map, params.threshold_of_block_average, params.mask_one_value, params.mask_zero_value, params.eval_func);
}


template<typename T, typename U> 
inline wsp::State wsp::img::GetCharacteristicMask(wsp::Image<T> &o_mask, const wsp::Image<U> &in_src, const wsp::Image<u32> &in_block_size_map,
        double threshold_of_block_average, T one_value, T zero_value, double (*eval_func)(double, double))
{
    u32 w=static_cast<int>(in_src.width()), h=static_cast<int>(in_src.height()), nc=static_cast<int>(in_src.num_of_channels());
    if(w!=in_block_size_map.width() || h!=in_block_size_map.height()){
        WSP_COMMON_ERROR_LOG("The sizes of in_src and in_block_size_map must be the same\n"); return WSP_STATE_FAILURE;
    }
    o_mask.SetSize(w, h, 1);
    return wsp::img::GetCharacteristicMask(o_mask.data(), in_src.data(), in_block_size_map.data(), in_src.width(), in_src.height(), in_src.num_of_channels(),
                                            threshold_of_block_average, one_value, zero_value, eval_func);
}

template<typename T, typename U> 
inline wsp::State wsp::img::GetCharacteristicMask(T *o_mask, const U *in_src, u32 width, u32 height, u32 num_of_channels, wsp::img::CharacteristicMaskParam<T> &params)
{
    u32 len = width*height;
    u32 *block_size_map = WSP_NEW u32[len];
    for(u32 i=0; i<len; i++){
        block_size_map[i] = params.block_size;
    }
    wsp::State state = wsp::img::GetCharacteristicMask(o_mask, in_src, block_size_map, width, height, num_of_channels, 
                        params.threshold_of_block_average, params.mask_one_value, params.mask_zero_value, params.eval_func);
    delete[] block_size_map;
    return state;
}


//! mask must have only 1 channel, in opposite, source can have any number of channels
template<typename T, typename U> 
wsp::State wsp::img::GetCharacteristicMask(T *o_mask, const U *in_src, const u32 *in_block_size_map, u32 width, u32 height, u32 num_of_channels,
        double threshold_of_block_average, T one_value, T zero_value, double (*eval_func)(double, double))
{
    int x, y, i, j, c;
    int index, cnt;
    int w_size;
    int w=static_cast<int>(width), h=static_cast<int>(height), nc=static_cast<int>(num_of_channels);
    int len = w * h;
    T *tmp_mask = WSP_NEW T[len];
    double *avs = WSP_NEW double[nc];
    double *vars= WSP_NEW double[nc];
    double var_av;    

    printf("GetCharacteristicMask: zero=%d, one=%d, thre=%f\n", zero_value, one_value, threshold_of_block_average);

    for(y=0; y<h; y++){
        //yw = y*w;
        for(x=0; x<w; x++){
            //index = yw+x;
            w_size = in_block_size_map[y*w + x];

            //! initialize sum
            for(c=0; c<nc; c++){
                avs[c] = 0.0;
                vars[c] = 0.0;
            }
            cnt=0;
            //! get average within block
            for(i=-w_size; i<=w_size; i++){
                for(j=-w_size; j<=w_size; j++){
                    if(y+i<0 || y+i>=h || x+j<0 || x+j>=w){ continue; }

                    index = ((y+i)*w + (x+j)) * nc;
                    for(c=0; c<nc; c++){
                        avs[c] += static_cast<double>(in_src[index+c]);
                    }
                    cnt++;
                }
            }

            for(c=0; c<nc; c++){ avs[c] /= static_cast<double>(cnt); }

            cnt=0;
            //! get variance
            for(i=-w_size; i<=w_size; i++){
                for(j=-w_size; j<=w_size; j++){
                    if(y+i<0 || y+i>=h || x+j<0 || x+j>=w){ continue; }

                    index = ((y+i)*w + (x+j)) * nc;
                    for(c=0; c<nc; c++){
                        //vars[c] += ((double)in_src[index+c] - avs[c])*((double)in_src[index+c] - avs[c]);
                        vars[c] += eval_func(static_cast<double>(in_src[index+c]), static_cast<double>(avs[c]));
                    }
                    cnt++;
                }
            }

            //! get sum of average of each channels
            var_av = 0.0;
            for(c=0; c<nc; c++){ 
                vars[c] /= static_cast<double>(cnt); 
                var_av += vars[c];
                //printf("%f\t", vars[c]);
            }
            //printf("%f\t", var_av);
            var_av /= static_cast<double>(nc);
            //printf("%f\n", var_av);


            if(var_av > threshold_of_block_average){
                tmp_mask[y*w + x] = one_value;
            }else{
                tmp_mask[y*w + x] = zero_value;
            }
        }
    }
    for(i=0; i<len; i++){
        o_mask[i] = tmp_mask[i];
    }

    delete[] tmp_mask;
    delete[] avs;
    delete[] vars;

    return WSP_STATE_SUCCESS;
}



// ----------------------------------------------------------------
template<typename T>
wsp::State wsp::img::RemoveHighVariancePixels(
    wsp::Image<T> &io_img, 
    const wsp::Image<T> &in_av_img, 
    double threshold_sd
)
{
    double sd, var, var_sum, diff;
    u32 w=io_img.width(), h=io_img.height(), nc=io_img.num_of_channels();
    u32 i, c;
    if(w!=in_av_img.width() || h!=in_av_img.height()){
        WSP_COMMON_ERROR_LOG("The sizes of main image and average image are different\n"); return WSP_STATE_FAILURE;
    }
    printf("RemoveHighVariancePixels: threshold of std deviation=%f\n", threshold_sd);
    for(i=0; i<io_img.length(); i++)
    {
        var_sum = 0.0;
        for(c=0; c<nc; c++)
        {
            diff = static_cast<double>(io_img[i*nc+c]) - static_cast<double>(in_av_img[i*nc+c]);
            var_sum += diff * diff;
        }
        var = var_sum/static_cast<double>(nc);
        sd = sqrt(var);
        //printf("sd=%f\n", sd);
        if(sd>=threshold_sd){
            io_img.SetAlpha(0, i);
            io_img.SetState(WSP_PIXEL_STATE_OCCLUDED, i);
            //printf("%d is High var\n", i);
        }
    }
    return WSP_STATE_SUCCESS;
}

//template<typename T>
//void wsp::img::CutOff(T *io_im, u32 width, u32 height, u32 numOfChannels, T min, T max, T fill_min, T fill_max){
//    T d;
//    u32 i, c, index, len=width*height;
//    if(min>=max){ return; }
//    for(i=0; i<len; i++){
//        index = i*numOfChannels;
//        for(c=0; c<numOfChannels; c++){
//            d = io_im[index+c];
//            if(d<=min){ io_im[index]=fill_min; }
//            else if(d>=max){ io_im[index]=fill_max; }
//        }
//        //d = io_im[index];
//    }
//}

template<typename T>
inline void wsp::img::Binalize(wsp::Image<T> &io_img, T threshold, T min, T max)
{
    //wsp::img::CutOff<T>(io_img.data(), io_img.width(), io_img.height(), io_img.num_of_channels(), threshold, threshold, min, max);
    wsp::CutOff<T>(io_img.data(), io_img.length()*io_img.num_of_channels(), threshold, min, threshold, max);
}

template<typename T> 
inline void wsp::img::BinalizeAlpha(wsp::Image<T> &io_img, uchar threshold)
{
    wsp::CutOff<uchar>(io_img.alpha_data(), io_img.length(), threshold, 0, threshold, 255);
}


template<typename _InType, typename _OutType> 
inline wsp::State wsp::img::GetIntensityMap(
    const wsp::Image<_InType> &in_rgb, 
    wsp::Image<_OutType> &o_intensity
)
{
    s32 width   = (s32)in_rgb.width();
    s32 height  = (s32)in_rgb.height();
    s32 nc      = (s32)in_rgb.num_of_channels();
    s32 num_pix = width * height;

    if ( nc == 1 )
    {
        o_intensity.CopyFrom( in_rgb );
        return WSP_STATE_SUCCESS;
    }

    o_intensity.SetSize( width, height, 1 );

    {
        double intensity;
        const _InType *end_c_ptr;

        const _InType *src_ptr = in_rgb.data();
        _OutType *dst_ptr = o_intensity.data();        

        for( _OutType *end_ptr=dst_ptr+num_pix; dst_ptr!=end_ptr; ++dst_ptr )
        {
            intensity = 0;
            /* Assuming byte order as big-endian */
            for( end_c_ptr=src_ptr+nc; src_ptr!=end_c_ptr; ++src_ptr )
            {
                intensity += (double)(*src_ptr);
            }
            intensity /= nc;
            *dst_ptr = (_OutType)( intensity );
        }
    }

    return WSP_STATE_SUCCESS;
}

template<typename _InType, typename _OutType>  
inline wsp::State wsp::img::GetChromaMap(
    const wsp::Image<_InType> &in_rgb, 
    wsp::Image<_OutType> &o_chroma_rgby,
    double plune_intensity_ratio
)
{
    wsp::Image<double> intensity_map;
    s32 width   = (s32)in_rgb.width();
    s32 height  = (s32)in_rgb.height();
    s32 src_nc      = (s32)in_rgb.num_of_channels();
    s32 num_pix = width * height;

    if ( src_nc < 3 )
    {
        WSP_COMMON_ERROR_LOG("Number of channels must be larger than 3\n");
        return WSP_STATE_INVALID_PARAMETER;
    }

    int dst_nc = 4;
    o_chroma_rgby.SetSize( width, height, dst_nc );

    wsp::img::GetIntensityMap( in_rgb, intensity_map );

    double max_intensity = wsp::GetMax( intensity_map.data(), intensity_map.length() );

    {
        double intensity;
        double *norm_col = WSP_NEW double[src_nc];
        double intensity_ratio;
        double r, g, b;
        double chroma_r, chroma_g, chroma_b, chroma_y;
        _InType *tmp_ptr;
        const _InType *end_c_ptr;
        double *color_ptr;

        const _InType *src_ptr = in_rgb.data();
        _OutType *dst_ptr = o_chroma_rgby.data();
        double *intensity_ptr = intensity_map.data();
        
        _OutType *end_ptr = dst_ptr + num_pix*dst_nc;
        for( ; dst_ptr!=end_ptr; ++intensity_ptr )
        {
            intensity = *intensity_ptr;
            intensity_ratio = intensity / max_intensity;

            if( intensity_ratio < plune_intensity_ratio )
            {
                /* Set 0 value to destination */
                memset( dst_ptr, 0, sizeof(_OutType) * dst_nc );
                dst_ptr += dst_nc;
                src_ptr += src_nc;
                continue;
            }

            color_ptr = norm_col;
            end_c_ptr = src_ptr + src_nc;
            for( ; src_ptr!=end_c_ptr; ++src_ptr, ++color_ptr )
            {
                *color_ptr = (double)(*src_ptr) / intensity;
            }
            r = norm_col[0];
            g = norm_col[1];
            b = norm_col[2];

            chroma_r = r - ( g + b )/2.0;
            chroma_g = g - ( r + b )/2.0;
            chroma_b = b - ( r + g )/2.0;
            chroma_y = ( r + g )/2.0 - fabs( r - g )/2.0 - b;

            /* Store calculation results with pruning threshold 0 */
            if( chroma_r < 0 ){ chroma_r = 0; }
            if( chroma_g < 0 ){ chroma_g = 0; }
            if( chroma_b < 0 ){ chroma_b = 0; }
            if( chroma_y < 0 ){ chroma_y = 0; }
            *dst_ptr = (_OutType)chroma_r;
            ++dst_ptr;

            *dst_ptr = (_OutType)chroma_g;
            ++dst_ptr;

            *dst_ptr = (_OutType)chroma_b;
            ++dst_ptr;

            *dst_ptr = (_OutType)chroma_y;
            ++dst_ptr;
        }
        delete[] norm_col;
    }

    return WSP_STATE_SUCCESS;
}



template<typename T>
wsp::State wsp::img::ShiftPixelsByDepth(wsp::Image<T> &o_dest, const wsp::Image<T> &in_src, const wsp::Image<double> &in_depth)
{
    double d1, init_depth;
    int inverter;
    u32 xd, width, height;
    wsp::Image<double> map_zbuf; //! buffer for depth value shifted to new pixel
    wsp::Image<T> tmp_dest(o_dest);

    if(in_depth.GetValueType()==WSP_DEPTH_VALUE_TYPE_LARGER_VALUE_IS_NEARER){
            inverter=1;
    }else{
            inverter=-1;
    }

    width = in_src.width();
    height = in_src.height();

    if(width != in_depth.width() || height != in_depth.height()){
        WSP_COMMON_ERROR_LOG("sizes of color and depth images are different\nwidth: %d %d, height %d %d", 
                width, in_depth.width(), height, in_depth.height());
        return WSP_STATE_FAILURE;
    }

    std::cout<<"shifting pixels.."<<std::endl;

    //! initialize maps -------------------------------------------
    tmp_dest.SetSize(width, height);
    map_zbuf.SetSize(width, height);
    init_depth = DBL_MAX*inverter;
    for(u32 y=0; y<height; y++){
        for(u32 x=0; x<width; x++){
            map_zbuf.SetElem(init_depth, x, y);
            tmp_dest.SetElem((T)0, x, y);
            tmp_dest.SetState(WSP_PIXEL_STATE_OCCLUDED, x, y);
            tmp_dest.SetAlpha((uchar)0, x, y);
        }
    }
    //tmp_dest.SaveAsTiff("dbgcolor.tif");
    //tmp_dest.saveAlphaAsTiff("dbgalpha.tif");
    //tmp_dest.SaveStateAsTiff("dbgstate.tif");
    //cout<<"Pixelshift width height:"<<width<<" "<<height<<endl;

    //! main loop -------------------------------------------------
    //! smaller disparities are nearer
    for(u32 y=0; y<height; y++){
        for(u32 x=0; x<width-1; x++){
            //! skip pixels witch have transparency
            if(in_src.GetAlpha(x, y)<255){
                if(tmp_dest.GetState(x, y)==WSP_PIXEL_STATE_OCCLUDED)
                {
                    tmp_dest.SetElem((T)0, x, y);
                }
                continue;
            }

            d1 = (double)in_depth.GetElem(x, y);

            xd = (u32)(x + d1);

            //! if out of bounds or destination pixel has a more far disparity
            if( in_src.IsOutOfBounds(xd, y) || d1*inverter>map_zbuf.GetElem(xd, y)*inverter ){
                continue;
            }
            //! shift pixel
            tmp_dest.SetElem(in_src.GetElem(x, y), xd, y);
            tmp_dest.SetAlpha(in_src.GetAlpha(x, y), xd, y);
            tmp_dest.SetState(WSP_PIXEL_STATE_NORMAL, xd, y);
            map_zbuf.SetElem(d1, xd, y);
        }
    }
    o_dest=tmp_dest;
    std::cout<<"pixelshift has done.\n";

    return WSP_STATE_SUCCESS;
}
/** @brief o_depth is shifted depth map */
template<typename T>
wsp::State wsp::img::ShiftPixelsByDepth_SubpixelLevel(wsp::Image<T> &o_dest, 
                                                      wsp::Image<double> *o_depth,
                                                      const wsp::Image<T> &in_src,
                                                      const wsp::Image<double> &in_depth,
                                                      T dest_min, T dest_max)
{
    int inverter;
    int width, height, nc;
    wsp::Image<double> map_zbuf; //* buffer for depth value shifted to new pixel
    //WspRgb rgb;
    //wsp::Vector3 rgbd0, rgbd1;
    wsp::Image<double> tmp_dest;
    wsp::Image<double> tmp_depth;

    if(in_depth.GetValueType()==WSP_DEPTH_VALUE_TYPE_LARGER_VALUE_IS_NEARER){
        std::cout<<"Smaller value is brighter"<<std::endl;
        inverter=1;
    }else{
        std::cout<<"Larger value is brighter"<<std::endl;
        inverter=-1;
    }

    width = static_cast<int>(in_src.width());
    height = static_cast<int>(in_src.height());
    nc = static_cast<int>(in_src.num_of_channels());

    if(width != in_depth.width() || height != in_depth.height()){
        WSP_COMMON_ERROR_LOG("sizes of color and depth images are different\nwidth: %d %d, height %d %d", 
            width, in_depth.width(), height, in_depth.height());
        return WSP_STATE_FAILURE;
    }

    printf("Shift Pixels: nc=%d\n", nc);

    // initialize maps -------------------------------------------
    tmp_dest.SetSize(width, height, nc);
    tmp_depth.SetSize(width, height, 1);
    map_zbuf.SetSize(width, height, 1);

    double init_depth = -DBL_MAX * inverter;

    #ifdef _OPENMP
    #pragma omp parallel
    #endif
    {
        double d, xd;
        double xr0, xr1; //! ratio for supixel depth
        double depth0, depth1;
        double ddiff; //! depth difference beween new depth and z-buffer depth
        double dthre = 0.0; //! depth difference threshold for replacing new depth, 0.0 means replace all depth
        u32 alpha0, alpha1;
        int y, x, c;
        int i, i0, i1;
        int xd0, xd1;
        uchar *ptr0, *ptr1;

        #ifdef _OPENMP
        #pragma omp for
        #endif
        for(y=0; y<height; y++){
            for(x=0; x<width; x++){
                i=y*width+x;
                //map_zbuf.SetElem(init_depth, x, y);
                map_zbuf[i]=init_depth;

                for(c=0; c<nc; c++){
                    //tmp_dest.SetElem(0, x, y, c);
                    tmp_dest[i*nc+c] = 0;
                }
                tmp_dest.SetState(WSP_PIXEL_STATE_OCCLUDED, i);
                tmp_dest.SetAlpha((uchar)0, i);

                tmp_depth[i]=0.0;
                //tmp_depth.SetElem(0.0, x, y);
                tmp_depth.SetState(WSP_PIXEL_STATE_OCCLUDED, i);
                tmp_depth.SetAlpha((uchar)0, i);
            }
        }
        //o_dest.SaveAsTiff("dbgcolor.tif");
        //o_dest.saveAlphaAsTiff("dbgalpha.tif");
        //o_dest.SaveStateAsTiff("dbgstate.tif");
        //cout<<"Pixelshift width height:"<<width<<" "<<height<<endl;



        // main loop -------------------------------------------------
        // smaller disparities are nearer
        #ifdef _OPENMP
        #pragma omp for
        #endif
        for(y=0; y<height; ++y){
            for(x=0; x<width-1; ++x){
                i = y*width+x;
                //for(c=0; c<nc; c++){
                //    tmp_dest[i*nc+c] = (double)in_src[i*nc+c]+1000;
                //}

                // skip pixels witch have transparency
                if(in_src.GetAlpha(i)<255){
                    //if(tmp_dest.GetState(i)==WSP_PIXEL_STATE_OCCLUDED){
                    //    for(c=0; c<nc; c++){
                    //        //tmp_dest.SetElem(0, x, y, c);
                    //        tmp_dest[i] = 0;
                    //    }
                    //    //tmp_depth.SetAlpha(in_src.GetAlpha(i), i);
                    //    //tmp_depth.SetState(WSP_PIXEL_STATE_IGNORED, i);
                    //    //tmp_dest.SetAlpha(in_src.GetAlpha(x, y), x, y);
                    //    //tmp_dest.SetState(WSP_PIXEL_STATE_IGNORED, x, y);
                    //}
                    continue;
                }

                d = (double)in_depth[i];//.GetElem(x, y);

                xd = (double)x + d;
                if(inverter==1){
                    xd0 = (int)floor(xd);
                }else{
                    xd0 = (int)ceil(xd);
                }
                xd1 = xd0+inverter;

                //if(tmp_dest.GetState(xd0, y)!=WSP_PIXEL_STATE_OCCLUDED){
                //    continue;
                //}

                i0 = y*width + static_cast<u32>(xd0);
                i1 = y*width + static_cast<u32>(xd1);

                // skip if out of bounds or destination pixel has a more far disparity
                //if( in_src.IsOutOfBounds(xd0, (int)y) || in_src.IsOutOfBounds(xd1, (int)y)
                //    || d*(double)inverter<map_zbuf.GetElem((u32)xd0, y)*(double)inverter 
                //    || d*(double)inverter<map_zbuf.GetElem((u32)xd1, y)*(double)inverter)
                if( in_src.IsOutOfBounds(xd0, (int)y) || in_src.IsOutOfBounds(xd1, (int)y)
                    || d*(double)inverter<map_zbuf[i0]*(double)inverter 
                    || d*(double)inverter<map_zbuf[i1]*(double)inverter)
                {
                    //tmp_dest.SetElem(wsp::Vector3(255, 0,0), xd0, y);
                    continue;
                }

            
                // jugde whether replacing old depth with new depth or not
                //if(abs(d1 - map_zbuf.GetElem((u32)xd0, y))>dthre){
                if(fabs(d - map_zbuf[i0])>dthre){
                    for(c=0; c<nc; c++){
                        tmp_dest[i0*nc + c] = 0;
                        //tmp_dest.SetElem(0, (u32)xd0, y, c);
                    }
                    tmp_dest.SetAlpha(uchar(0), i0);

                    tmp_depth[i0]=0.0;
                    tmp_depth.SetAlpha(uchar(0), i0);
                }
                //if(abs(d - map_zbuf.GetElem((u32)xd1, y))>dthre){
                if(fabs(d - map_zbuf[i1])>dthre){
                    for(c=0; c<nc; ++c){
                        tmp_dest[i1*nc + c] = 0;
                        //tmp_dest.SetElem(0, (u32)xd1, y, c);
                    }
                    tmp_dest.SetAlpha(uchar(0), i1);

                    tmp_depth[i1] = 0.0;
                    tmp_depth.SetAlpha(0, i1);
                    //tmp_depth.SetElem(0.0, i1);
                    //tmp_depth.SetAlpha(uchar(0), i1);
                }

                // shift pixel -------------------
                // calculate ratio of subpixel
                xr1 = fabs(xd-(double)xd0);
                xr0 = 1.0 - xr1;

                //rgb = in_src.GetElem(x, y);
                //rgbd0 = wsp::Vector3(rgb.r, rgb.g, rgb.b) * xr0;
                //rgbd1 = wsp::Vector3(rgb.r, rgb.g, rgb.b) * xr1;
                //tmp_dest.AddElem(rgbd0, (u32)xd0, y);
                //tmp_dest.AddElem(rgbd1, (u32)xd1, y);
                for(c=0; c<nc; ++c){
                    //tmp_dest.AddElem((double)in_src.GetElem(x, y, c)*xr0, (u32)xd0, y, c);
                    //tmp_dest.AddElem((double)in_src.GetElem(x, y, c)*xr1, (u32)xd1, y, c);
                    //tmp_dest[(y*width+(u32)xd0)*nc+c] = (double)in_src[(y*width+x)*nc+c]*xr0;
                    //tmp_dest[(y*width+(u32)xd1)*nc+c] = (double)in_src[(y*width+x)*nc+c]*xr1;
                    tmp_dest[i0*nc+c] += ((double)in_src[i*nc+c])*xr0;
                    tmp_dest[i1*nc+c] += ((double)in_src[i*nc+c])*xr1;
                }

                depth0 = in_depth.GetElem(x, y) * xr0;
                depth1 = in_depth.GetElem(x, y) * xr1;
                tmp_depth[i0]+=depth0;
                tmp_depth[i1]+=depth1;
                //tmp_depth.AddElem(depth0, (u32)xd0, y);
                //tmp_depth.AddElem(depth1, (u32)xd1, y);

                alpha0 = (u32)wsp::math::Round((double)in_src.GetAlpha(x, y) * xr0);
                alpha1 = (u32)wsp::math::Round((double)in_src.GetAlpha(x, y) * xr1);
                tmp_dest.AddAlpha(alpha0, (u32)xd0, y);
                tmp_dest.AddAlpha(alpha1, (u32)xd1, y);
                tmp_depth.AddAlpha(alpha0,(u32)xd0, y);
                tmp_depth.AddAlpha(alpha1,(u32)xd1, y);

                tmp_dest.SetState(WSP_PIXEL_STATE_NORMAL, (u32)xd0, y);
                tmp_dest.SetState(WSP_PIXEL_STATE_NORMAL, (u32)xd1, y);
                tmp_depth.SetState(WSP_PIXEL_STATE_NORMAL, (u32)xd0, y);
                tmp_depth.SetState(WSP_PIXEL_STATE_NORMAL, (u32)xd1, y);

                // update z-buffer
                //map_zbuf.SetElem(d, (u32)xd0, y);
                //map_zbuf.SetElem(d, (u32)xd1, y);
                map_zbuf[i0] = d;
                map_zbuf[i1] = d;
            }
        }
    }
    //cout<<"pixelshift has done.\n";

    //wsp::img::Vector3Img2RgbImg(o_dest, tmp_dest);
    //tmp_dest.Normalize(dest_min, dest_max, 1);
    tmp_dest.CopyIntoAnyImage(o_dest);
    if(o_depth!=NULL){
        *o_depth=tmp_depth;
    }

    return WSP_STATE_SUCCESS;
}
template<typename T> wsp::State wsp::img::NormalizePixels(wsp::Image<T> &io_img, T min, T max)
{
    if(io_img.width()==0 || io_img.height()==0 || min>=max){ WSP_COMMON_ERROR_LOG("args are invalid\n"); return WSP_STATE_FAILURE; }

    T smin=io_img[0], smax=io_img[0];
    int w=(int)io_img.width(), h=(int)io_img.height();
    int len = w*h;
    int i;
    //! find min and max
    for(i=0; i<len; i++){
        if(smin>io_img[i]){ smin = io_img[i]; }
        if(smax<io_img[i]){ smax = io_img[i]; }
    }
    //! -0.0 is for conversion of double
    double range= (smax-smin)-0.0;
    double mult = (double)255.0/range;
    T val;
    for(i=0; i<len; i++){
        val = (io_img[i]-smin) * mult;
        io_img[i] = val;
    }
    return WSP_STATE_SUCCESS;
}

template<typename T> 
inline void wsp::img::FillupSpecifiedStateWithAnyColor(T *io_rgb_data, 
                                                       const uchar *in_state_data, 
                                                       u32 width, u32 height, 
                                                       uchar target_state, 
                                                       uchar fill_r, uchar fill_g, uchar fill_b)
{
    u32 i, len=width*height;
    for(i=0; i<len; i++){
        if(in_state_data[i]==target_state){
            io_rgb_data[i*3] = fill_r;
            io_rgb_data[i*3+1] = fill_g;
            io_rgb_data[i*3+2] = fill_b;
        }
    }
}


template<typename T>
wsp::State wsp::img::ScaleUpByAlpha(wsp::Image<T> &io_img, u32 alphaThreshold, double mult){
    if(io_img.alpha_data()==NULL){ WSP_COMMON_ERROR_LOG("Alpha pointer is NULL\n"); return WSP_STATE_FAILURE; }
    u32 i, c;
    u32 nc=io_img.num_of_channels();
    double alpha, scale;

    for(i=0; i<io_img.length(); i++){
        alpha = (double)io_img.GetAlpha(i);
        if((u32)alpha<=alphaThreshold || alpha==255){ continue; }
        scale = (255.0/alpha)*mult;
        for(c=0; c<nc; c++){
            io_img[i*nc+c] =(T)(io_img[i*nc+c]*scale);
        }
        io_img.SetAlpha(255, i);
    }

    return WSP_STATE_SUCCESS;
}

template<typename T> int wsp::img::FindInnerOcclusion(
    wsp::Image<T> &io_img, 
    int max_continuous_occlusions_hor, 
    int max_continuous_occlusions_ver
){
    int width = io_img.width(), height = io_img.height();
    
    printf("finding inner occlusions..\n");
    printf("max continuous occlusions = (%d, %d)", max_continuous_occlusions_hor, max_continuous_occlusions_ver);

    //io_img.SaveState("inOcc_before.tif");
    //io_img.SaveAlpha("inOcc_alpha.tif");

    int occ_cnt = 0;
    // horizontal search -------------------------------------
    int end_w = width - 1;
    int threshold = max_continuous_occlusions_hor;
    {
        int x, y, i, xd, d;
        for(y=0; y<height; ++y){
            for(x=0; x<end_w; ++x){
                // counting continuous occluded pixels
                if(io_img.GetState(x, y)==WSP_PIXEL_STATE_OCCLUDED){
                    d = 1;
                    while(x+d>=0 && x+d<width ){
                        if(io_img.GetState(x+d, y)!=WSP_PIXEL_STATE_OCCLUDED){ break; }
                        ++d; 
                    }

                    //printf("YX(%d %d): %d<=%d\n", y, x, d, threshold);
                    if(d<=threshold){
                        for(i=0; i<d; ++i){
                            xd = x+i;
                            //io_img.SetElem((T)0, xd, y);
                            io_img.SetState(WSP_PIXEL_STATE_INNER_OCCLUDED, xd, y);
                        }
                        occ_cnt += d;
                    }
                    x += d;
                }
            }
        }
    }

    // vertical search ------------------------------------
    int end_h = height - 1;
    threshold = max_continuous_occlusions_ver;
    {
        int x, y, i, yd, d;
        for(x=0; x<width; ++x){
            for(y=0; y<end_h; ++y){
                // counting continuous occluded pixels
                if(io_img.GetState(x, y)==WSP_PIXEL_STATE_OCCLUDED){
                    d = 1;
                    while(y+d>=0 && y+d<height ){
                        if(io_img.GetState(x, y+d)!=WSP_PIXEL_STATE_OCCLUDED){ break; }
                        ++d; 
                    }

                    //printf("YX(%d %d): %d<=%d\n", y, x, d, threshold);
                    if(d<=threshold){
                        for(i=0; i<d; ++i){
                            yd = y+i;
                            //io_img.SetElem((T)0, xd, y);
                            io_img.SetState(WSP_PIXEL_STATE_INNER_OCCLUDED, x, yd);
                        }
                        occ_cnt += d;
                    }
                    y += d;
                }
            }
        }
    }

    //io_img.SaveState("inOcc_after.tif");
    printf("finding inner occlusions has done, %d occlusions are detected.\n", occ_cnt);
    return occ_cnt;
}

template<typename T>
int wsp::img::FindInnerOcclusionByAlpha(
    wsp::Image<T> &io_img, 
    int max_continuous_alpha_hor, 
    int max_continuous_alpha_ver, 
    uchar thre_alpha
){
    int cnt;
    std::cout<<"Finding inner occlusion by alpha"<<std::endl;
    int i, len=io_img.length();
    wsp::Image<T> tmp_img(io_img);
    for(i=0; i<len; ++i){
        if(io_img.GetAlpha(i)<thre_alpha){
            tmp_img.SetState(WSP_PIXEL_STATE_OCCLUDED, i);
            //io_img.SetElem(WspRgb(255, 0, 0), i);
        }
    }

    cnt=wsp::img::FindInnerOcclusion(tmp_img, max_continuous_alpha_hor, max_continuous_alpha_ver);

    for(i=0; i<len; ++i){
        if(tmp_img.GetState(i)==WSP_PIXEL_STATE_INNER_OCCLUDED){
            io_img.SetState(WSP_PIXEL_STATE_INNER_OCCLUDED, i);
        }
    }
    return cnt;
}


//! Interpolation --------------------------------------------------

template<typename T> 
inline int wsp::img::FillUpGaussianAverageState(
    wsp::Image<T> &io_img, const wsp::Image<uchar> *in_mask, wsp::img::PixelState target_state,
    int block_size, double variance, bool use_alpha_weight
)
{
    const uchar *mask = in_mask!=NULL? in_mask->data():NULL;
    return wsp::img::FillUpGaussianAverage<T>(io_img.data(), io_img.alpha_data(), io_img.state_data(), mask, io_img.width(), io_img.height(),io_img.num_of_channels(),
        target_state, block_size, variance, use_alpha_weight);
}
template<typename T> 
inline int wsp::img::FillUpGaussianAverageAlpha(
    wsp::Image<T> &io_img, 
    const wsp::Image<uchar> *in_mask, 
    int block_size, 
    double variance, 
    bool use_alpha_weight
)
{
    const uchar *mask = in_mask!=NULL? in_mask->data():NULL;
    return wsp::img::FillUpGaussianAverage<T>(
                io_img.data(), 
                io_img.alpha_data(), 
                io_img.state_data(), 
                mask, 
                io_img.width(), 
                io_img.height(),
                io_img.num_of_channels(),
                WSP_PIXEL_STATE_TRANSPARENT, 
                block_size, 
                variance, 
                use_alpha_weight
            );
}




template<typename T>
wsp::State wsp::img::FillUpTimes(
    wsp::Image<T> &io_img, 
    const wsp::Image<uchar> *in_mask, 
    wsp::img::PixelState target_state, 
    int init_blocksize, 
    int blocksize_step, 
    double variance
)
{
    int blocksize = init_blocksize;
    int occlusion_amount[2] = {static_cast<int>(io_img.width()*io_img.height()), 0};
    //char filename[64];

    wsp::Image<T> tmp_img(io_img);

    int loopCnt=0;
    do{
        if(target_state==WSP_PIXEL_STATE_TRANSPARENT){
            occlusion_amount[0] = wsp::img::FillUpGaussianAverageAlpha(tmp_img, in_mask, blocksize, variance);
        }else{
            occlusion_amount[0] = wsp::img::FillUpGaussianAverageState(tmp_img, in_mask, target_state, blocksize, variance);
        }
        printf("fill up: %d size has done, rest of occlusions are %d pixels.\n", blocksize, occlusion_amount[0]);
        blocksize+=blocksize_step;
        //if(occlusion_amount[0] == occlusion_amount[1]){ break; }
        occlusion_amount[1] = occlusion_amount[0];
        loopCnt++;
    }while(occlusion_amount[0]>0 && loopCnt<=MAX_WHILE_LOOP);

    io_img=tmp_img;
    printf("all of occlusions are filled.\n\n\n");

    return WSP_STATE_SUCCESS;
}
template<typename T> wsp::State wsp::img::FillUpTimesWithinSegment(wsp::Image<T> &io_img, const wsp::Image<int> &in_segm,
                                                        wsp::img::PixelState target_state, int init_blocksize)
{
    if(io_img.width()!=in_segm.width() || io_img.height()!=in_segm.height())
    { WSP_COMMON_ERROR_LOG("sizes of input image and segmentation image must be the same\n"); return WSP_STATE_FAILURE; }

    int w=static_cast<int>(io_img.width()), h=static_cast<int>(io_img.height());
    int len=static_cast<int>(io_img.length()), nc=static_cast<int>(io_img.num_of_channels());
    int i_seg, index_c, nSegm;
    int i, c;
    wsp::Image<uchar> mask;
    wsp::Vector<int> segmList;
    char filename[MAX_LENGTH_OF_NAME];

    in_segm.GetIndexList(segmList);
    nSegm = static_cast<int>(segmList.length());
    mask.SetSize(w, h);
    wsp::Image<T> tmp_img;

    uchar *tmp=new uchar[len];
    for(i_seg=0; i_seg<nSegm; i_seg++){
        index_c=segmList[i_seg];
        //if(index_c!=143){
        //    continue;
        //}
        tmp_img = io_img;
        //sprintf(filename, "test_fillSeg_alpha_index%d.tif", index_c);
        //tmp_img.SaveAlpha(filename);
        //! creating mask from segm image
        for(i=0; i<len; i++){
            if(in_segm[i]==index_c){
                mask[i]=255; continue;
            }
            mask[i]=0;
        }
        //sprintf(filename, "test_fillSeg_mask_index%d.tif", index_c);
        //mask.SaveImage(filename);
        //wsp::img::FillUpTimes<T>(tmp_img, &mask, target_state, init_blocksize);
        for(i=0; i<30; i++){
            wsp::img::FillUpGaussianAverageAlpha(tmp_img, &mask, 3);
        }
        //sprintf(filename, "test_fillSeg_depth_index%d.tif", index_c);
        //tmp_img.SaveImage(filename);

        //! update source image
        for(i=0; i<len; i++){
            if(in_segm[i]==index_c){
                for(c=0; c<nc; c++){
                    io_img[i*nc+c]=tmp_img[i*nc+c];
                }
                io_img.SetAlpha(255, i);
            }
        }
    }
    return WSP_STATE_SUCCESS;
}


// ===================================================================
// ===================================================================
//**
//                          Segmentation 
//**
// ===================================================================
// ===================================================================
template<typename T> 
wsp::State wsp::img::SetAverageForSegment(wsp::Image<T> &io_img, const wsp::Image<int> &in_segm, uchar thre_alpha)
{
    int w=in_segm.width(), h=in_segm.height(), len=in_segm.length(), nc=io_img.num_of_channels();
    if(w!=io_img.width() || h!=io_img.height())
    { 
        WSP_COMMON_ERROR_LOG(
            "sizes of input image and segmentation image must be the same: w(%d!=%d), h(%d!=%d)\n", 
            w, io_img.width(), h, io_img.height()
        ); 
        return WSP_STATE_FAILURE; 
    }
    int i_seg, index_c, nSegm;
    int i, c, cnt;
    double *sum;
    double *av;
    wsp::Vector<int> segmList;
    //char filename[MAX_LENGTH_OF_NAME];

    sum = WSP_NEW double[nc];
    av = WSP_NEW double[nc];
    in_segm.GetIndexList(segmList);
    nSegm = segmList.length();
    printf("SetAverageForEachSegment: num of segm=%d", nSegm);

    for(i_seg=0; i_seg<nSegm; i_seg++){
        index_c=segmList[i_seg];

        //! get average for segment area -----------
        //! init
        for(c=0; c<nc; c++){
            sum[c] = 0.0;
        }
        //! get sum
        cnt=0;
        for(i=0; i<len; i++){
            if(io_img.GetAlpha(i)<thre_alpha){ continue; }
            if(in_segm[i]==index_c){
                for(c=0; c<nc; c++){
                    sum[c]+=static_cast<double>(io_img[i*nc+c]);
                }
                cnt++;
            }
        }
        if(cnt==0){ continue; }
        //! get average
        for(c=0; c<nc; c++){
            av[c] = (sum[c]/static_cast<double>(cnt));
        }

        //! assign average value for current segment as index
        for(i=0; i<len; i++){
            if(io_img.GetAlpha(i)<thre_alpha){ continue; }
            if(in_segm[i]==index_c){
                for(c=0; c<nc; c++){
                    io_img[i*nc+c]=static_cast<uchar>(av[c]);
                }
            }
        }
    }
    delete[] av;
    delete[] sum;
    return WSP_STATE_SUCCESS;
}


namespace{
    enum HistogramLevelState{
        HISTOGRAM_LEVEL_WSP_STATE_TOO_FEW_LEVEL,
        HISTOGRAM_LEVEL_WSP_STATE_MOUNTAIN_LEVEL,
        HISTOGRAM_LEVEL_WSP_STATE_RESET,
        HISTOGRAM_LEVEL_WSP_STATE_CONTINUE
    };
    struct HistogramSegmInfo{
        int id;
        int start_intensity;
        int end_intensity;
        int num_total_pix;
    };

    //template<typename T>
    //void GetHistogramSegmList(
       // const wsp::Histogram<T> &in_histogram,
    //    wsp::Vector<HistogramSegmInfo> &o_segm_list,
       // int thre_pix_amount, 
       // int thre_continuous_intensity
    //){
       // T min=in_histogram.range_min();
    //    T max=in_histogram.range_max();
       // int len_hg = in_histogram.length();
       // int nc_hg  = in_histogram.num_of_channels();

    //    o_segm_list.SetLength( static_cast<int>(max-min) ); 

       // // find mountain
       // int num_segm;
       // {
    //        HistogramLevelState hg_level_state = HISTOGRAM_LEVEL_WSP_STATE_CONTINUE;
          //  int segm_id=1;
          //  int cont_intensity=0;
          //  int start_index = 0;
    //        int end_index;

          //  num_segm=0;
          //  const int *hg_ptr = in_histogram.data();
          //  const int *end_ptr = hg_ptr + len_hg * nc_hg;
          //  for( int i=0; hg_ptr!=end_ptr; ++hg_ptr, ++i )
          //  {
             //   if( *hg_ptr > thre_pix_amount )
             //   {
    //                cont_intensity=0;
    //                continue;
             //   }
            
             //   if( cont_intensity < thre_continuous_intensity )
             //   {
                //    ++cont_intensity;
             //   }
             //   else /* Register segmentation [start_index : end_index] */
             //   {
                //    end_index = i - cont_intensity;
                //    WSP_COMMON_DEBUG_LOG("segm %d: %d-%d\n", segm_id, start_index, end_index);

    //                /* Add to segm list */
                //    o_segm_list[num_segm].id = segm_id;
                //    o_segm_list[num_segm].start_intensity = start_index + min;
                //    o_segm_list[num_segm].end_intensity   = end_index   + min;

    //                /* Update segm info*/
                //    ++segm_id;
                //    ++num_segm;

    //                if( cont_intensity != 0 )
    //                {
                //        start_index = i - cont_intensity + 1;
    //                    end_index = i - 1;
                //        WSP_COMMON_DEBUG_LOG("segm %d: %d-%d\n", segm_id, start_index, end_index);

    //                    /* Add to segm list */
                //        o_segm_list[num_segm].id = segm_id;
                //        o_segm_list[num_segm].start_intensity = start_index + min;
                //        o_segm_list[num_segm].end_intensity   = end_index   + min;

    //                    /* Update segm info*/
                //        ++segm_id;
                //        ++num_segm;
    //                }

    //                /* Reset */
                //    start_index = i;
                //    cont_intensity = 0;
             //   }
          //  }
          //  end_index = len_hg * nc_hg - 1;
          //  WSP_COMMON_DEBUG_LOG("segm %d: %d-%d\n", segm_id, start_index, end_index);

    //        /* Add to segm list */
          //  o_segm_list[num_segm].id = segm_id;
          //  o_segm_list[num_segm].start_intensity = start_index + min;
          //  o_segm_list[num_segm].end_intensity   = end_index   + min;
          //  ++num_segm;
       // }

    //    o_segm_list.SetLength( num_segm );

       // return;
    //}



    enum HistogramSegmSwitch{
        HISTOGRAM_SEGM_SWITCH_GRATER,
        HISTOGRAM_SEGM_SWITCH_LESS
   };

    template<typename T>
    void GetHistogramSegmList(
        const wsp::Histogram<T> &in_histogram,
        wsp::Vector<HistogramSegmInfo> &o_segm_list,
        int thre_pix_amount
    ){
        T min=in_histogram.range_min();
        T max=in_histogram.range_max();
        int len_hg = in_histogram.length();
        int nc_hg  = in_histogram.num_of_channels();

        o_segm_list.SetLength( static_cast<int>(max-min) ); 

        // find mountain
        int num_segm;
        {
            int segm_id=1;
            int cont_intensity=0;
            int start_index = 0;
            int end_index;

            num_segm=0;
            const int *hg_ptr = in_histogram.data();
            const int *end_ptr = hg_ptr + len_hg * nc_hg;
            HistogramSegmSwitch segm_switch = HISTOGRAM_SEGM_SWITCH_LESS;
            int total_pix = 0;
            for( int i=0; hg_ptr!=end_ptr; ++hg_ptr, ++i )
            {
                total_pix += *hg_ptr;
                if( *hg_ptr > thre_pix_amount )
                {
                    if( segm_switch == HISTOGRAM_SEGM_SWITCH_LESS )
                    {
                        /* Register segmentation [start_index : end_index] */
                        end_index = i;
                        WSP_COMMON_DEBUG_LOG("segm %d: %d-%d\n", segm_id, start_index, end_index);

                        /* Add to segm list */
                        o_segm_list[num_segm].id = segm_id;
                        o_segm_list[num_segm].start_intensity = start_index + min;
                        o_segm_list[num_segm].end_intensity   = end_index   + min;
                        o_segm_list[num_segm].num_total_pix   = total_pix;

                        /* Update segm info*/
                        ++segm_id;
                        ++num_segm;

                        /* Reset */
                        start_index = i+1;
                        total_pix = 0;
                    }
                    segm_switch = HISTOGRAM_SEGM_SWITCH_GRATER;
                }
                else
                {
                    if( segm_switch == HISTOGRAM_SEGM_SWITCH_GRATER )
                    {
                        /* Register segmentation [start_index : end_index] */
                        end_index = i;
                        WSP_COMMON_DEBUG_LOG("segm %d: %d-%d\n", segm_id, start_index, end_index);

                        /* Add to segm list */
                        o_segm_list[num_segm].id = segm_id;
                        o_segm_list[num_segm].start_intensity = start_index + min;
                        o_segm_list[num_segm].end_intensity   = end_index   + min;

                        /* Update segm info*/
                        ++segm_id;
                        ++num_segm;

                        /* Reset */
                        start_index = i+1;
                        total_pix = 0;
                    }
                    segm_switch = HISTOGRAM_SEGM_SWITCH_LESS;
                }
            
            }
            if( start_index != len_hg * nc_hg )
            {
                end_index = len_hg * nc_hg - 1;
                WSP_COMMON_DEBUG_LOG("segm %d: %d-%d\n", segm_id, start_index, end_index);

                /* Add to segm list */
                o_segm_list[num_segm].id = segm_id;
                o_segm_list[num_segm].start_intensity = start_index + min;
                o_segm_list[num_segm].end_intensity   = end_index   + min;
                ++num_segm;
            }
        }

        o_segm_list.SetLength( num_segm );

        return;
    }
}
template<typename InType, typename OutType> 
int wsp::img::SegmentationByHistogram(
    const InType *in_image, 
    OutType *o_segm, 
    int length, int num_channels, 
    int thre_pix_amount,
    InType min, InType max
)
{
    if(num_channels!=1){
        WSP_COMMON_ERROR_LOG("Number of channels must be 1\n"); return 0;
    }
    int npix = length * num_channels;
    WSP_COMMON_DEBUG_LOG(
        "----------------------------\n"
        "  SegmentationByHistogram\n"
        "----------------------------\n"
        "min=%f, max=%f\n"
        "threshold=%d\n"
        , (float)min, (float)max, thre_pix_amount
    );

    wsp::Histogram<InType> histogram;
    histogram.SetData(in_image, length, num_channels, min, max);
    int len_hg = histogram.length();
    int nc_hg  = histogram.num_of_channels();

    wsp::Vector<HistogramSegmInfo> segm_list;

    GetHistogramSegmList<InType>(
        histogram,
        segm_list,
        thre_pix_amount
    );
    
    /* initialize destination, 0 is discontinuous area */
    memset(o_segm, 0, npix*sizeof(OutType));

    int num_segm = segm_list.length();
    /* fill destination up with segmentation ID */
    {
        int id, start_index, end_index;
        HistogramSegmInfo *segm_ptr = segm_list.data();
        HistogramSegmInfo *end_ptr  = segm_ptr + num_segm;
        for( ; segm_ptr!=end_ptr; ++segm_ptr )
        {
            id = segm_ptr->id;
            start_index = segm_ptr->start_intensity;
            end_index = segm_ptr->end_intensity;
            WSP_COMMON_DEBUG_LOG("segm id %d: %d %d\n", id, start_index, end_index);
            const InType *src_ptr = in_image;
            OutType *dst_ptr = o_segm;
            OutType *dst_end = dst_ptr + length;
            for(; dst_ptr!=dst_end; ++dst_ptr, src_ptr+=num_channels){
                if(start_index<=*src_ptr && *src_ptr<=end_index){
                    *dst_ptr = id;
                }
            }
        }
    }
    return num_segm;
}


template<typename T>
int wsp::img::DetermineThresholdOfSegmHistogram(
    const wsp::Histogram<T> &in_histogram, 
    int min_num_segm, 
    int max_num_segm,
    double min_num_pix_ratio,
    double max_num_pix_ratio,
    int num_loop
)
{
    assert( max_num_segm > 2 );
    WSP_COMMON_DEBUG_LOG(
        "--------------------\n"
        "Determining params of segmentation \n"
        "min %d, max %d, max_num_segm %d\n"
        , in_histogram.range_min()
        , in_histogram.range_max()
        , max_num_segm
    );
    int npix = in_histogram.total_num_pixels();
    const int decres_amount = npix * (max_num_pix_ratio - min_num_pix_ratio) / num_loop;
    int thre_pix_amount = npix * max_num_pix_ratio;

    int current_num_segm=0;

    int min_thre_pix_amount = thre_pix_amount;
    double min_variance = DBL_MAX;
    int res_num_segm = current_num_segm;

    for( int i=0;
        i < num_loop;
        thre_pix_amount -= decres_amount, ++i
    )
    {
        WSP_COMMON_DEBUG_LOG("\nloop %d:\n", i);
        wsp::Vector<HistogramSegmInfo> segm_list;

        GetHistogramSegmList(
            in_histogram,
            segm_list,
            thre_pix_amount
        );

        current_num_segm = segm_list.length();
        WSP_COMMON_DEBUG_LOG("thre %d:\tcurrent_num_segm=%d\n", thre_pix_amount, current_num_segm);


        WSP_COMMON_DEBUG_LOG(
            "current_num_segm = %d (min %d, max %d)\n"
            , current_num_segm, min_num_segm, max_num_segm);
        if( ( current_num_segm < min_num_segm) || ( current_num_segm > max_num_segm ) ){
            continue;
        }

        /* get variance */
        double average = npix / (double)current_num_segm;
        double variance = 0;
        double mult = 1.0/(double)current_num_segm;
        for( int i=0; i<current_num_segm; ++i)
        {
            variance += ( segm_list[i].num_total_pix - average )*( segm_list[i].num_total_pix - average )*mult;
        }
        //variance /= (double)current_num_segm;

        WSP_COMMON_DEBUG_LOG("variance < min_variance: %f < %f\n", variance, min_variance);
        if( variance < min_variance ){
            min_variance = variance;
            min_thre_pix_amount = thre_pix_amount;
            res_num_segm = current_num_segm;
            WSP_COMMON_DEBUG_LOG(
                "minimum thre = %d (num_segm %d, variance %f)\n"
                , min_thre_pix_amount, res_num_segm, min_variance);
        }

    }
    WSP_COMMON_DEBUG_LOG(
        "minimum thre = %d (num_segm %d, variance %f)\n"
        , min_thre_pix_amount, res_num_segm, min_variance);
    WSP_COMMON_DEBUG_LOG("--------------------\n");
    return min_thre_pix_amount;
}


// ===============================================================
// ===============================================================
//**
//                        Other
//**
// ===============================================================
// ===============================================================
template<typename T> 
wsp::State wsp::img::FitIntensity_BinarySearch(wsp::Image<T> &io_img, const wsp::Image<T> &in_comp_img, double min_scale, double max_scale, double (*eval_func)(double, double))
{
    u32 len = io_img.length(), nc = io_img.num_of_channels();
    if(max_scale<min_scale){
        WSP_COMMON_ERROR_LOG("max_scale must be larger than min_scale\n");
        return WSP_STATE_FAILURE;
    }
    if(len!=in_comp_img.length() || nc!=in_comp_img.num_of_channels()){
        WSP_COMMON_ERROR_LOG("invalid arguments\n");
        return WSP_STATE_FAILURE;
    }
    u32 i, c, cnt;
    T value;
    double middle_scale = 1.0; 
    double diff;
    double sum = 0.0, tmp_sum;
    double threshold = 0.001;
    wsp::Image<double> tmp_img(io_img);
    //for(i=0; i<len; i++){
    //    if(io_img.GetAlpha(i)<255){ continue; }
    //    for(c=0; c<nc; c++){
    //        sum += eval_func(static_cast<double>(in_comp_img[i*nc+c]), tmp_img[i*nc+c]);
    //    }
    //}
    cnt=0;
    while(true)
    {
        middle_scale = (max_scale+min_scale) / 2.0;
        //printf("%d: middle_scale=%f\n", cnt, middle_scale);
        tmp_sum = 0.0;
        //for(i=0; i<len; i++){
        //    if(io_img.GetAlpha(i)<255){ continue; }
        //    //diff = (static_cast<double>(in_comp_img[i]) - io_img[i]*middle_scale);
        //    //diff = (static_cast<double>(in_comp_img[i]) - tmp_img[i]*middle_scale);
        //    for(c=0; c<nc; c++){
        //        tmp_sum += eval_func(static_cast<double>(in_comp_img[i*nc+c]), tmp_img[i*nc+c]*middle_scale); //diff*diff;
        //    }Z
        //}
        for(i=0; i<len; i++){
            if(io_img.GetAlpha(i)<255){ continue; }
            //diff = (static_cast<double>(in_comp_img[i]) - io_img[i]*middle_scale);
            //diff = (static_cast<double>(in_comp_img[i]) - tmp_img[i]*middle_scale);
            for(c=0; c<nc; c++){
                tmp_sum += static_cast<double>(in_comp_img[i*nc+c]) - tmp_img[i*nc+c]*middle_scale; //diff*diff;
            }
        }
        //printf("%d: sum=%f, tmp_sum=%f\n", cnt, sum, tmp_sum);
        if(tmp_sum > 0){
            //printf("smaller\n");
            min_scale = middle_scale;
        }else{
            //printf("bigger\n");
            max_scale = middle_scale;
        }
        //printf("%d: min_scale=%f, max_scale=%f\n", cnt, min_scale, max_scale);
        if(fabs(min_scale-max_scale)<threshold){
            //printf("scale range reached threshold\n");
            break; 
        }
        //sum = tmp_sum;
        if(cnt>MAX_WHILE_LOOP){
            WSP_COMMON_WARNING_LOG("while loop counter reached max loop count\n");
            break;
        }
        cnt++;
        //printf("\n");
    }
    printf("computed scale: %f\n", middle_scale);
    for(i=0; i<len; i++){
        for(c=0; c<nc; c++){
            tmp_img[i*nc+c] *= middle_scale;
            io_img[i*nc+c] = static_cast<T>(tmp_img[i*nc+c]);
            if(sizeof(T)==1){
                if(tmp_img[i*nc+c]>=255){
                    io_img[i*nc+c] = 255;
                }else if(tmp_img[i*nc+c]<=0){
                    io_img[i*nc+c] = 0;
                }
            }
        }
    }
    return WSP_STATE_SUCCESS;
}

//template<typename T> 
//wsp::State FitIntensity_StepSearch(wsp::Image<T> &io_img, const wsp::Image<T> &in_comp_img, double min_scale=0.1, double max_scale=3.0, double search_step=0.05, double (*eval_func)(double, double)=wsp::math::GetSquaredDiff)
//{
//    u32 len = io_img.length(), nc = io_img.num_of_channels();
//    if(max_scale<min_scale){
//        WSP_COMMON_ERROR_LOG("max_scale must be larger than min_scale\n");
//        return WSP_STATE_FAILURE;
//    }
//    if(len!=in_comp_img.length() || nc!=in_comp_img.num_of_channels()){
//        WSP_COMMON_ERROR_LOG("invalid arguments\n");
//        return WSP_STATE_FAILURE;
//    }
//    u32 i, c, cnt;
//    T value;
//    double middle_scale = 1.0; 
//    double diff;
//    double sum = 0.0, tmp_sum;
//    double threshold = 0.001;
//    wsp::Image<double> tmp_img(io_img);
//    for(i=0; i<len; i++){
//        if(io_img.GetAlpha(i)<255){ continue; }
//        for(c=0; c<nc; c++){
//            sum += eval_func(static_cast<double>(in_comp_img[i*nc+c]), tmp_img[i*nc+c]);
//        }
//    }
//    double scale;
//    for(scale=1.0; ; ){
//        tmp_sum = 0.0;
//        for(i=0; i<len; i++){
//            if(io_img.GetAlpha(i)<255){ continue; }
//            //diff = (static_cast<double>(in_comp_img[i]) - io_img[i]*middle_scale);
//            //diff = (static_cast<double>(in_comp_img[i]) - tmp_img[i]*middle_scale);
//            for(c=0; c<nc; c++){
//                tmp_sum += eval_func(static_cast<double>(in_comp_img[i*nc+c]), tmp_img[i*nc+c]*middle_scale); //diff*diff;
//            }
//        }
//        printf("%d: sum=%f, tmp_sum=%f\n", cnt, sum, tmp_sum);
//        if(tmp_sum==sum){ 
//            printf("sum of correlation value is the same as previous\n");
//            break; 
//        }
//    }
//    return WSP_STATE_SUCCESS;
//}



template<typename T>
wsp::State wsp::img::GetSaliencyMap(
    const wsp::Image<T> &in_src,
    wsp::Image<T> &o_saliency_map
)
{
    char filename[MAX_LENGTH_OF_PATH];
    const int kNumOfSaliencyMapSrc = 9;
    wsp::Image<T> resized_img[kNumOfSaliencyMapSrc];
    wsp::Image<T> tmp_img;
    int src_w = in_src.width();
    int src_h = in_src.height();
    int nc = in_src.num_of_channels();

    /* Resize source of saliency map */
    int dst_w = src_w;
    int dst_h = src_h;
    for(int i=0; i<kNumOfSaliencyMapSrc; ++i, dst_w/=2, dst_h/=2 )
    {
        /* Scale down image */
        tmp_img.SetSize( dst_w, dst_h, nc );
        wsp::img::ScaleImageNearestNeighbor(
            in_src.data(), src_w, src_h, nc,
            tmp_img.data(), dst_w, dst_h
        );

        /* Scale up image */
        resized_img[i].SetSize( src_w, src_h, nc );
        wsp::img::ScaleImageNearestNeighbor(
            in_src.data(), dst_w, dst_h, nc,
            resized_img[i].data(), src_w, src_h
        );

        /* Save source images */
        sprintf(filename, "src_saliencymap-%d.png", i);
        resized_img[i].Save(filename);
    }

    return WSP_STATE_SUCCESS;
}

#endif
