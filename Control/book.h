#ifndef BOOK_H
#define BOOK_H

#include <QString>
#include <QList>
#include <QMap>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QDir>

class Chapter;
class Sentence;


class Book {
public:
    QString bookId, title, author;
    // QMap<QString, QString> metadata;
    // QMap<QString, Chapter> chapters;
    QString storagePath;

    Book(const QString& _bookId = QString(), const QString& _title = QString(), const QString& _author = QString());

    // void addMetadata(const QString& key, const QString& value) ;

    // void saveToDisk(const QString& basePath = "books") const ;
    // static Book loadFromDisk(const QString& bookId, const QString& basePath = "books");
    // Chapter& addChapter(const QString& chapterId, const QString& title = QString());
    // Sentence addSentence(const QString& chapterId, const QString& text);
    // QList<Sentence> iterSentences() const ;
};




#endif // BOOK_H
