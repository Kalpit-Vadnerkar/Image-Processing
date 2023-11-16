#include "ImageStatistics.h"
#include <vector>
#include <cfloat>

namespace img {

void Statistics(const ImgProc& in, std::vector<float>& max, std::vector<float>& min) {
    max.assign(in.getChannels(), -FLT_MAX);
    min.assign(in.getChannels(), FLT_MAX);

    for (int i = 0; i < in.getHeight(); i++) {
        for (int j = 0; j < in.getWidth(); j++) {
            std::vector<float> pixel;
            in.value(i, j, pixel);
            for (int c = 0; c < in.getChannels(); c++) {
                if (pixel[c] > max[c]) max[c] = pixel[c];
                if (pixel[c] < min[c]) min[c] = pixel[c];
            }
        }
    }
}

void Histogram(const ImgProc& in, const std::vector<float>& min, const std::vector<float>& max, int bins, std::vector<std::vector<float>>& histogram) {
    histogram.resize(in.getChannels(), std::vector<float>(bins, 0.0f));

    for (int i = 0; i < in.getHeight(); ++i) {
        for (int j = 0; j < in.getWidth(); ++j) {
            std::vector<float> pixel;
            in.value(i, j, pixel);
            for (int c = 0; c < in.getChannels(); ++c) {
                if (pixel[c] >= min[c] && pixel[c] <= max[c]) {
                    int binIndex = static_cast<int>((pixel[c] - min[c]) / (max[c] - min[c]) * (bins - 1));
                    histogram[c][binIndex]++;
                }
            }
        }
    }
}

void NormalizeHistograms(std::vector<std::vector<float>>& histograms) {
#pragma omp parallel for
    for (std::vector<float>& channelHistogram : histograms) {
        float htotal = 0.0f;
        for (float bin : channelHistogram) {
            htotal += bin;
        }
        for (float& bin : channelHistogram) {
            bin /= htotal;
        }
    }
}

void Cumulative(const std::vector<std::vector<float>>& histogram, std::vector<std::vector<float>>& cumulative) {
    cumulative.resize(histogram.size(), std::vector<float>(histogram[0].size(), 0.0f));
#pragma omp parallel for
    for (int c = 0; c < static_cast<int>(histogram.size()); ++c) {
        cumulative[c][0] = histogram[c][0];
        for (int i = 1; i < static_cast<int>(histogram[c].size()); ++i) {
            cumulative[c][i] = cumulative[c][i - 1] + histogram[c][i];
        }
    }
}

void ApplyCumulative(ImgProc& out) {
    const int bins = 500;

    std::vector<float> max, min;
    Statistics(out, max, min); 

    std::vector<std::vector<float>> histograms;
    Histogram(out, min, max, bins, histograms);
    
    // Normalize histograms to create PDFs
    NormalizeHistograms(histograms);

    std::vector<std::vector<float>> cumulative;
    Cumulative(histograms, cumulative);

    for (int i = 0; i < out.getHeight(); ++i) {
        for (int j = 0; j < out.getWidth(); ++j) {
            std::vector<float> pixel;
            out.value(i, j, pixel);
            for (int c = 0; c < out.getChannels(); ++c) {
                int binIndex = static_cast<int>((pixel[c] - min[c]) / (max[c] - min[c]) * (bins - 1));
                pixel[c] =cumulative[c][binIndex]; 
            }
            out.set_value(i, j, pixel);
        }
    }
}


} // namespace img

