
#include <wsp/test/tpl_fn-test_lane.hpp"

#include <wsp/image/tpl_cl-image.h>

#include "cl-perlin_noise.h"

class MathTest
    : public wsp::TestLane<MathTest>
{
public:
    MathTest()
        : wsp::TestLane<MathTest>(*this)
    {
        WSP_TESTLANE_ADD_TEST( MathTest::PerlinNoiseTest );
    }

private:
    void PerlinNoiseTest();
};


void MathTest::PerlinNoiseTest()
{
    // Define the size of the image
    unsigned int width = 480, height = 480;

    // Create an empty PPM image
    int nc = 3;
    wsp::Image<u8> image(width, height, nc);
    wsp::Image<u8> image_x(width, height, nc);
    wsp::Image<u8> image_y(width, height, nc);

    // Create a PerlinNoise object with the reference permutation vector
    wsp::PerlinNoise pn( time(NULL) );

    unsigned int kk = 0;
    // Visit every pixel of the image and assign a color generated with Perlin noise
    for(unsigned int i = 0; i < height; ++i) {     // y
        for(unsigned int j = 0; j < width; ++j) {  // x
            double x = (double)j/((double)width);
            double y = (double)i/((double)height);

            {
                // Typical Perlin noise
                double n = pn.noise(10 * x, 10 * y, 0.8);

                // Wood like structure
                //n = 20 * pn.noise(x, y, 0.8);
                //n = n - floor(n);

                // Map the values to the [0, 255] interval, for simplicity we use 
                // tones of grey
                u8 pix_value = (u8)floor(255 * n);
                image[kk*nc]   = pix_value;
                image[kk*nc+1] = pix_value;
                image[kk*nc+2] = pix_value;
            }

            {
                // Typical Perlin noise
                double n = pn.noise(0, 10 * y, 0.8);

                // Wood like structure
                //n = 20 * pn.noise(x, y, 0.8);
                //n = n - floor(n);

                // Map the values to the [0, 255] interval, for simplicity we use 
                // tones of grey
                u8 pix_value = (u8)floor(255 * n);
                image_y[kk*nc]   = pix_value;
                image_y[kk*nc+1] = pix_value;
                image_y[kk*nc+2] = pix_value;
            }

            {
                // Typical Perlin noise
                double n = pn.noise(10 * x, 0, 0.8);

                // Wood like structure
                //n = 20 * pn.noise(x, y, 0.8);
                //n = n - floor(n);

                // Map the values to the [0, 255] interval, for simplicity we use 
                // tones of grey
                u8 pix_value = (u8)floor(255 * n);
                image_x[kk*nc]   = pix_value;
                image_x[kk*nc+1] = pix_value;
                image_x[kk*nc+2] = pix_value;
            }

            kk++;
        }
    }

    // Save the image in a binary PPM file
    image.Save("perlinnoise.jpg");
    image_x.Save("perlinnoise_x.jpg");
    image_y.Save("perlinnoise_y.jpg");
}

