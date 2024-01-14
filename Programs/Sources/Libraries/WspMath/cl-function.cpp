// @file cl-function.cpp
// 

#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <iostream>

#include "_define_mathutil.h"
#include "cl-function.h"

using namespace std;

u32 wsp::Function::msm_seed = 0x1234;
u32 wsp::Function::lcg_seed = 0x1111;


// Constructor, Destructor ----------------------------------------------------------

#define INIT_WSP_FUNCTION m_start(0.0),m_end(0.0),m_id(0),m_af_G(1.0e-011),bm_mean(0.0),bm_dev(1.0)

void wsp::Function::init(){
    // for linear function
    m_gradient = 1.0;
    m_interceptL = 0.0;

    // for quadratic function
    m_mult2 = 1.0;
    m_mult1 = 1.0;
    m_interceptQ = 0.0;

    // for normal distribution
    m_nd_mean = 0.0;
    m_nd_dev = 1.0;

    // for acceleration
    m_af_mass_myself = 1.0;
    m_af_mass_neighbor = 1.0;

    // for Molecular Dynamics
    for(int itr_i=0; itr_i<3; itr_i++){
        vel[itr_i] = 0.0;
        acc[itr_i] = 0.0;
        acc_prev[itr_i] = 0.0;
    }
    mass = 1.0;
    sigma = 1.0;
    epsilon = 1.0;

#ifdef USE_QT
    //m_color = new QColor();
    m_color.setRgb(0,0,0);
    dot_size = 2;
    line_width = 0;
    drawLine = true;
    drawDot = true;
#endif

}
wsp::Function::Function()
    :INIT_WSP_FUNCTION
{
    init();
#ifdef DEBUG_CLASS
    printf("wsp::Function constructor\n");
#endif

}
wsp::Function::Function(u32 length)
    : wsp::Vector<double>(length), INIT_WSP_FUNCTION
{ init(); }
wsp::Function::Function(const double* ary, u32 length)
    : wsp::Vector<double>(ary, length), INIT_WSP_FUNCTION
{ init(); }
wsp::Function::Function(const wsp::Function& func)
    : wsp::Vector<double>(func), INIT_WSP_FUNCTION
{ init(); }
wsp::Function::Function(const wsp::Vector<double>& ary)
    : wsp::Vector<double>(ary), INIT_WSP_FUNCTION
{ init(); }
wsp::Function::~Function()
{
#ifdef DEBUG_CLASS
    printf("wsp::Function destructor: %d\n", m_id);
#endif
#ifdef USE_QT
    //delete m_color;
    //m_color=NULL;
#endif
}

// end Constructor, Destructor ------------------------------------------------------

// Accessor: Getter -----------------------------------------------------------------

double wsp::Function::getX(u32 index) const{
    return (double)index * getStep() + m_start;
}
double wsp::Function::getStart() const{ return m_start; }
double wsp::Function::getEnd() const{ return m_end; }
inline double wsp::Function::getStep() const{
    if(length_<=1){
        return 0.0;
    }
    return (m_end - m_start)/(double)(length_-1);
}
int wsp::Function::getId() const{ return m_id; }

double wsp::Function::getNorm() const{
    return sqrt(getNormSquare());
}

double wsp::Function::getNormSquare() const{
    if(data_==NULL){ return 0.0; }
    double sum=0.0;
    for(u32 itr_i=0; itr_i<length_; itr_i++){
        sum += data_[itr_i]*data_[itr_i];
    }
    return sum;
}
double wsp::Function::getRootMeanSquareError(const wsp::Function& another) const{
    wsp::Function fn(*this - another);
    return sqrt(fn.getNormSquare() / (double)length_ );
}

double wsp::Function::getMassMyself() const{
    return m_af_mass_myself;
}
double wsp::Function::getMassNeighbor() const{
    return m_af_mass_neighbor;
}

//#ifdef USE_QT
//const QColor wsp::Function::getColor() const{
//    QColor color(*m_color);
//    return color;
//}
//#endif
// end Accessor: Getter -------------------------------------------------------------

