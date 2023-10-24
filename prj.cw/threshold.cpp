#include<iostream>
#include<string>
#include <opencv2/opencv.hpp>

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

    cv::Mat image = cv::imread("smth.jpg");

    if (image.empty()) {
        std::cout << "Could not read image" << std::endl;
        return 1;
    }

    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    cv::Mat denoised = denoiseImage(gray, 2, 10, cv::Size(5, 5));

    // маска
    cv::Mat mask;
    cv::threshold(denoised, mask, 128, 255, cv::THRESH_OTSU);  
    
    cv::imshow("Image", image);
    cv::imshow("Mask", mask);

    cv::waitKey(0);

    return 0;
}
