#ifndef BOOKCONVECTOR_H
#define BOOKCONVECTOR_H

#include <QObject>
#include <mupdf/fitz.h>

class BookConvector : public QObject
{
    Q_OBJECT
public:
    // struct TextItem {
    //     std::string text;
    //     float x0;  // Начало по оси X
    //     float y0;  // Начало по оси Y
    // };

    struct LineItem {
        std::string text;
        float x0;  // X-координата линии (верхний левый угол)
        float y0;  // Y-координата линии
    };


    explicit BookConvector(QObject *parent = nullptr);
    std::string getPageText(fz_context *ctx, fz_document *doc, int pageNumber);
    /*static*/ void extractTextFromPage(fz_context* ctx, fz_document* doc, int pageNum, std::string& output);

    static bool compareLines(const LineItem &a, const LineItem &b);
    static std::string utf32_to_utf8(int codepoint);
    std::string extractOrderedTextLines(fz_context *ctx, fz_document *doc, int pageNum);
    QStringList splitTextSentences(const QString& text);




public slots:
    void conver(QString filePath);

signals:
    void alert(const QString & title, const QString & text, QString type);

// signals:
};

#endif // BOOKCONVECTOR_H