// Accessor: Setter -----------------------------------------------------------------
void wsp::Function::setId(int id){ m_id = id; }
void wsp::Function::setStart(double x_start){
    if(x_start>=m_end){
        cerr<<"x_start must be smaller than x end"<<endl;
        return;
    }
    m_start = x_start;
}
void wsp::Function::setEnd(double x_end){
    if(m_start>=x_end){
        cerr<<"x_end must be larger than x start"<<endl;
        return;
    }
    m_end = x_end;
}
void wsp::Function::setStep(double x_step){
    if(m_start>=m_end || x_step==0){
        cerr<<"Error in setParameters: ";
        cerr<<"x start must be larger than x end, or x step shouldn't be 0:";
        cout<<"x start="<<m_start<<", x end="<<m_end<<", x step="<<x_step<<endl;
        return;
    }
    u32 length = (u32)((m_end-m_start)/x_step) + 1;
    SetLength(length);
}
//#ifdef USE_QT
//void wsp::Function::setColor(QColor& color){
//    m_color->setRgb(color.rgb());
//}
//#endif
void wsp::Function::Copy(const wsp::Function &func, u32 skipStep)
{
    m_start = func.getStart();
    m_end = func.getEnd();
    m_id = func.getId();
    CopyFrom(func.data(), func.length(), skipStep);
}
void wsp::Function::Copy(const wsp::Vector<double> &ary, u32 skipStep){
    m_start = DV_START;
    m_end = DV_END;
    m_id = DV_ID;
    CopyFrom(ary.data(), ary.length(), skipStep);
}

void wsp::Function::setParameters(double x_start, double x_end, double x_step, int id)
{
    if(x_start>=x_end || x_step==0){
        cerr<<"Error in setParameters: ";
        cerr<<"x_start must be larger than x_end, or x_step shouldn't be 0:";
        cout<<"x_start="<<x_start<<", x_end="<<x_end<<", x_step="<<x_step<<endl;
        return;
    }
    m_start = x_start;
    m_end = x_end;
    m_id = id;
    setStep(x_step);
}
void wsp::Function::setParameters(double x_start, double x_end, u32 numOfSample, int id)
{
    if(x_start>=x_end || numOfSample==0){
        cerr<<"Error in setParameters: ";
        cerr<<"x_start must be larger than x_end, or numOfSample shouldn't be 0:";
        cout<<"x_start="<<x_start<<", x_end="<<x_end<<", sample="<<numOfSample<<endl;
        return;
    }
    m_start = x_start;
    m_end = x_end;
    m_id = id;
    SetLength(numOfSample);
}
double wsp::Function::middleSquareMethod()
{
    u32 randv;
    randv = msm_seed * msm_seed;
    randv = randv<<8;
    randv = randv>>16;
    //cout<<hex<<randv<<endl;
    if(randv < (1<<8)){
        msm_seed +=1;
        msm_seed = msm_seed * msm_seed;
    }else{
        msm_seed = randv;
    }
    return randv;
}
double wsp::Function::linearCongruentialGenerator()
{
    u32 a, c, m, randv;
    a = (lcg_seed<<16) + (lcg_seed<<1) + lcg_seed;
    m = 0x7ffffff;
    c = 0;
    randv = (a + c) & m;
    //cout<<randv<<endl;
    lcg_seed = randv;
    return randv;
}

double wsp::Function::boxMuller(double mean, double dev, bool use_sin)
{
    double rand1, rand2;

    rand1 = ((double)rand()) / (double)(RAND_MAX+1.0);
    rand2 = ((double)rand()) / (double)(RAND_MAX+1.0);

    if( use_sin == false ){ 
        return dev * sqrt( -2.0 * log( rand1 ) ) * cos( 2.0 * M_PI * rand2 ) + mean; 
    }
    return dev * sqrt( -2.0 * log( rand1 ) ) * sin( 2.0 * M_PI * rand2 ) + mean;
}

void wsp::Function::addNoise(AlgorithmName alg)
{
    srand((u32)time(NULL));
    double x = m_start;
    for(u32 itr_i=0; itr_i<length_; itr_i++)
    {
        switch(alg){
        case BoxMuller:
            data_[itr_i] += boxMuller(bm_mean, bm_dev, true);
            break;
        case MiddleSquare:
            data_[itr_i] += middleSquareMethod();
            break;
        case LinearCongruential:
            data_[itr_i] += linearCongruentialGenerator();
            break;
        default:
            data_[itr_i] += boxMuller(bm_mean, bm_dev, true);
            break;
        }
    }
}

void wsp::Function::setFunction(double(*func)(double x)){
    double x = m_start;
    for(u32 itr_i=0; itr_i<length_; itr_i++)
    {
        data_[itr_i] = func(x);
        x += getStep();
    }
}
void wsp::Function::setFunction(double(wsp::Function::* func)(double x)){
    double x = m_start;
    for(u32 itr_i=0; itr_i<length_; itr_i++)
    {
        data_[itr_i] = (this->* func)(x);
        x += getStep();
    }
}
void wsp::Function::setFunction(u32 retIndex, const wsp::Function& initValues, void(*func)(double t, const wsp::Function& y, wsp::Function& dest)){
    double x = m_start;
    wsp::Function y1(initValues), y2(initValues);
    data_[0] = initValues[retIndex];
    for(u32 itr_i=0; itr_i<length_; itr_i++)
    {
        double x, y;
        x=y1[0];
        y=y1[1];
        func(x, y1, y2);
        x += getStep();
        y1 = y2;
        x=y1[0];
        y=y1[1];
        data_[itr_i] = y2[retIndex];
    }
}

