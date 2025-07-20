#ifndef LANGUAGEDETECTOR_H
#define LANGUAGEDETECTOR_H

#include <QString>
#include <fasttext.h>
#include <sstream>

class FastTextLanguageDetector
{
public:
    FastTextLanguageDetector(const std::string& modelPath, float probabilityThreshold = 0.7f)
        : threshold(probabilityThreshold)
    {
        model.loadModel(modelPath);
    }

    QString detectLanguage(const QString& input)
    {
        std::vector<std::pair<fasttext::real, std::string>> predictions;

        std::istringstream in(input.toStdString());
        model.predictLine(in, predictions, 1, 0.0f);

        if (predictions.empty() || predictions[0].first < threshold)
            return QString();

        // fastText возвращает метки в формате "__label__en"
        std::string langLabel = predictions[0].second;
        const std::string prefix = "__label__";
        if (langLabel.find(prefix) == 0)
        {
            // std::cout << "Detected language: " << langLabel << "  Probability=" << predictions[0].first << std::endl;
            return QString::fromStdString(langLabel.substr(prefix.size()));
        }
        return QString();
    }

private:
    fasttext::FastText model;
    float threshold;
};


#endif // LANGUAGEDETECTOR_H
