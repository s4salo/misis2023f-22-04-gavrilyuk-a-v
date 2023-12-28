#include <ImageProcessor/ImageProcessor.hpp>
fs::path ImageProcessor::CONFIG_FILE_PATH = "settings.json";

void ImageProcessor::setConfigFilePath(const std::string& path) {
    CONFIG_FILE_PATH = fs::path(path) / "settings.json";
}

void ImageProcessor::setConfigFileDirectory(const std::string& path) {
    CONFIG_FILE_PATH = fs::path(path);
}

std::string ImageProcessor::filterToString(Settings::Filter filter) {
    switch (filter) {
    case Settings::NLM: return "NLM";
    case Settings::BILATERAL: return "BILATERAL";
    case Settings::NONE: return "NONE";
    default: return "NONE";
    }
}

ImageProcessor::Settings::Filter ImageProcessor::stringToFilter(const std::string& filter) {
    if (filter == "NLM") return ImageProcessor::Settings::NLM;
    if (filter == "BILATERAL") return ImageProcessor::Settings::BILATERAL;
    if (filter == "NONE") return ImageProcessor::Settings::NONE;
    return ImageProcessor::Settings::NONE;
}

std::string ImageProcessor::thresholdMethodToString(Settings::ThresholdMethod method) {
    switch (method) {
    case Settings::BINARY: return "BINARY";
    case Settings::OTSU: return "OTSU";
    case Settings::MEAN_STD_DEV: return "MEAN_STD_DEV";
    case Settings::KAPUR: return "KAPUR";
    default: return "BINARY";
    }
}

ImageProcessor::Settings::ThresholdMethod ImageProcessor::stringToThresholdMethod(const std::string& thresholdMethod) {
    if (thresholdMethod == "BINARY") return ImageProcessor::Settings::BINARY;
    if (thresholdMethod == "OTSU") return ImageProcessor::Settings::OTSU;
    if (thresholdMethod == "MEAN_STD_DEV") return ImageProcessor::Settings::MEAN_STD_DEV;
    if (thresholdMethod == "KAPUR") return ImageProcessor::Settings::KAPUR;
}



// Serialization
void to_json(json& j, const ImageProcessor::Settings& settings) {
    j = json{
        {"threshold", settings.threshold},
        {"filterIterations", settings.filterIterations},
        {"filterhParam", settings.filterhParam},
        {"ksize", settings.ksize},
        {"BDC", settings.BDC},
        {"filter", ImageProcessor::filterToString(settings.filter)},
        {"thresholdMethod", ImageProcessor::thresholdMethodToString(settings.thresholdMethod)}
    };
}

void from_json(const json& j, ImageProcessor::Settings& settings) {
    j.at("threshold").get_to(settings.threshold);
    j.at("filterIterations").get_to(settings.filterIterations);
    j.at("filterhParam").get_to(settings.filterhParam);
    j.at("ksize").get_to(settings.ksize);
    j.at("BDC").get_to(settings.BDC);
    std::string filter;
    j.at("filter").get_to(filter);
    settings.filter = ImageProcessor::stringToFilter(filter);
    std::string thresholdMethod;
    j.at("thresholdMethod").get_to(thresholdMethod);
    settings.thresholdMethod = ImageProcessor::stringToThresholdMethod(thresholdMethod);
}


// Function to check if configuration file exists
bool ImageProcessor::configExists() {
    std::ifstream file(CONFIG_FILE_PATH);
    return file.good();
}

// Function to create a default configuration file
void ImageProcessor::createDefaultConfig() {
    ImageProcessor::Settings defaultSettings;
    json j = defaultSettings;
    std::ofstream file(CONFIG_FILE_PATH);
    if (file.is_open()) {
        file << j.dump(4);
        file.close();
    }
    else {
        std::cerr << "Error: Could not create the default configuration file.\n";
    }
}