void wsp::Function::setNormalDistribution(double mean, double dev){
    m_nd_mean = mean;
    m_nd_dev = dev;
    setFunction(&wsp::Function::getNormalDistribution);
}
void wsp::Function::setLinear(double gradient, double intercept){
    m_gradient = gradient;
    m_interceptL = intercept;
    setFunction(&wsp::Function::getLinear);
}
void wsp::Function::setQuadratic(double multiplier2, double multiplier1, double intercept){
    m_mult1 = multiplier1;
    m_mult2 = multiplier2;
    m_interceptQ = intercept;
    setFunction(&wsp::Function::getQuadratic);
}
void wsp::Function::setSin(){
    setFunction(sin);
}
void wsp::Function::setCos(){
    setFunction(cos);
}
void wsp::Function::setExp(){
    setFunction(exp);
}
void wsp::Function::setTan(){
    setFunction(tan);
}

void wsp::Function::setTanh(){
    setFunction(tanh);
}

double wsp::Function::quadrature(wsp::Function::AlgorithmName alg)
{
    if(length_==0){
        return 0.0;
    }
    switch(alg)
    {
    case Trapezoid:
        return trapezoid();
    case Simpson:
        return simpson();
    default:
        return 0.0;
    }
}

void wsp::Function::integrate(wsp::Function::AlgorithmName alg, double initValue, double(*dfunc)(double t, double y))
{
    if(length_==0){
        return;
    }
    switch(alg)
    {
    case Euler:
        improvedEuler(initValue, dfunc);
        return;
    case RungeKutta:
        rungeKutta4th(initValue, dfunc);
        return;
    case RungeKuttaGill:
        rungeKuttaGill(initValue, dfunc);
        return;
    default:
        return;
    }
}
void wsp::Function::absolute(){
    for(u32 itr_i=0; itr_i<length_; itr_i++){
        data_[itr_i] = fabs(data_[itr_i]);
    }
}

void wsp::Function::setMassWspself(double mass){
    m_af_mass_myself = mass;
}
void wsp::Function::setMassNeighbor(double mass){
    m_af_mass_neighbor = mass;
}


// end Accessor: Setter -------------------------------------------------------------

// Private Functions ----------------------------------------------------------------

double wsp::Function::getNormalDistribution(double x){
    if(m_nd_dev==0){ return 0.0; }
    return exp(-(x-m_nd_mean)*(x-m_nd_mean) / (2*m_nd_dev*m_nd_dev));
}
double wsp::Function::getLinear(double x){
    return m_gradient*x + m_interceptL;
}
double wsp::Function::getQuadratic(double x){
    return m_mult2*x*x + m_mult1*x + m_interceptQ;
}
double wsp::Function::getAcceleration(double t){
//    m_af_r1 = ;
    double dist = m_af_r1 - m_af_r2;
    return m_af_G * m_af_mass_neighbor / (dist * dist);
}

void wsp::Function::rungeKutta4th(double initValue, double(*dfunc)(double t, double y))
{
    double y, t, h, k1, k2, k3, k4;
    u32 itr_i;
    t = m_start;
    h = getStep();
    y = initValue;
    data_[0] = y;
    for(itr_i=1; itr_i<length_; itr_i++)
    {
        k1 = h * dfunc(t, y); 
        k2 = h * dfunc(t + h/2.0, y + k1/2.0);
        k3 = h * dfunc(t + h/2.0, y + k2/2.0);
        k4 = h * dfunc(t + h, y + k3);

        y += (k1 + 2*(k2 + k3) + k4) / 6.0;

        data_[itr_i] = y;
        t += h;
    }
}
void wsp::Function::rungeKuttaGill(double initValue, double(*dfunc)(double t, double y))
{
    double y, t, h, k, q;
    u32 itr_i;
    t = m_start;
    h = getStep();
    y = initValue;
    data_[0] = y;
    for(itr_i=1; itr_i<length_; itr_i++)
    {
        // step 1
        k = h * dfunc(t, y); 

        // step 2
        y += k/2.0;
        q = k;
        k = h * dfunc(t+h/2.0, y);

        // step 3
        y += (1.0-sqrt(0.5))*(k-q);
        q = (2.0-sqrt(2.0))*k + (-2.0+3.0*sqrt(0.5))*q;
        k = h * dfunc(t+h/2.0, y);

        // step 4
        y += (1.0+sqrt(0.5))*(k-q);
        q = (2.0+sqrt(2.0))*k + (-2.0-3.0*sqrt(0.5))*q;
        k = h * dfunc(t+h, y);

        // last step
        y += k/6.0 - q/3.0;

        data_[itr_i] = y;
        t += h;
    }
}
void wsp::Function::improvedEuler(double initValue, double(*dfunc)(double t, double y)){
    double t, h, y1, y2, fn1, fn2;
    u32 i;
    t = m_start;
    h = getStep();
    y1 = initValue;
    data_[0] = y1;
    for(i=1; i<length_; i++)
    {
        t += h;
        fn1 = dfunc(t, y1);

        y2 = y1 + h * fn1;
        fn2 = dfunc(t + h, y2);

        y1 += 0.5 * h * (fn1 + fn2);
        data_[i] = y1;
    }
}

