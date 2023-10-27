#include "Fractals.h"
#include <complex>
#include <iostream>

namespace img {

// Constructor for ColorLUT
ColorLUT::ColorLUT(double gamma_value) : gamma(gamma_value) {
    black = {0.0f, 0.0f, 0.0f};
    
    // Initialize the Clemson color bands
    bands.push_back({239/255.0f, 219/255.0f, 178/255.0f}); // RGB239 219 178
    bands.push_back({245/255.0f, 102/255.0f, 0.0f});       // RGB245 102 0
    bands.push_back({82/255.0f, 45/255.0f, 128/255.0f});   // RGB82 45 128
    bands.push_back({185/255.0f, 71/255.0f, 0.0f});        // RGB185 71 0
    bands.push_back({84/255.0f, 98/255.0f, 35/255.0f});    // RGB84 98 35
    bands.push_back({0.0f, 94/255.0f, 184/255.0f});        // RGB0 94 184
    bands.push_back({203/255.0f, 196/255.0f, 188/255.0f}); // RGB203 196 188
    bands.push_back({136/255.0f, 139/255.0f, 141/255.0f}); // RGB136 139 141
}

void ColorLUT::operator()(const double& value, std::vector<float>& C) const {
    // If v > 1, return black
    if (value > 1) {
        C = black;
        return;
    }

    // For v ≤ 1
    double x = value * sqrt(bands.size());
    int m = static_cast<int>(x);
    int m_prime = m + 1;
    double alpha = x - m;	

    // If m' > M, set m' = m
    if (m_prime >= static_cast<int>(bands.size())) {
        m_prime = m;
    }

    // Linear interpolation of colors
    for (int i = 0; i < 3; i++) {
        C[i] = bands[m][i] * (1.0f - alpha) + bands[m_prime][i] * alpha;
    }
}

Point JuliaSet::operator()(const Point& P) const {
    std::complex<double> Pc(P.x, P.y);
    std::complex<double> cc(c.x, c.y);
    
    for(int i=0;i<nb_iterations;i++)
    {
        std::complex<double> temp = Pc;
        for(int c=1;c<cycles;c++) { temp = temp * Pc; }
        Pc = temp + cc;
        // Escape condition
        if (std::abs(Pc) > 2.0) { break; }
    }  

    Point Pout;
    Pout.x = Pc.real();
    Pout.y = Pc.imag();
    return Pout;
}

void ApplyFractalWarpLUT( ImgProc& out, const Point& center, const double range, const Warp& w, const ColorLUT& lut )
{
    float R = 2.0;
    for( int j=0;j<out.getHeight();j++)
    {
#pragma omp parallel for
        for( int i=0;i<out.getWidth();i++)
        {
            Point P;
            P.x = 2.0*(double)i/(double)out.getWidth() - 1.0;
            P.y = 2.0*(double)j/(double)out.getHeight() - 1.0;
            P.x *= range;
            P.y *= range;
            P.x += center.x;
            P.y += center.y;
            Point PP = w(P);
            double rate = std::sqrt(PP.x*PP.x + PP.y*PP.y)/R;
            std::vector<float> v(3,0.0);
            lut(rate,v);
            out.set_value(i,j,v);
        }
    }
}

} // namespace img

