/**
 * @file tpl_fn-plane.inl
 * 
 */

#ifndef    WSP_MATH_BASE_TEMPLATE_FUNCTION_PLANE_INL_
#define    WSP_MATH_BASE_TEMPLATE_FUNCTION_PLANE_INL_

#include "_tpl_fn-math.h"

template<typename T> 
wsp::math::State wsp::math::GetBisectorPlaneOfTwoPoints(
    const T in_point_a[3],
    const T in_point_b[3],
    wsp::math::PlaneParam<T> &o_bisec_plane_param
)
{
    if( in_point_a[0] == in_point_b[0]
     && in_point_a[1] == in_point_b[1]
     && in_point_a[2] == in_point_b[2]
    ){ 
        return WSP_MATH_STATE_INVALID_PARAMETER;
    }

    o_bisec_plane_param.normal[0] = in_point_b[0] - in_point_a[0];
    o_bisec_plane_param.normal[1] = in_point_b[1] - in_point_a[1];
    o_bisec_plane_param.normal[2] = in_point_b[2] - in_point_a[2];

    wsp::math::NormalizeLengthVec3( o_bisec_plane_param.normal );

    /* -----------------------
        no optimization code 
        -----------------------*/
    // float point_middle[3];
    o_bisec_plane_param.pivot[0] = (in_point_a[0] + in_point_b[0]) * 0.5f;
    o_bisec_plane_param.pivot[1] = (in_point_a[1] + in_point_b[1]) * 0.5f;
    o_bisec_plane_param.pivot[2] = (in_point_a[2] + in_point_b[2]) * 0.5f;
    // *o_bisec_plane_dot_product = 
    //     o_bisec_plane_normal[0] * point_on_plane[0] + 
    //     o_bisec_plane_normal[1] * point_on_plane[1] + 
    //     o_bisec_plane_normal[2] * point_on_plane[2];

    o_bisec_plane_param.dot = 
        wsp::math::GetDotProduct(o_bisec_plane_param.pivot, o_bisec_plane_param.normal);


    return WSP_MATH_STATE_SUCCESS;
}


/**
 * ���ʂe��
 * a x + b y + c z = d�@�E�E�E���@
 * ����AB���A�}��ϐ�t��p���āA
 * P = A + t e�@�E�E�E���A
 * �@A = ( Ax, Ay, Az )
 * �@e = [ Bx-Ax By-Ay Bz-Az ]
 * �@�@= [ Ex Ey Ez ]
 * �Ƃ��܂��B
 * 
 * ���A�𐬕��ɕ������܂��B
 * x = Ax + t Ex
 * y = Ay + t Ey
 * z = Az + t Ez
 * 
 * ���@�ɑ�����܂��B
 * �@a x + b y + c z = d
 * ��a (Ax + t Ex) + b ( Ay + t Ey ) + c ( Az + t Ez ) = d
 * ��a Ax + t a Ex + b Ay + t b Ey + c Az + t c Ez = d
 * ��t ( a Ex + b Ey + c Ez ) = d - a Ax - b Ay - c Az
 * ��t = { d - ( a Ax + b Ay + c Az ) } / ( a Ex + b Ey + c Ez )
 * 
 * DE = a Ex + b Ey + c Ez
 * �Ƃ��܂��B
 * 
 * DE���[���ȂƂ�
 * �@��_�͂���܂���i���ʂ`�Ɛ����`�a�͕��s�ł��j
 * DE���[���łȂ��Ƃ�
 * �@���ʂ`�ƒ����`�a�Ƃ̌�_�́A
 * �@P = A + t e
 * �@�@A = ( Ax, Ay, Az )
 * �@�@e = [ Bx-Ax By-Ay Bz-Az ]
 * �@�@t = { d - ( a Ax + b Ay + c Az ) } / ( a Ex + b Ey + c Ez )
 * �@e < 0 : �_�o�͓_�`����O
 * �@e > 1 : �_�o�͓_�`��艜
 */
