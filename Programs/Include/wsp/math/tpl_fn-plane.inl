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
 * 平面Ｆを
 * a x + b y + c z = d　・・・式①
 * 線分ABを、媒介変数tを用いて、
 * P = A + t e　・・・式②
 * 　A = ( Ax, Ay, Az )
 * 　e = [ Bx-Ax By-Ay Bz-Az ]
 * 　　= [ Ex Ey Ez ]
 * とします。
 * 
 * 式②を成分に分解します。
 * x = Ax + t Ex
 * y = Ay + t Ey
 * z = Az + t Ez
 * 
 * 式①に代入します。
 * 　a x + b y + c z = d
 * ⇔a (Ax + t Ex) + b ( Ay + t Ey ) + c ( Az + t Ez ) = d
 * ⇔a Ax + t a Ex + b Ay + t b Ey + c Az + t c Ez = d
 * ⇔t ( a Ex + b Ey + c Ez ) = d - a Ax - b Ay - c Az
 * ⇔t = { d - ( a Ax + b Ay + c Az ) } / ( a Ex + b Ey + c Ez )
 * 
 * DE = a Ex + b Ey + c Ez
 * とします。
 * 
 * DEがゼロなとき
 * 　交点はありません（平面Ａと線分ＡＢは平行です）
 * DEがゼロでないとき
 * 　平面Ａと直線ＡＢとの交点は、
 * 　P = A + t e
 * 　　A = ( Ax, Ay, Az )
 * 　　e = [ Bx-Ax By-Ay Bz-Az ]
 * 　　t = { d - ( a Ax + b Ay + c Az ) } / ( a Ex + b Ey + c Ez )
 * 　e < 0 : 点Ｐは点Ａより手前
 * 　e > 1 : 点Ｐは点Ａより奥
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
 *  平面Ｆ１を
 *      a1 x + b1 y + c1 z = d1　・・・式①
 *  平面Ｆ２を
 *      a2 x + b2 y + c2 z = d2　・・・式②
 *  とし、
 *  交線Ｌを交線の通る点Ａ、交線の方向ベクトルe、媒介変数t、を用いた、
 *   P = A + t e
 *  として求めることとします。
 *  
 *  交線の方向ベクトルe は、２つの平面の法線ベクトルと垂直なベクトルです。
 *  ２つのベクトルと垂直なベクトルは、外積計算により求まります。
 *   e = [ b1 c2 - c1 b2 c1 a2 - a1 c2 a1 b2 - b1 a2 ]
 *  このベクトルの各成分は、後で使用するので、
 *  Xe = b1 c2 - c1 b2
 *  Ye = c1 a2 - a1 c2
 *  Ze = a1 b2 - b1 a2
 *  とします。
 *  
 *  次に、交線が通る任意の１点Ａを求めます。
 *  これは、言いかえると、式①、式②をとともに満たすをx y z の組を見つけることです。
 *  
 *  連立３元１次方程式を、z = 0 として、連立２元１次方程式に見立てて、x、yを求めます。
 *  a1 x + b1 y = d1　・・・式③
 *  a2 x + b2 y = d2　・・・式④
 *  式③、式④より
 *  x = ( d1 b2 - d2 b1 ) / ( a1 b2 - a2 b1 )
 *      = ( d1 b2 - d2 b1 ) / Ze
 *  y = ( d1 a2 - d2 a1 ) / ( b1 a2 - b2 a1 )
 *      = ( d1 a2 - d2 a1 ) / ( - Ze )
 *  
 *  Ze がゼロでなければ、( x , y , 0 )という形で点Ａが求まります。
 *  
 *  Ze がゼロのときには、y = 0 として、連立２元１次方程式に見立てて、x、zを求める。
 *      a1 x + c1 z = d1　・・・式⑤
 *      a2 x + c2 z = d2　・・・式⑥
 *  式⑤、式⑥より
 *  x = ( d1 c2 - d2 c1 ) / ( a1 c2 - a2 c1 )
 *      = ( d1 c2 - d2 c1 ) / ( - Ye )
 *  z = ( d1 a2 - d2 a1 ) / ( c1 a2 - c2 a1 )
 *      = ( d1 a2 - d2 a1 ) / Ye
 *  
 *  Ye がゼロでなければ、( x , 0 , z )という形で点Ａが求まる。
 *  
 *  Ze, および Ye がゼロのときには、x = 0 として、連立２元１次方程式として、y、zを求める。
 *      b1 y + c1 z = d1　・・・式⑦
 *      b2 y + c2 z = d2　・・・式⑧
 *  式⑦、式⑧より
 *  y = ( d1 c2 - c2 c1 ) / ( b1 c2 - b2 c1 )
 *      = ( d1 c2 - d2 c1 ) / Xe
 *  z = ( d1 b2 - d2 b1 ) / ( c1 b2 - c2 b1 )
 *      = ( d1 b2 - d2 b1 ) / ( - Xe )
 *  
 *  Xe がゼロでなければ、( 0 , y , z )という形で点Ａが求まる。
 *  
 *  Xe, Ze, Ye がすべてゼロのときには、２つの平面は平行で交線がない場合です。
 *  
 *  
 *  まとめますと、
 *  まず、２つの平面の法線ベクトルの外積計算により、交線の方向ベクトル を求めます。
 *  e = [ b1 c2 - c1 b2 c1 a2 - a1 c2 a1 b2 - b1 a2 ]
 *  ここで、
 *      Xe = b1 c2 - c1 b2
 *      Ye = c1 a2 - a1 c2
 *      Ze = a1 b2 - b1 a2
 *  とします。
 *  交線Ｌが通る点Ｐは、
 *  Ze ≠ 0 のとき、
 *      ( ( d1 b2 - d2 b1 ) / Ze, ( d1 a2 - d2 a1 ) / ( - Ze ), 0 )
 *  Ye ≠ 0 のとき、
 *      ( ( d1 c2 - d2 c1 ) / ( - Ye ), 0, ( d1 a2 - d2 a1 ) / Ye )
 *  Xe ≠ 0 のとき
 *      ( 0, ( d1 c2 - d2 c1 ) / Xe, ( d1 b2 - d2 b1 ) / ( - Xe ) )
 *  Xe = 0, Ye = 0, Ze = 0 のとき、
 *      交線はありません（２平面は平行です）
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

    // o_vec_intersect_lineのどの成分もゼロのときは２つの平面は平行で交線なし。
    return WSP_MATH_STATE_PARALLEL;
}

#endif