// Function to read the configuration from file
ImageProcessor::Settings ImageProcessor::readConfig() {
    std::ifstream file(CONFIG_FILE_PATH);
    json j;
    if (file.is_open()) {
        file >> j;
        file.close();
    }
    else {
        std::cerr << "Warning: Could not open the configuration file for reading. Using default settings.\n";
        return Settings(); // Return default settings if file could not be opened.
    }
    return j.get<Settings>();
}

// Function to update the configuration file
void ImageProcessor::updateConfig(const Settings& newSettings) {
    json j = newSettings;
    std::ofstream file(CONFIG_FILE_PATH, std::ios::out | std::ios::trunc);
    if (file.is_open()) {
        file << j.dump(4);
        file.close();
    }
    else {
        std::cerr << "Error: Could not open " << CONFIG_FILE_PATH << " for writing.\n";
    }
}

//Вспомогательные функции
std::vector<std::string> ImageProcessor::listFiles(const std::string& folder) {
    std::vector<std::string> filenames;
    for (const auto& entry : fs::directory_iterator(folder)) {
        filenames.push_back(entry.path().string());
    }
    return filenames;
}

std::vector<cv::Mat> ImageProcessor::loadImages(const std::vector<std::string>& filenames) {
    std::vector<cv::Mat> images;
    for (auto fn : filenames) {
        cv::Mat image = cv::imread(fn);
        images.push_back(image);
    }
    return images;
}

// Helper function: Calculate the cumulative sum of a histogram
std::vector<float> ImageProcessor::cumulativeSum(const cv::Mat& hist) {
    std::vector<float> cdf(hist.rows, 0.0f);
    cdf[0] = hist.at<float>(0);
    for (int i = 1; i < hist.rows; ++i) {
        cdf[i] = cdf[i - 1] + hist.at<float>(i);
    }
    return cdf;
}


//методы бинаризации
cv::Mat ImageProcessor::thresholdMeanStdDev(const cv::Mat& src, double k) {
    cv::Scalar mean, stdDev;
    cv::meanStdDev(src, mean, stdDev);

    int threshold = mean[0] + k * stdDev[0];
    cv::Mat dst;
    cv::threshold(src, dst, threshold, 255, cv::THRESH_BINARY);
    return dst;
}

cv::Mat ImageProcessor::thresholdKapur(const cv::Mat& src) {
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
cv::Mat ImageProcessor::denoiseBilateral(const cv::Mat& img, int iterations, int ksize, double sigma) {

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


cv::Mat ImageProcessor::denoiseImageNLM(const cv::Mat& img, int iterations, int hParam, int ksize) {
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



std::vector<cv::Mat> ImageProcessor::createMasks(std::vector<cv::Mat>& images, ImageProcessor::Settings settings) {
   
    std::vector<cv::Mat> masks;

    for (auto image : images) {
        cv::Mat gray;
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

        // filter choice
        if (settings.filter == ImageProcessor::Settings::Filter::NLM) {
            gray = denoiseImageNLM(gray, settings.filterIterations, settings.filterhParam, settings.ksize);
        }
        if (settings.filter == ImageProcessor::Settings::Filter::BILATERAL) {
            gray = denoiseBilateral(gray, settings.filterIterations, settings.ksize, 1.0);
        }

        cv::Mat mask;

        // threshold choice
        if (settings.thresholdMethod == ImageProcessor::Settings::ThresholdMethod::BINARY) {
            cv::threshold(gray, mask, settings.threshold, 255, cv::THRESH_BINARY);
        }
        else if (settings.thresholdMethod == ImageProcessor::Settings::ThresholdMethod::OTSU) {
            cv::threshold(gray, mask, 0, 255, cv::THRESH_OTSU);
        }
        else if (settings.thresholdMethod == ImageProcessor::Settings::ThresholdMethod::MEAN_STD_DEV) {
            mask = thresholdMeanStdDev(gray, static_cast<double>(settings.BDC));
        }
        else if (settings.thresholdMethod == ImageProcessor::Settings::ThresholdMethod::KAPUR) {
            mask = thresholdKapur(gray);
        }

        masks.push_back(mask);
    }
    return masks;
}