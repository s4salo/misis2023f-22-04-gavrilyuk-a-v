#include<iostream>
#include<opencv2/opencv.hpp>
#include<string>
int main(int argc, const char* argv[]) {

    cv::Mat image = cv::imread("smth2.jpg");
    std::cout << image.channels();
    if (!image.data) {
        std::cout << "Image file not found\n";
        return 1;
    }

    //Подготовка изображения для нахождения контуров
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    cv::threshold(image, image, 128, 255, cv::THRESH_BINARY);

    //Нахождение контуров
    std::vector<std::vector<cv::Point> > contours;
    cv::Mat contourOutput = image.clone();
    cv::findContours(contourOutput, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);

    //Отрисовка контуров
    cv::Mat contourImage(image.size(), CV_8UC3, cv::Scalar(0, 0, 0)); 

    for (size_t idx = 0; idx < contours.size(); idx++) {
        cv::drawContours(contourImage, contours, idx, cv::Scalar(255, 0, 0));
    }

    cv::imshow("Input Image", image);
    cv::moveWindow("Input Image", 0, 0);
    cv::imshow("Contours", contourImage);
    cv::moveWindow("Contours", 200, 0);
    cv::waitKey(0);

    return 0;
}
