#ifndef IMAGE_PROCESSOR_HPP
#define IMAGE_PROCESSOR_HPP

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <opencv2/core/utils/logger.hpp>

namespace fs = std::filesystem;
using json = nlohmann::json;

class ImageProcessor {
public:
    struct Settings {
        int threshold = 128;
        int filterIterations = 2;
        int filterhParam = 10;
        int ksize = 5;
        int BDC = 0;
        enum Filter {
            NLM,
            BILATERAL,
            NONE
        };
        Filter filter = NONE;
        enum ThresholdMethod {
            BINARY,
            OTSU,
            MEAN_STD_DEV,
            KAPUR
        };
        ThresholdMethod thresholdMethod = BINARY;
    };

    static bool configExists();
    static void createDefaultConfig();
    static Settings readConfig();
    static void updateConfig(const Settings& newSettings);
    static std::vector<std::string> listFiles(const std::string& folder);
    static std::vector<cv::Mat> loadImages(const std::vector<std::string>& filenames);
    static std::vector<cv::Mat> createMasks(const std::vector<std::string>& filenames, std::vector<cv::Mat>& images, Settings settings);

private:
    static std::string filterToString(Settings::Filter filter);
    static Settings::Filter stringToFilter(const std::string& filter);
    static std::string thresholdMethodToString(Settings::ThresholdMethod method);
    static Settings::ThresholdMethod stringToThresholdMethod(const std::string& thresholdMethod);

    friend void to_json(json& j, const Settings& settings);
    friend void from_json(const json& j, Settings& settings);

    static std::vector<float> cumulativeSum(const cv::Mat& hist);

    static cv::Mat thresholdMeanStdDev(const cv::Mat& src, double k);
    static cv::Mat thresholdKapur(const cv::Mat& src);

    static cv::Mat denoiseBilateral(const cv::Mat& img, int iterations, int ksize, double sigma);
    static cv::Mat denoiseImageNLM(const cv::Mat& img, int iterations, int hParam, int ksize);
};

#endif // IMAGE_PROCESSOR_HPP