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

        cv::Mat temp;
        cv::fastNlMeansDenoising(blur, temp, hParam);

        result = temp;
    }

    return result;
}

int main() {

    std::vector<cv::Mat> images;
    std::vector<cv::Mat> masks;
    std::vector<std::string> filenames;
    
    filenames = listFiles("");

    for (auto fn : filenames) {
        cv::Mat image = cv::imread(fn);
        images.push_back(image);

        cv::Mat gray;
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

        // удаление шума
        cv::Mat denoised = denoiseImage(gray, 2, 10, cv::Size(5, 5));

        // маска
        cv::Mat mask;
        cv::threshold(denoised, mask, 128, 255, cv::THRESH_OTSU);

        masks.push_back(mask);
    }

    //cv::imshow("Image", image);

    //cv::imshow("Mask", mask);

    //cv::waitKey(0);

    return 0;
}
