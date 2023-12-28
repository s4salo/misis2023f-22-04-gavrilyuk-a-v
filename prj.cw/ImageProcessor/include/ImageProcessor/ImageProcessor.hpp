#ifndef IMAGE_PROCESSOR_HPP
#define IMAGE_PROCESSOR_HPP

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <opencv2/core/utils/logger.hpp>

namespace fs = std::filesystem;
using json = nlohmann::json;
/**
* @brief �����, ��������������� ����������� ���������� � ����������� �����������.
*/
class ImageProcessor {
public:
    /**
    * @brief ���������, ���������� ��������� ��������� �����������.
    */
    struct Settings {
        //! ����� ����������� ��� ������ ����������� BINARY
        int threshold = 128;
        //! ���������� �������� ����������
        int filterIterations = 2;
        //! ��������, ������������ ��� ������ ���������� NLM 
        int filterhParam = 10;
        //! ������ ����, ������������ ��� ��������������
        int ksize = 5;
        //! ��������� k ��� �������� ������� ������� � ������ thresholdMeanStdDev
        int BDC = 0;
        /**
        * @brief ��������� ��� ������������� �������
        */
        enum Filter {
            NLM,
            BILATERAL,
            NONE
        };
        //! �������� �� ��������� NONE ��� ����������
        Filter filter = NONE;
        /**
        * @brief ��������� ��� ������������� ������ �����������
        */
        enum ThresholdMethod {
            BINARY,
            OTSU,
            MEAN_STD_DEV,
            KAPUR
        };
        //! �������� �� ��������� BINARY ��� �����������
        ThresholdMethod thresholdMethod = BINARY;
    };

    /**
    * @brief ������������� ���� �� ����������������� �����
    * @param path ���� �� �����, � ������� ����� ��������� ���������������� ����
    */
    static void setConfigFilePath(const std::string& path);

    static void setConfigFileDirectory(const std::string& path);

    /**
    * @brief ��������� ���� �� ���������������� ���� ��� ���
    */
    static bool configExists();
    /**
    * @brief ������� ���������������� ���� � ����������� �� ���������
    */
    static void createDefaultConfig();
    /**
    * @brief ������ ���������������� ����
    * @return ��������� �� ����������������� �����
    */
    static Settings readConfig();
    /**
    * @brief ��������� ���������������� ����
    * @param newSettings ����������� ��������� ��������
    */
    static void updateConfig(const Settings& newSettings);
    static std::vector<std::string> listFiles(const std::string& folder);
    static std::vector<cv::Mat> loadImages(const std::vector<std::string>& filenames);

    /**
    * @brief ������� ����� �����������
    * @param images ������ � ��������� �������������
    * @param settings ��������� �� ����������������� �����
    * @return ������ ����� ����������� � �����-��������� �����
    */
    static std::vector<cv::Mat> createMasks(std::vector<cv::Mat>& images, Settings settings);

    /**
* @brief ����������� ������ � ����� �����������
* @param method ��� ������ �����������
* @return ��������� ������������� �������
*/
    static std::string thresholdMethodToString(Settings::ThresholdMethod method);

    /**
    * @brief ����������� ������ � ������
    * @param filter ��� �������
    * @return ��������� ������������� �������
    */
    static std::string filterToString(Settings::Filter filter);

    /**
    * @brief ����������� ������ � ����� �����������
    * @param thresholdMethod ������-������������� ������
    * @return ����� �����������
    */
    static Settings::ThresholdMethod stringToThresholdMethod(const std::string& thresholdMethod);

    /**
    * @brief ����������� ������ � ��� �������
    * @param filter ������-������������� �������
    * @return ��� �������
    */
    static Settings::Filter stringToFilter(const std::string& filter);

private:
    //! ���� �� ����������������� �����
    static fs::path CONFIG_FILE_PATH;

    /**
    * @brief ������������ �������� � JSON
    * @param j JSON ������
    * @param settings ���������
    */
    friend void to_json(json& j, const Settings& settings);

    /**
    * @brief �������������� �������� �� JSON
    * @param j JSON ������
    * @param settings ���������
    */
    friend void from_json(const json& j, Settings& settings);

    /**
    * @brief ��������� ����������� ������� ������������� �����������
    * @param hist �����������
    * @return ������ �������� ����
    */
    static std::vector<float> cumulativeSum(const cv::Mat& hist);

    /**
    * @brief ����� ����������� � ������� �������� � ������������ ����������
    * @param src ������� �����������
    * @param k �����������
    * @return �����
    */
    static cv::Mat thresholdMeanStdDev(const cv::Mat& src, double k);

    /**
    * @brief ����� ����������� ������
    * @param src ������� �����������
    * @return �����
    */
    static cv::Mat thresholdKapur(const cv::Mat& src);

    /**
    * @brief ���������� ����������� ������������� ��������
    * @param img ������� �����������
    * @param iterations ���������� ��������
    * @param ksize ������ ���� ������������ �������
    * @param sigma �������� ��� �������������� �������
    * @return ��������������� �����������
    */
    static cv::Mat denoiseBilateral(const cv::Mat& img, int iterations, int ksize, double sigma);
    /**
    * @brief ���������� ����������� �������� NLM
    * @param img ������� �����������
    * @param iterations ���������� ��������
    * @param hParam �������� NLM �������
    * @param ksize ������ ���� ������������ �������
    * @return ��������������� �����������
    */
    static cv::Mat denoiseImageNLM(const cv::Mat& img, int iterations, int hParam, int ksize);
};

#endif