template<typename T> 
wsp::math::State wsp::math::GetIntersectionPointOfPlaneAndLine(    
    const T in_point_a[3],
    const T in_point_b[3],
    const wsp::math::PlaneParam<T> &in_plane,
    T o_point_intersection[3],
    T parallel_threshold
)
{
    T vec_e[3];
    vec_e[0] = in_point_b[0] - in_point_a[0];
    vec_e[1] = in_point_b[1] - in_point_a[1];
    vec_e[2] = in_point_b[2] - in_point_a[2];

    wsp::math::NormalizeLengthVec3( vec_e );

    T de = wsp::math::GetDotProduct( in_plane.normal, vec_e );
    if( parallel_threshold >= fabs(de) )
    {    /* Plane and line is nearly parallel */
        return WSP_MATH_STATE_PARALLEL;
    }

    T t = ( in_plane.dot - wsp::math::GetDotProduct( in_plane.normal, in_point_a ) ) / de;
    o_point_intersection[0] = in_point_a[0] + t * vec_e[0];
    o_point_intersection[1] = in_point_a[1] + t * vec_e[1];
    o_point_intersection[2] = in_point_a[2] + t * vec_e[2];

    return WSP_MATH_STATE_SUCCESS;
}



/**
 *  ���ʂe�P��
 *      a1 x + b1 y + c1 z = d1�@�E�E�E���@
 *  ���ʂe�Q��
 *      a2 x + b2 y + c2 z = d2�@�E�E�E���A
 *  �Ƃ��A
 *  ����k������̒ʂ�_�`�A����̕����x�N�g��e�A�}��ϐ�t�A��p�����A
 *   P = A + t e
 *  �Ƃ��ċ��߂邱�ƂƂ��܂��B
 *  
 *  ����̕����x�N�g��e �́A�Q�̕��ʂ̖@���x�N�g���Ɛ����ȃx�N�g���ł��B
 *  �Q�̃x�N�g���Ɛ����ȃx�N�g���́A�O�όv�Z�ɂ�苁�܂�܂��B
 *   e = [ b1 c2 - c1 b2 c1 a2 - a1 c2 a1 b2 - b1 a2 ]
 *  ���̃x�N�g���̊e�����́A��Ŏg�p����̂ŁA
 *  Xe = b1 c2 - c1 b2
 *  Ye = c1 a2 - a1 c2
 *  Ze = a1 b2 - b1 a2
 *  �Ƃ��܂��B
 *  
 *  ���ɁA������ʂ�C�ӂ̂P�_�`�����߂܂��B
 *  ����́A����������ƁA���@�A���A���ƂƂ��ɖ�������x y z �̑g�������邱�Ƃł��B
 *  
 *  �A���R���P�����������Az = 0 �Ƃ��āA�A���Q���P���������Ɍ����ĂāAx�Ay�����߂܂��B
 *  a1 x + b1 y = d1�@�E�E�E���B
 *  a2 x + b2 y = d2�@�E�E�E���C
 *  ���B�A���C���
 *  x = ( d1 b2 - d2 b1 ) / ( a1 b2 - a2 b1 )
 *      = ( d1 b2 - d2 b1 ) / Ze
 *  y = ( d1 a2 - d2 a1 ) / ( b1 a2 - b2 a1 )
 *      = ( d1 a2 - d2 a1 ) / ( - Ze )
 *  
 *  Ze ���[���łȂ���΁A( x , y , 0 )�Ƃ����`�œ_�`�����܂�܂��B
 *  
 *  Ze ���[���̂Ƃ��ɂ́Ay = 0 �Ƃ��āA�A���Q���P���������Ɍ����ĂāAx�Az�����߂�B
 *      a1 x + c1 z = d1�@�E�E�E���D
 *      a2 x + c2 z = d2�@�E�E�E���E
 *  ���D�A���E���
 *  x = ( d1 c2 - d2 c1 ) / ( a1 c2 - a2 c1 )
 *      = ( d1 c2 - d2 c1 ) / ( - Ye )
 *  z = ( d1 a2 - d2 a1 ) / ( c1 a2 - c2 a1 )
 *      = ( d1 a2 - d2 a1 ) / Ye
 *  
 *  Ye ���[���łȂ���΁A( x , 0 , z )�Ƃ����`�œ_�`�����܂�B
 *  
 *  Ze, ����� Ye ���[���̂Ƃ��ɂ́Ax = 0 �Ƃ��āA�A���Q���P���������Ƃ��āAy�Az�����߂�B
 *      b1 y + c1 z = d1�@�E�E�E���F
 *      b2 y + c2 z = d2�@�E�E�E���G
 *  ���F�A���G���
 *  y = ( d1 c2 - c2 c1 ) / ( b1 c2 - b2 c1 )
 *      = ( d1 c2 - d2 c1 ) / Xe
 *  z = ( d1 b2 - d2 b1 ) / ( c1 b2 - c2 b1 )
 *      = ( d1 b2 - d2 b1 ) / ( - Xe )
 *  
 *  Xe ���[���łȂ���΁A( 0 , y , z )�Ƃ����`�œ_�`�����܂�B
 *  
 *  Xe, Ze, Ye �����ׂă[���̂Ƃ��ɂ́A�Q�̕��ʂ͕��s�Ō�����Ȃ��ꍇ�ł��B
 *  
 *  
 *  �܂Ƃ߂܂��ƁA
 *  �܂��A�Q�̕��ʂ̖@���x�N�g���̊O�όv�Z�ɂ��A����̕����x�N�g�� �����߂܂��B
 *  e = [ b1 c2 - c1 b2 c1 a2 - a1 c2 a1 b2 - b1 a2 ]
 *  �����ŁA
 *      Xe = b1 c2 - c1 b2
 *      Ye = c1 a2 - a1 c2
 *      Ze = a1 b2 - b1 a2
 *  �Ƃ��܂��B
 *  ����k���ʂ�_�o�́A
 *  Ze �� 0 �̂Ƃ��A
 *      ( ( d1 b2 - d2 b1 ) / Ze, ( d1 a2 - d2 a1 ) / ( - Ze ), 0 )
 *  Ye �� 0 �̂Ƃ��A
 *      ( ( d1 c2 - d2 c1 ) / ( - Ye ), 0, ( d1 a2 - d2 a1 ) / Ye )
 *  Xe �� 0 �̂Ƃ�
 *      ( 0, ( d1 c2 - d2 c1 ) / Xe, ( d1 b2 - d2 b1 ) / ( - Xe ) )
 *  Xe = 0, Ye = 0, Ze = 0 �̂Ƃ��A
 *      ����͂���܂���i�Q���ʂ͕��s�ł��j
 */
