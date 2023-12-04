#include "ImageProcessor/ImageProcessor.hpp"

namespace fs = std::filesystem;

// Function to create a directory for test results if not exists
void createResultsDirectory(const std::string& path) {
    if (!fs::is_directory(path) || !fs::exists(path)) {
        fs::create_directory(path);
    }
}
//int argc, char* argv[]
int main() {
    //// Check if the path to the test images folder is provided as a command line argument
    //if (argc != 2) {
    //    std::cerr << "Usage: " << argv[0] << " <path_to_test_images_folder>" << std::endl;
    //    return 1;
    //}

    std::string testImagesFolder = "C:/Users/gav-y/source/repos/ConsoleApplication1/ConsoleApplication1/images";
    std::string resultsFolder = "C:/Users/gav-y/OneDrive/Рабочий стол/res/";

    //createResultsDirectory(resultsFolder);

    // Get the list of file names in the test images folder
    std::vector<std::string> testImageFilenames = ImageProcessor::listFiles(testImagesFolder);

    // Load images
    std::vector<cv::Mat> images = ImageProcessor::loadImages(testImageFilenames);

    std::vector<ImageProcessor::Settings> settingsList {
        // Configuration 1: NLM filter with BINARY threshold
        {
            128,    // threshold
            4,      // filterIterations
            10,     // filterhParam
            5,      // ksize
            0,    // BDC not used in BINARY thresholding
            ImageProcessor::Settings::NLM,             // filter
            ImageProcessor::Settings::BINARY           // thresholdMethod
        },

        // Configuration 2: BILATERAL filter with OTSU threshold
        {
            0,      // threshold not used in OTSU thresholding
            2,      // filterIterations
            10,    // filterhParam (sigmaSpace for bilateral)
            5,      // ksize
            0,    // BDC not used in OTSU thresholding
            ImageProcessor::Settings::BILATERAL,       // filter
            ImageProcessor::Settings::OTSU             // thresholdMethod
        },

        // Configuration 3: No filter, MEAN_STD_DEV threshold
        {
            0,      // threshold not used in MEAN_STD_DEV thresholding
            2,      // filterIterations
            10,      // filterhParam not used without filtering
            5,      // ksize not used without filtering
            0,    // BDC factor for MEAN_STD_DEV thresholding
            ImageProcessor::Settings::BILATERAL,            // filter
            ImageProcessor::Settings::MEAN_STD_DEV     // thresholdMethod
        },

        // Configuration 4: No filter, KAPUR threshold
        {
            0,      // threshold not used in KAPUR thresholding
            2,      // filterIterations
            10,      // filter hParam not used without filtering
            5,      // ksize not used without filtering
            0,    // BDC not used in KAPUR thresholding
            ImageProcessor::Settings::BILATERAL,            // filter
            ImageProcessor::Settings::KAPUR            // thresholdMethod
        }
    };

    // Processing images with different settings
    for (const auto& settings : settingsList) {
        std::vector<cv::Mat> masks = ImageProcessor::createMasks(images, settings);

        // Save masks with a naming convention showing the settings used
        for (size_t i = 0; i < masks.size(); ++i) {
            fs::path test_image_path(testImageFilenames[i]);
            std::string file_name = test_image_path.filename().replace_extension("").string(); // делает имя файла без расширения
            std::string outputPath = resultsFolder + file_name + "_" + ImageProcessor::thresholdMethodToString(settings.thresholdMethod) + ".png";
            cv::imwrite(outputPath, masks[i]);
        }
    }

    std::cout << "Test processing completed. Check the results folder for output masks." << std::endl;

    return 0;
}