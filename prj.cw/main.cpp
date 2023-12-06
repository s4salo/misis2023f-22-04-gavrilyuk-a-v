#include <ImageProcessor/ImageProcessor.hpp>


int main() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);
    //set the config file path
    ImageProcessor::setConfigFilePath("C:/Users/gav-y/source/repos/ConsoleApplication1/ConsoleApplication1");

    if (!ImageProcessor::configExists()) {
        ImageProcessor::createDefaultConfig();
    }

    auto filenames = ImageProcessor::listFiles("C:/Users/gav-y/source/repos/ConsoleApplication1/ConsoleApplication1/images");
    auto images = ImageProcessor::loadImages(filenames);

    ImageProcessor::Settings config = ImageProcessor::readConfig();
    config.thresholdMethod = ImageProcessor::Settings::KAPUR;
    config.filter = ImageProcessor::Settings::BILATERAL;
    config.filterIterations = 2;
    config.ksize = 5;
    ImageProcessor::updateConfig(config);

    // Process images and create masks
    auto masks = ImageProcessor::createMasks(images, config);

    cv::imshow("Image", images[0]);
    cv::imshow("Mask", masks[0]);
    cv::waitKey(0);

    for (auto& image : images) {
        image.release();
    }
    for (auto& mask : masks) {
        mask.release();
    }

    std::cout << "Image Processing Application completed" << std::endl;
    return 0;
}