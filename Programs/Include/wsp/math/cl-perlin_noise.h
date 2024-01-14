

#ifndef WSP_MATH_CLASS_PERLINNOISE_H
#define WSP_MATH_CLASS_PERLINNOISE_H

#include <vector>

#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include <wsp/common/_define_commoncore.h>
#endif

namespace wsp{

    class WSP_DLL_EXPORT PerlinNoise {
        // The permutation vector
        std::vector<int> p;
    public:
        // Initialize with the reference values for the permutation vector
        PerlinNoise();

        // Generate a new permutation vector based on the value of seed
        explicit PerlinNoise(unsigned int seed);

        // Get a noise value, for 2D images z can have any value
        double noise(double x, double y, double z);
    private:
        double fade(double t);
        double lerp(double t, double a, double b);
        double grad(int hash, double x, double y, double z);
    };

}

#endif
