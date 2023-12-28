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
* @brief Класс, предоставляющий возможности фильтрации и бинаризации изображения.
*/
class ImageProcessor {
public:
    /**
    * @brief Структура, содержащая настройки обработки изображений.
    */
    struct Settings {
        //! Порог бинаризации для метода бинаризации BINARY
        int threshold = 128;
        //! Количество итераций фильтрации
        int filterIterations = 2;
        //! Параметр, используемый для метода фильтрации NLM 
        int filterhParam = 10;
        //! Размер ядра, используется для шумоподавления
        int ksize = 5;
        //! Множитель k для рассчета средней яркости в методе thresholdMeanStdDev
        int BDC = 0;
        /**
        * @brief Доступные для использования фильтры
        */
        enum Filter {
            NLM,
            BILATERAL,
            NONE
        };
        //! Значение по умолчанию NONE для фильтрации
        Filter filter = NONE;
        /**
        * @brief Доступные для использования методы бинаризации
        */
        enum ThresholdMethod {
            BINARY,
            OTSU,
            MEAN_STD_DEV,
            KAPUR
        };
        //! Значение по умолчанию BINARY для бинаризации
        ThresholdMethod thresholdMethod = BINARY;
    };

    /**
    * @brief Устанавливает путь до конфигурационного файла
    * @param path Путь до папки, в которой будет храниться конфигурационный файл
    */
    static void setConfigFilePath(const std::string& path);

    static void setConfigFileDirectory(const std::string& path);

    /**
    * @brief Проверяет есть ли конфигурационный файл или нет
    */
    static bool configExists();
    /**
    * @brief Создает конфигурационный файл с настройками по умолчанию
    */
    static void createDefaultConfig();
    /**
    * @brief Читает конфигурационный файл
    * @return Настройки из конфигурационного файла
    */
    static Settings readConfig();
    /**
    * @brief Обновляет конфигурационный файл
    * @param newSettings Обновленная структура настроек
    */
    static void updateConfig(const Settings& newSettings);
    static std::vector<std::string> listFiles(const std::string& folder);
    static std::vector<cv::Mat> loadImages(const std::vector<std::string>& filenames);

    /**
    * @brief Создает маски изображений
    * @param images вектор с исходными изображениями
    * @param settings настройки из конфигурационного файла
    * @return Вектор масок изображений в желто-фиолетвом цвете
    */
    static std::vector<cv::Mat> createMasks(std::vector<cv::Mat>& images, Settings settings);

    /**
* @brief Преобразует строку в метод бинаризации
* @param method Тип метода бинаризации
* @return Строковое представление фильтра
*/
    static std::string thresholdMethodToString(Settings::ThresholdMethod method);

    /**
    * @brief Преобразует фильтр в строку
    * @param filter Тип фильтра
    * @return Строковое представление фильтра
    */
    static std::string filterToString(Settings::Filter filter);

    /**
    * @brief Преобразует строку в метод бинаризации
    * @param thresholdMethod Строка-идентификатор метода
    * @return Метод бинаризации
    */
    static Settings::ThresholdMethod stringToThresholdMethod(const std::string& thresholdMethod);

    /**
    * @brief Преобразует строку в тип фильтра
    * @param filter Строка-идентификатор фильтра
    * @return Тип фильтра
    */
    static Settings::Filter stringToFilter(const std::string& filter);

private:
    //! Путь до конфигурационного файла
    static fs::path CONFIG_FILE_PATH;

    /**
    * @brief Сериализация настроек в JSON
    * @param j JSON объект
    * @param settings Настройки
    */
    friend void to_json(json& j, const Settings& settings);

    /**
    * @brief Десериализация настроек из JSON
    * @param j JSON объект
    * @param settings Настройки
    */
    friend void from_json(const json& j, Settings& settings);

    /**
    * @brief Вычисляет суммативную функцию распределения гистограммы
    * @param hist Гистограмма
    * @return Вектор значений СФРГ
    */
    static std::vector<float> cumulativeSum(const cv::Mat& hist);

    /**
    * @brief Метод бинаризации с помощью среднего и стандартного отклонения
    * @param src Входное изображение
    * @param k Коэффициент
    * @return Маска
    */
    static cv::Mat thresholdMeanStdDev(const cv::Mat& src, double k);

    /**
    * @brief Метод бинаризации Капура
    * @param src Входное изображение
    * @return Маска
    */
    static cv::Mat thresholdKapur(const cv::Mat& src);

    /**
    * @brief Фильтрация изображения билатеральным фильтром
    * @param img Входное изображение
    * @param iterations Количество итераций
    * @param ksize Размер ядра Гауссовского фильтра
    * @param sigma Параметр для билатерального фильтра
    * @return Отфильтрованное изображение
    */
    static cv::Mat denoiseBilateral(const cv::Mat& img, int iterations, int ksize, double sigma);
    /**
    * @brief Фильтрация изображения фильтром NLM
    * @param img Входное изображение
    * @param iterations Количество итераций
    * @param hParam Параметр NLM фильтра
    * @param ksize Размер ядра Гауссовского фильтра
    * @return Отфильтрованное изображение
    */
    static cv::Mat denoiseImageNLM(const cv::Mat& img, int iterations, int hParam, int ksize);
};

#endif