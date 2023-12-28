#include <ImageProcessor/ImageProcessor.hpp>
#include <iostream>

int main(int argc, char* argv[]) {

    cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << "<config_file_path>" << std::endl;
        return 1;
    }

    std::string inputPicsFolder = "../pics";
    ImageProcessor::setConfigFileDirectory(argv[1]);

    if (!ImageProcessor::configExists()) {
        std::cerr << "Error: The configuration file does not exist." << std::endl;
        return 1;
    }

    ImageProcessor::Settings settings = ImageProcessor::readConfig();

    auto filenames = ImageProcessor::listFiles(inputPicsFolder);
    auto images = ImageProcessor::loadImages(filenames);

    // Process images and create masks using the settings from the configuration file
    auto masks = ImageProcessor::createMasks(images, settings);

    // Create 'results' directory at the same level as 'pics'
    fs::path resultsFolder = fs::path(inputPicsFolder).parent_path() / "results";
    if (!fs::exists(resultsFolder)) {
        fs::create_directory(resultsFolder);
    }

    // Save masks to the 'results' folder
    for (int i = 0; i < masks.size(); i++) {
        std::string filename = fs::path(filenames[i]).filename().string();
        cv::Mat& mask = masks[i];
        std::string outputFilename = (resultsFolder / fs::path(filename).stem()).string() + "_mask.png";
        cv::imwrite(outputFilename, mask);
        // Release the mask since it is no longer needed
        mask.release();
    }

    // Release images that are no longer needed
    for (auto& image : images) {
        image.release();
    }

    std::cout << "Image processing completed using config file: " << argv[1] << std::endl;
    return 0;
}