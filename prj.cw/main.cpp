#include <ImageProcessor/ImageProcessor.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <config_folder_path> <input_folder>" << std::endl;
        return 1;
    }

    cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

    fs::path configPath_args = argv[1];
    std::string configPath = configPath_args.string();

    fs::path inputFolder_args = argv[2];
    std::string inputFolder = inputFolder_args.string();

    std::string outputFolder = (inputFolder_args.parent_path().parent_path() / "output/").string();


    // Set the config file path
    ImageProcessor::setConfigFilePath(configPath);

    if (!ImageProcessor::configExists()) {
        ImageProcessor::createDefaultConfig();
    }

    auto filenames = ImageProcessor::listFiles(inputFolder);
    auto images = ImageProcessor::loadImages(filenames);

    // List of available presets
    std::vector<ImageProcessor::Settings> presets = {
        // Preset 1 - MEAN_STD_DEV threshold with NLM denoising
        {
            .threshold = 128,
            .filterIterations = 2,
            .filterhParam = 10,
            .ksize = 5,
            .BDC = 0,
            .filter = ImageProcessor::Settings::NLM,
            .thresholdMethod = ImageProcessor::Settings::MEAN_STD_DEV
        },
        // Preset 2 - KAPUR threshold with Bilateral denoising
        {
            .threshold = 0,
            .filterIterations = 2,
            .filterhParam = 10,
            .ksize = 5,
            .BDC = 0,
            .filter = ImageProcessor::Settings::BILATERAL,
            .thresholdMethod = ImageProcessor::Settings::KAPUR
        },
        
        //Preset 3 - BINARY threshold whith Bilateral denoising
        {
            .threshold = 128,
            .filterIterations = 2,
            .filterhParam = 10,
            .ksize = 5,
            .BDC = 0,
            .filter = ImageProcessor::Settings::BILATERAL,
            .thresholdMethod = ImageProcessor::Settings::BINARY
        },
    };

    if (!fs::exists(outputFolder)) {
        fs::create_directory(outputFolder);
        
    }

    for (int i = 0; i < presets.size(); ++i) {
        ImageProcessor::Settings& preset = presets[i];
        // Update configuration to current preset
        ImageProcessor::updateConfig(preset);

        // Process images and create masks
        auto masks = ImageProcessor::createMasks(images, preset);

        // Save masks to output folder, creating a subdirectory for each preset
        std::string presetFolder = outputFolder + "/preset_" + std::to_string(i + 1);
        if (!fs::exists(presetFolder)) {
            fs::create_directory(presetFolder);
        }

        for (int j = 0; j < masks.size(); j++) {
            std::string filename = fs::path(filenames[j]).filename().string();
            cv::Mat& mask = masks[j];
            std::string outputFilename = presetFolder + "/" + (fs::path(filename).stem().string() + "_mask.png");
            cv::imwrite(outputFilename, mask);
        }

        for (auto& mask : masks) {
            mask.release();
        }
    }

    for (auto& image : images) {
        image.release();
    }

    std::cout << "Image Processing Application completed" << std::endl;
    return 0;
}