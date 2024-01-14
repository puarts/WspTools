// @file cl-function.h
// 

#ifndef __WSP_FUNCTION_H__
#define __WSP_FUNCTION_H__

#include <iostream>

#ifndef __WSP_COMMONUTIL_TEMPLATE_CLASS_VECTOR_H__
#include <wsp/common/tpl_cl-vector.h>
#endif
#include <wsp/common/_define_qt.h>


#define DV_START (0.0)
#define DV_END (1.0)
#define DV_ID (0)

#define MY_RAND_MAX (0x7ffffff)

namespace wsp{

    class WSP_DLL_EXPORT Function
        : public wsp::Vector<double>
    {
    public:
        enum AlgorithmName{
            Trapezoid, Simpson, GaussianQuadrature,
            Euler, RungeKutta, RungeKuttaGill,
            BoxMuller,
            MiddleSquare, LinearCongruential
        };

    private:
        // private variables ------------------
        double m_start;
        double m_end;
        int m_id;

        // ------------------------------------
        // Don't need to be careful about these varables below
        // because these are temporary variables.
        // for linear function
        double m_gradient;
        double m_interceptL;

        // for quadratic function
        double m_mult2;
        double m_mult1;
        double m_interceptQ;

        // for normal distribution
        double m_nd_mean;
        double m_nd_dev;

        // for acceleration function
        double m_af_mass_myself;
        double m_af_mass_neighbor;
        double m_af_r1;
        double m_af_r2;
        double m_af_G;

        // for random number generation
        static u32 msm_seed;
        static u32 lcg_seed;

        //-------------------------------------


    public:
        //! for WspQtGraphViewer
        #ifdef USE_QT
        QColor m_color;
        u32 dot_size, line_width;
        bool drawDot;
        bool drawLine;
        #endif

        // for boxmuller
        double bm_mean;
        double bm_dev;
        // for Molecular Dynamics
        double vel[3];
        double acc[3];
        double acc_prev[3];
        double mass;
        double sigma, epsilon;



    private:
        // private functions ------------------
        void init();
        double getLinear(double x);
        double getQuadratic(double x);
        double getNormalDistribution(double x);
        double getAcceleration(double x);

        // Quadrature Algorithm
        double trapezoid();
        double simpson();

        // Integration Algorithm for ODE
        void rungeKutta4th(double initValue, double(*dfunc)(double t, double y));
        void rungeKuttaGill(double initValue, double(*dfunc)(double t, double y));
        void improvedEuler(double initValue, double(*dfunc)(double t, double y));

        // Random Number Generation Method
        double middleSquareMethod();
        double linearCongruentialGenerator();
        double boxMuller(double mean, double stddev, bool use_sin);

    public:
        Function();
        Function(u32 length);
        Function(const double* func, u32 length);
        Function(const wsp::Function& func);
        Function(const wsp::Vector<double>& ary);
        ~Function();

        // Accessor: Getter --------------------------
        double getX(u32 index) const;
        double getStart() const;
        double getEnd() const;
        inline double getStep() const;
        int getId() const;
        double getNorm() const;
        double getNormSquare() const;
        double getRootMeanSquareError(const wsp::Function& another) const;


        // molecular dynamics functions
        double getMassMyself() const;
        double getMassNeighbor() const;


        // Accessor: Setter --------------------------
        void Copy(const wsp::Function &func, u32 skipStep=0);
        void Copy(const wsp::Vector<double> &ary, u32 skipStep=0);
        void setId(int id);
        void setStep(double x_step);
        void setStart(double x_start);
        void setEnd(double x_end);
        void setParameters(double x_start, double x_end, double x_step=1.0, int id=0);
        void setParameters(double x_start, double x_end, u32 numOfSample, int id=0);

        void setMassWspself(double mass);
        void setMassNeighbor(double mass);

        void setFunction(double(*func)(double x));
        void setFunction(double(wsp::Function::* func)(double x));
        void setFunction(u32 retIndex, const wsp::Function& initValues, void(*func)(double t, const wsp::Function& y, wsp::Function& dest));
        void setNormalDistribution(double mean, double dev);
        void setLinear(double gradient, double intercept);
        void setQuadratic(double multiplier2, double multiplier1, double intercept);
        void setSin();
        void setCos();
        void setTan();
        void setTanh();
        void setExp();


        void absolute();
        double quadrature(AlgorithmName alg);
        double gaussianQuadrature5thOrder(double(*func)(double x));
        void integrate(AlgorithmName alg, double initValues, double(*dfunc)(double t, double y) );
        void addNoise(AlgorithmName alg);

        // I/O ------------------------------------
        wsp::State SaveAsTiff(const char* fileName);

        // Operators-------------------------------
        wsp::Function& operator=(const wsp::Function& other);
        //inline wsp::Function operator-(const wsp::Function &right) const{
        //    wsp::Function result(*this);
        //    if(right.length()<length_){
        //        return result;
        //    }
        //    for(u32 i=0; i<length_; i++){
        //        result[i] -= right[i];
        //    }
        //    return result;
        //}



    };

}

std::ostream& operator<<(std::ostream& lhs, const wsp::Function& rhs);


#endif