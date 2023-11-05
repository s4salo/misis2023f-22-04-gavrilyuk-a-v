#include<iostream>
#include<string>
#include<cmath>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <vector>
namespace fs = std::filesystem;

struct Settings {
    int threshold = 128;
    int filterIterations = 1;
    int filterhParam = 10;
    int ksize = 5;
    int BDC = 0;
    enum Filter {
        NLM,
        BILITERAL,
        NONE
    };
    Filter filter = NONE;
    enum ThresholdMethod {
        BINARY,
        OTSU,
        MEAN_STD_DEV
    };
    ThresholdMethod thresholdMethod = BINARY;
};

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


//методы бинаризации
cv::Mat thresholdMeanStdDev(const cv::Mat& src, double k) {
    cv::Scalar mean, stdDev;
    cv::meanStdDev(src, mean, stdDev);

    int threshold = mean[0] + k * stdDev[0];
    cv::Mat dst;
    cv::threshold(src, dst, threshold, 255, cv::THRESH_BINARY);

    return dst;
}

//Методы фильтрации
cv::Mat denoiseBiliteral(const cv::Mat& img, int iterations, int ksize, double sigma) {

    cv::Mat result = img;

    for (int i = 0; i < iterations; i++) {

        cv::Mat blur;
        cv::GaussianBlur(result, blur, cv::Size(ksize, ksize), 0);

        // Фильтрация    
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

        // Фильтрация    
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

        //выбор фильтра
        if (settings.filter == Settings::Filter::NLM) {
            gray = denoiseImageNLM(gray, settings.filterIterations, settings.filterhParam, settings.ksize);
        }
        if (settings.filter == Settings::Filter::BILITERAL) {
            gray = denoiseBiliteral(gray, settings.filterIterations, settings.ksize, 1);
        }

        // маска
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
    Settings config;

    config.BDC = 0.45;
    config.filter = Settings::Filter::BILITERAL;
    config.thresholdMethod = Settings::ThresholdMethod::MEAN_STD_DEV;
    config.threshold = 110;
    config.filterIterations = 1;
    config.ksize = 3;

    filenames = listFiles("./images");
    images = loadImages(filenames);
    masks = createMasks(filenames, images, config);

    std::cout << "Size of images array:" << images.size() << "\n";

    if (images.size() == masks.size()) {
        std::cout << "Masks are done";
    }
    else {
        std::cout << "SMTH went wrong";
    }

    cv::imshow("Image", images[1]);
    cv::imshow("Mask", masks[1]);
    cv::waitKey(0);

    for (auto& image : images) {
        image.release();
    }
    for (auto& mask : masks) {
        mask.release();
    }

    return 0;
}