#include<iostream>
#include<algorithm>
#include<string>
#include<cmath>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include <opencv2/core/utils/logger.hpp>
namespace fs = std::filesystem;
using json = nlohmann::json;
const std::string CONFIG_FILE_PATH = "settings.json";


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

// Converting Settings to string and vice versa

std::string filterToString(Settings::Filter filter) {
    switch (filter) {
        case Settings::NLM: return "NLM";
        case Settings::BILATERAL: return "BILATERAL";
        case Settings::NONE: return "NONE";
        default: return "NONE";
    }
}

Settings::Filter stringToFilter(const std::string& filter) {
    if (filter == "NLM") return Settings::NLM;
    if (filter == "BILATERAL") return Settings::BILATERAL;
    if (filter == "NONE") return Settings::NONE;
    return Settings::NONE; 
}

std::string thresholdMethodToString(Settings::ThresholdMethod method) {
    switch (method) {
        case Settings::BINARY: return "BINARY";
        case Settings::OTSU: return "OTSU";
        case Settings::MEAN_STD_DEV: return "MEAN_STD_DEV";
        case Settings::KAPUR: return "KAPUR";
        default: return "BINARY";
    }
}

Settings::ThresholdMethod stringToThresholdMethod(const std::string& thresholdMethod) {
    if (thresholdMethod == "BINARY") return Settings::BINARY;
    if (thresholdMethod == "OTSU") return Settings::OTSU;
    if (thresholdMethod == "MEAN_STD_DEV") return Settings::MEAN_STD_DEV;
    if (thresholdMethod == "KAPUR") return Settings::KAPUR;
}



// Serialization
void to_json(json& j, const Settings& settings) {
    j = json{
        {"threshold", settings.threshold},
        {"filterIterations", settings.filterIterations},
        {"filterhParam", settings.filterhParam},
        {"ksize", settings.ksize},
        {"BDC", settings.BDC},
        {"filter", filterToString(settings.filter)},
        {"thresholdMethod", thresholdMethodToString(settings.thresholdMethod)}
    };
}

void from_json(const json& j, Settings& settings) {
    j.at("threshold").get_to(settings.threshold);
    j.at("filterIterations").get_to(settings.filterIterations);
    j.at("filterhParam").get_to(settings.filterhParam);
    j.at("ksize").get_to(settings.ksize);
    j.at("BDC").get_to(settings.BDC);
    std::string filter;
    j.at("filter").get_to(filter);
    settings.filter = stringToFilter(filter);
    std::string thresholdMethod;
    j.at("thresholdMethod").get_to(thresholdMethod);
    settings.thresholdMethod = stringToThresholdMethod(thresholdMethod);
}


// Function to check if configuration file exists
bool configExists() {
    std::ifstream file(CONFIG_FILE_PATH);
    return file.good();
}

// Function to create a default configuration file
void createDefaultConfig() {
    Settings defaultSettings; // Initialize with default constructor
    json j = defaultSettings; // Serialize the settings structure to JSON
    std::ofstream file(CONFIG_FILE_PATH);
    file << j.dump(4); // Write JSON to the file with a 4-space indent
    file.close();
}

// Function to read the configuration from file
Settings readConfig() {
    std::ifstream file(CONFIG_FILE_PATH);
    json j;
    file >> j;
    return j.get<Settings>();
}

// Function to update the configuration file
void updateConfig(const Settings& newSettings) {
    json j = newSettings;
    // Use ofstream with the trunc mode to overwrite the existing file.
    std::ofstream file(CONFIG_FILE_PATH, std::ios::out | std::ios::trunc);
    if (file.is_open()) {
        file << j.dump(4);  // Write JSON to the file with a 4-space indent
        file.close();
    }
    else {
        // Handle error if file could not be opened.
        std::cerr << "Error: Could not open " << CONFIG_FILE_PATH << " for writing.\n";
    }
}

//Вспомогательные функции
std::vector<std::string> listFiles(const std::string& folder) {
    std::vector<std::string> filenames;
    for (const auto& entry : fs::directory_iterator(folder)) {
        filenames.push_back(entry.path().string());
    }
    return filenames;
}

std::vector<cv::Mat> loadImages(const std::vector<std::string>& filenames) {
    std::vector<cv::Mat> images;
    for (auto fn : filenames) {
        cv::Mat image = cv::imread(fn);
        images.push_back(image);
    }
    return images;
}

// Helper function: Calculate the cumulative sum of a histogram
std::vector<float> cumulativeSum(const cv::Mat& hist) {
    std::vector<float> cdf(hist.rows, 0.0f);
    cdf[0] = hist.at<float>(0);
    for (int i = 1; i < hist.rows; ++i) {
        cdf[i] = cdf[i - 1] + hist.at<float>(i);
    }
    return cdf;
}


//методы бинаризации
cv::Mat thresholdMeanStdDev(const cv::Mat& src, double k) {
    cv::Scalar mean, stdDev;
    cv::meanStdDev(src, mean, stdDev);

    int threshold = mean[0] + k * stdDev[0];
    cv::Mat dst;
    cv::threshold(src, dst, threshold, 255, cv::THRESH_BINARY);
    return dst;
}

