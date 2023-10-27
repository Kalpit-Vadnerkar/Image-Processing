#include "ImageProcessing.h"
#include <cstdlib> 
#include <ctime>
#include <iostream>

namespace img {

Stencil::Stencil(int halfwidth) 
    : half_width(halfwidth), width(2*halfwidth + 1), stencil_values(new float[width * width])
{}

Stencil::~Stencil()
{
    delete[] stencil_values;
}

float& Stencil::operator()(int i, int j)
{
    int index = (i + half_width) + (j + half_width) * width;
    return stencil_values[index];
}

const float& Stencil::operator()(int i, int j) const
{
    int index = (i + half_width) + (j + half_width) * width;
    return stencil_values[index];
}

void BoundedLinearConvolution(const Stencil& stencil, const ImgProc& in, ImgProc& out)
{
    out.clear(in.getWidth(), in.getHeight(), in.getChannels());
    for(int j = 0; j < out.getHeight(); j++) {
        int jmin = j - stencil.halfwidth();
        int jmax = j + stencil.halfwidth();
#pragma omp parallel for
        for(int i = 0; i < out.getWidth(); i++) {
            int imin = i - stencil.halfwidth();
            int imax = i + stencil.halfwidth();
            std::vector<float> pixel(out.getChannels(), 0.0);
            for(int jj = jmin; jj <= jmax; jj++) {
                int stencilj = jj - j;
                for(int ii = imin; ii <= imax; ii++) {
                    int stencili = ii - i;
                    const float& stencil_value = stencil(stencili, stencilj);
                    if(jj >= 0 && jj < out.getHeight() && ii >= 0 && ii < out.getWidth()) {
                        std::vector<float> sample(in.getChannels(), 0.0);
                        in.value(ii, jj, sample);
                        for(size_t c = 0; c < sample.size(); c++) {
                            pixel[c] += sample[c] * stencil_value;
                        }
                    }
                }
            }
            out.set_value(i, j, pixel);
        }
    }
}

Stencil makeRandomFilter()
{
    Stencil stencil;

    std::srand(static_cast<unsigned>(std::time(0)));
    float sum = 0.0f;
    
    for (int i = 0; i < stencil.size() * stencil.size(); ++i) {
        float value = static_cast<float>(std::rand()) / (RAND_MAX / 0.2) - 0.1;
        stencil.setValue(i, value);
        sum += value;
    }

    float centerValue = stencil(stencil.halfwidth(), stencil.halfwidth());   
    stencil.setValue(stencil.halfwidth() * stencil.size() + stencil.halfwidth(), centerValue - (sum - 1.0f));
    return stencil;
}

} // namespace img