template<typename T> 
wsp::math::State wsp::math::GetIntersectionLineOfTwoPlanes(
    const wsp::math::PlaneParam<T> &in_plane_a,
    const wsp::math::PlaneParam<T> &in_plane_b,
    T o_vec_intersect_line[3],
    T o_point_on_intersect_line[3]
)
{
    wsp::math::GetCrossProduct( in_plane_a.normal, in_plane_b.normal, o_vec_intersect_line );

    if( 0.0 != o_vec_intersect_line[2] )
    {
        o_point_on_intersect_line[0] = ( in_plane_a.dot * in_plane_b.normal[1] - in_plane_b.dot * in_plane_a.normal[1] ) / o_vec_intersect_line[2];
        o_point_on_intersect_line[1] = ( in_plane_a.dot * in_plane_b.normal[0] - in_plane_b.dot * in_plane_a.normal[0] ) / ( - o_vec_intersect_line[2] );
        o_point_on_intersect_line[2] = 0;
        return WSP_MATH_STATE_SUCCESS;
    }
    if( 0.0 != o_vec_intersect_line[1] )
    {
        o_point_on_intersect_line[0] = ( in_plane_a.dot * in_plane_b.normal[2] - in_plane_b.dot * in_plane_a.normal[2] ) / ( - o_vec_intersect_line[1] );
        o_point_on_intersect_line[1] = 0;
        o_point_on_intersect_line[2] = ( in_plane_a.dot * in_plane_b.normal[0] - in_plane_b.dot * in_plane_a.normal[0] ) / o_vec_intersect_line[1];
        return WSP_MATH_STATE_SUCCESS;
    }
    if( 0.0 != o_vec_intersect_line[0] )
    {
        o_point_on_intersect_line[0] = 0;
        o_point_on_intersect_line[1] = ( in_plane_a.dot * in_plane_b.normal[2] - in_plane_b.dot * in_plane_a.normal[2] ) / o_vec_intersect_line[0];
        o_point_on_intersect_line[2] = ( in_plane_a.dot * in_plane_b.normal[1] - in_plane_b.dot * in_plane_a.normal[1] ) / ( - o_vec_intersect_line[0] );
        return WSP_MATH_STATE_SUCCESS;
    }

    // o_vec_intersect_line�̂ǂ̐������[���̂Ƃ��͂Q�̕��ʂ͕��s�Ō���Ȃ��B
    return WSP_MATH_STATE_PARALLEL;
}

#endif