cv::Mat thresholdKapur(const cv::Mat& src) {
    CV_Assert(src.type() == CV_8UC1); // Ensure the image is grayscale

    const int histSize = 256;
    float range[] = { 0, 256 }; // the upper boundary is exclusive
    const float* histRange = { range };
    bool uniform = true, accumulate = false;

    // Calculate histogram
    cv::Mat hist;
    cv::calcHist(&src, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);

    // Normalize the histogram
    hist /= src.total();

    // Calculate the cumulative distribution function (CDF)
    std::vector<float> cdf = cumulativeSum(hist);

    double maxEntropy = -1;
    int bestThreshold = 0;
    for (int t = 0; t < histSize; t++) {
        double backgroundEntropy = 0, objectEntropy = 0;

        for (int i = 0; i <= t; i++) {
            if (cdf[i] > 0 && hist.at<float>(i) > 0) {
                backgroundEntropy -= (hist.at<float>(i) / cdf[t]) * std::log(hist.at<float>(i) / cdf[t]);
            }
        }

        for (int i = t + 1; i < histSize; i++) {
            if (cdf[i] > cdf[t] && hist.at<float>(i) > 0) {
                objectEntropy -= (hist.at<float>(i) / (1 - cdf[t])) * std::log(hist.at<float>(i) / (1 - cdf[t]));
            }
        }

        double totalEntropy = backgroundEntropy + objectEntropy;
        if (totalEntropy > maxEntropy) {
            maxEntropy = totalEntropy;
            bestThreshold = t;
        }
    }

    // Apply the best threshold
    cv::Mat dst;
    cv::threshold(src, dst, bestThreshold, 255, cv::THRESH_BINARY);
    return dst;
}

//Методы фильтрации
cv::Mat denoiseBilateral(const cv::Mat& img, int iterations, int ksize, double sigma) {

    cv::Mat result = img;

    for (int i = 0; i < iterations; i++) {
        cv::Mat blur;
        cv::GaussianBlur(result, blur, cv::Size(ksize, ksize), 0);  
        cv::Mat temp;
        cv::bilateralFilter(blur, temp, 1, 1, 0);
        result = temp;
    }
    return result;
}


cv::Mat denoiseImageNLM(const cv::Mat& img, int iterations, int hParam, int ksize) {
    cv::Mat result = img;
    for (int i = 0; i < iterations; i++) {
        cv::Mat blur;
        cv::GaussianBlur(result, blur, cv::Size(ksize, ksize), 0);   
        cv::Mat temp;
        cv::fastNlMeansDenoising(blur, temp, hParam);
        result = temp;
    }
    return result;
}



std::vector<cv::Mat> createMasks(const std::vector<std::string>& filenames, std::vector<cv::Mat>& images, Settings settings) {

    std::vector<cv::Mat> masks;

    for (auto image : images) {
        cv::Mat gray;
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

        // filter choice
        if (settings.filter == Settings::Filter::NLM) {
            gray = denoiseImageNLM(gray, settings.filterIterations, settings.filterhParam, settings.ksize);
        }
        if (settings.filter == Settings::Filter::BILATERAL) {
            gray = denoiseBilateral(gray, settings.filterIterations, settings.ksize, 1);
        }

        // mask
        cv::Mat mask;

        if (settings.thresholdMethod == Settings::ThresholdMethod::BINARY) {
            cv::threshold(gray, mask, settings.threshold, 255, cv::THRESH_BINARY);
        }
        else if (settings.thresholdMethod == Settings::ThresholdMethod::OTSU) {
            cv::threshold(gray, mask, 0, 255, cv::THRESH_OTSU);
        }
        else if (settings.thresholdMethod == Settings::ThresholdMethod::MEAN_STD_DEV) {
            mask = thresholdMeanStdDev(gray, settings.BDC);
        }
        else if (settings.thresholdMethod == Settings::ThresholdMethod::KAPUR) {
            mask = thresholdKapur(gray);
        }

        masks.push_back(mask);

        gray.release();
        mask.release();
    }
    return masks;
}

int main() {
    std::vector<cv::Mat> images;
    std::vector<cv::Mat> masks;
    std::vector<std::string> filenames;
    cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

    // Check if the config file exists
    if (!configExists()) {
        createDefaultConfig();
    }

    Settings config = readConfig();

    config.thresholdMethod = Settings::KAPUR;
    config.filter = Settings::BILATERAL;
    updateConfig(config);

    filenames = listFiles("C:/Users/gav-y/source/repos/ConsoleApplication1/ConsoleApplication1/images");
    images = loadImages(filenames);
    masks = createMasks(filenames, images, config);

    std::cout << "Size of images array:" << images.size() << "\n";

    if (images.size() == masks.size()) {
        std::cout << "Masks are done";
    }
    else {
        std::cout << "SMTH went wrong";
    }

    cv::imshow("Image", images[6]);
    cv::imshow("Mask", masks[6]);
    cv::waitKey(0);

    for (auto& image : images) {
        image.release();
    }
    for (auto& mask : masks) {
        mask.release();
    }

    return 0;
}