#include <ImageProcessor/ImageProcessor.hpp>


int main() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);
    ImageProcessor obj;
    std::vector<cv::Mat> images;
    std::vector<cv::Mat> masks;
    std::vector<std::string> filenames;

    if (!obj.configExists()) {
        obj.createDefaultConfig();
    }

    ImageProcessor::Settings config = obj.readConfig();
    config.thresholdMethod = ImageProcessor::Settings::BINARY;
    config.filter = ImageProcessor::Settings::BILATERAL;
    config.filterIterations = 5;
    config.ksize = 5;
    obj.updateConfig(config);
    obj.readConfig();
    filenames = obj.listFiles("C:/Users/gav-y/source/repos/ConsoleApplication1/ConsoleApplication1/images");
    images = obj.loadImages(filenames);
    masks = obj.createMasks(images, config);

    cv::imshow("Image", images[0]);
    cv::imshow("Mask", masks[0]);
    cv::waitKey(0);

    for (auto& image : images) {
        image.release();
    }
    for (auto& mask : masks) {
        mask.release();
    }

    std::cout << "Image Processing Application" << std::endl;
    return 0;
}