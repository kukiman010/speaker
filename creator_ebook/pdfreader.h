#ifndef PDFREADER_H
#define PDFREADER_H

#include <QObject>
#include <QString>
#include <string>
#include <mupdf/fitz.h>
#include "book.h"

class PdfReader : public QObject
{
    Q_OBJECT
public:

    struct LineItem {
        std::string text;
        float x0;  // X-координата линии (верхний левый угол)
        float y0;  // Y-координата линии
    };


    PdfReader(QObject* parent = nullptr);
    ~PdfReader();

    bool                    openDocument(const QString& fileWay);
    void                    closeDocument();
    int                     pageCount() const                                   { return m_pageCount; }
    int                     textPages() const                                   { return m_textPages; }
    int                     imageOnlyPages() const                              { return m_imageOnlyPages; }

    QString                 extractTextFromPage(int pageNumber);
    LiteBook                extractTextAllPages();

    QList<int> findCorruptedPages();
    QString savePageAsImage(int pageNumber, const QString& outPathPrefix);




signals:
    void                    alert(const QString & title, const QString & text, QString type);


private:

    fz_context *            _ctx;
    fz_document *           _doc;
    int                     m_pageCount;
    int                     m_textPages;
    int                     m_imageOnlyPages;
    QString                 filePath;

    void                    analyzePages();
    static bool             compareLines(const LineItem &a, const LineItem &b);
    static std::string      utf32_to_utf8(int codepoint);
    QString                 extractOrderedTextLines(int pageNum);
};

#endif // PDFREADER_H
