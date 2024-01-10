#include <ImageProcessor/ImageProcessor.hpp>
#include <iostream>

int main(int argc, char* argv[]) {

    cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << "<config_file_path>" << std::endl;
        return 1;
    }

    std::string inputPicsFolder = "../pics/images";
    std::string groundTruthFolder = "../pics/ground";
    ImageProcessor::setConfigFileDirectory(argv[1]);

    if (!ImageProcessor::configExists()) {
        std::cerr << "Error: The configuration file does not exist." << std::endl;
        return 1;
    }

    ImageProcessor::Settings settings = ImageProcessor::readConfig();

    auto filenames = ImageProcessor::listFiles(inputPicsFolder);
    auto images = ImageProcessor::loadImages(filenames);

    auto masks = ImageProcessor::createMasks(images, settings);


    fs::path resultsFolder = fs::path(inputPicsFolder).parent_path() / "results";
    if (!fs::exists(resultsFolder)) {
        fs::create_directory(resultsFolder);
    }

    for (int i = 0; i < masks.size(); i++) {
        std::string filename = fs::path(filenames[i]).filename().string();
        cv::Mat& mask = masks[i];
        std::string outputFilename = (resultsFolder / fs::path(filename).stem()).string() + "_mask.png";
        cv::imwrite(outputFilename, mask);

        // Загрузка эталонной маски
        std::string groundTruthFilename = groundTruthFolder + "/" + filename;
        cv::Mat groundTruth = cv::imread(groundTruthFilename, cv::IMREAD_GRAYSCALE);
        if (!groundTruth.empty()) {
            ImageProcessor::SegmentationQuality quality = ImageProcessor::evaluateSegmentation(groundTruth, mask);
            std::cout << "Evaluation for " << filename << ":" << std::endl;
            std::cout << " Dice Coefficient: " << quality.diceCoefficient << std::endl;
            std::cout << " Accuracy: " << quality.accuracy << std::endl;
        }
        else {
            std::cerr << "Error: Ground truth file not found or invalid for " << filename << std::endl;
        }

        mask.release();
    }

    for (auto& image : images) {
        image.release();
    }

    std::cout << "Image processing completed using config file: " << argv[1] << std::endl;
    return 0;
}