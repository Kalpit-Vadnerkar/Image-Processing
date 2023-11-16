#ifndef IMAGESTATISTICS_H
#define IMAGESTATISTICS_H

#include "ImgProc.h"
#include <vector>
#include <string>

namespace img {

void Statistics(const ImgProc& in, std::vector<float>& max, std::vector<float>& min);

void Histogram(const ImgProc& in, const std::vector<float>& min, const std::vector<float>& max, int bins, std::vector<std::vector<float>>& histogram);

void NormalizeHistograms(std::vector<std::vector<float>>& histograms);

void Cumulative(const std::vector<std::vector<float>>& histogram, std::vector<std::vector<float>>& cumulative);

void ApplyCumulative(ImgProc& out);

} // namespace img

#endif // IMAGESTATISTICS_H

