#include<iostream>
#include<string>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <vector>
namespace fs = std::filesystem;


std::vector<std::string> listFiles(const std::string& folder) {

    std::vector<std::string> filenames;

    for (const auto& entry : fs::directory_iterator(folder)) {
        filenames.push_back(entry.path().string());
    }

    return filenames;
}


cv::Mat denoiseImage(const cv::Mat& img, int iterations, int hParam, cv::Size kernelSize) {

    cv::Mat result = img;

    for (int i = 0; i < iterations; i++) {

        cv::Mat blur;
        cv::GaussianBlur(result, blur, kernelSize, 0);

        // Фильтрация    
        cv::Mat temp;
        cv::fastNlMeansDenoising(blur, temp, hParam);

        result = temp;
    }

    return result;
}

std::vector<cv::Mat> loadImages(const std::vector<std::string>& filenames) {

    std::vector<cv::Mat> images;

    for (auto fn : filenames) {
        cv::Mat image = cv::imread(fn);
        images.push_back(image);
    }
    return images;
}

std::vector<cv::Mat> createMasks(const std::vector<std::string>& filenames, std::vector<cv::Mat>& images) {
        
    std::vector<cv::Mat> masks;

    for (auto image : images) { 
        cv::Mat gray;
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

        // удаление шума
        cv::Mat denoised = denoiseImage(gray, 2, 10, cv::Size(5, 5));

        // маска
        cv::Mat mask;
        cv::threshold(denoised, mask, 128, 255, cv::THRESH_OTSU);

        masks.push_back(mask);
    }
    return masks;
}

int main() {

    std::vector<cv::Mat> images;
    std::vector<cv::Mat> masks;
    std::vector<std::string> filenames;
    
    filenames = listFiles("./images");
    images = loadImages(filenames);
    masks = createMasks(filenames, images);

    std::cout << "Image's size:" << images.size() << "\n";

    if (images.size() == masks.size()) {
        std::cout << "Masks are done";
    }
    else {
        std::cout << "SMTH went wrong";
    }

    return 0;
}