double wsp::Function::trapezoid()
{
    double h = getStep();
    double result = (h/2.0) * (data_[0] + data_[length_-1]);
    for(u32 itr_i=1; itr_i<length_-1; itr_i++){
        result += h*data_[itr_i];
    }
    return result;
}
double wsp::Function::simpson(){
    double y0, y1, y2;
    bool is_odd;
    double result = 0.0;

    for(u32 itr_i=0; itr_i<length_-2; itr_i+=2){
        y0 = data_[itr_i];
        y1 = data_[itr_i+1];
        y2 = data_[itr_i+2];
        result += y0 + 4.0*y1 + y2;
    }

    is_odd = ((length_-1)%2 == 1)? true:false;

    if(is_odd){
        y0 = data_[length_-3];
        y1 = data_[length_-2];
        y2 = data_[length_-1];
        result += (5.0*y2 + 8.0*y1 - y0) / 4.0;
    }

    return result*getStep()/3.0;
}
double wsp::Function::gaussianQuadrature5thOrder(double(*func)(double x))
{
    double result, xi, wi, x[5], w[5], range, offset;
    result = 0.0;

    range = (getEnd() - getStart()) / 2.0;
    offset = (getEnd()+getStart())/2.0;

    x[0] = 0.90617984;
    x[1] = 0.53846931;
    x[2] = 0.0;
    x[3] = -x[1];
    x[4] = -x[0];

    w[0] = 0.23692688;
    w[1] = 0.47862867;
    w[2] = 0.56888888;
    w[3] = w[1];
    w[4] = w[0];

    for(int itr_i=0; itr_i<5; itr_i++){
        xi = x[itr_i] * range + offset;
        wi = w[itr_i] * range;

        result += func(xi) * wi;
    }
    return result;
}

// I/O ---------------------------------------------------------------------------
//#ifdef USE_LIBTIFF
//wsp::State wsp::Function::SaveAsTiff(const char* fileName){
//    wsp::State state;
//    uchar *img = new uchar[length_];
//    double max = GetMax();
//    double min = GetMin();
//    double range = max - min;
//    for(int i=0; i<length_; i++){
//        img[i] = (uchar)floor((data_[i]-min)*(255/range));
//        printf("%d\n", img[i]);
//    }
//    state = wsp::img::SaveAsTiff8(img, length_, 1, fileName);
//    if(state!=WSP_STATE_SUCCESS){ delete img; return state; }
//    delete img;
//    return WSP_STATE_SUCCESS;
//}
//#else
wsp::State wsp::Function::SaveAsTiff(const char* fileName){
    WSP_COMMON_ERROR_LOG("SaveAsTiff is not supported.");
    return WSP_STATE_FAILURE;
}
//#endif
// Operators ------------------------------------------------------------------------

wsp::Function& wsp::Function::operator=(const wsp::Function& rhs){
    wsp::Vector<double>::operator = (rhs);
    m_start = rhs.getStart();
    m_end = rhs.getEnd();
    m_id = rhs.getId();
    
    #ifdef USE_QT
    m_color = rhs.m_color;
    #endif

    return *this;
}


ostream& operator<<(ostream& lhs, const wsp::Function& rhs)
{
    for(u32 itr_i=0; itr_i<rhs.length(); itr_i++){
        lhs<<setw(PRINT_COLW)<<setprecision(PRINT_PREC);
        //lhs<<scientific;
        lhs<<PRINT_TYPE;
        lhs<<rhs[itr_i];
        //lhs<<endl;
        //lhs<<" ";
        //lhs<<'\t';
    }
    lhs<<endl;
    return lhs;
}

// end Operators --------------------------------------------------------------------

