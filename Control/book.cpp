#include "book.h"

// book::book() {}

Book::Book(const QString &_bookId, const QString &_title, const QString &_author)
    : bookId(_bookId), title(_title), author(_author), storagePath("books/" + _bookId)
{
}

// void Book::addMetadata(const QString &key, const QString &value)
// {
//     metadata[key] = value;
// }

// void Book::saveToDisk(const QString &basePath) const
// {
//     QDir().mkpath(basePath + "/" + bookId);
//     QJsonObject meta;
//     meta["book_id"] = bookId;
//     meta["title"] = title;
//     meta["author"] = author;

//     QJsonObject mdObj;
//     for (auto it = metadata.begin(); it != metadata.end(); ++it)
//         mdObj[it.key()] = it.value();
//     meta["metadata"] = mdObj;

//     QJsonArray chapterArr;
//     for (const Chapter& ch : chapters)
//         chapterArr.append(ch.toJson());
//     meta["chapters"] = chapterArr;

//     QFile f(basePath + "/" + bookId + "/meta.json");
//     if (f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
//         f.write(QJsonDocument(meta).toJson( QJsonDocument::Indented ));
//         f.close();
//     }
// }

// Book Book::loadFromDisk(const QString &bookId, const QString &basePath)
// {
//     QFile f(basePath + "/" + bookId + "/meta.json");
//     if (!f.open(QIODevice::ReadOnly)) throw std::runtime_error("File not found");
//     QJsonObject meta = QJsonDocument::fromJson(f.readAll()).object();
//     f.close();

//     Book book(meta["book_id"].toString(), meta["title"].toString(), meta["author"].toString());
//     QJsonObject mdObj = meta["metadata"].toObject();
//     for (const QString& k : mdObj.keys())
//         book.metadata[k] = mdObj[k].toString();

//     QJsonArray chaptersArray = meta["chapters"].toArray();
//     for (const auto& chVal : chaptersArray) {
//         Chapter ch = Chapter::fromJson(chVal.toObject(), book.bookId);
//         book.chapters[ch.chapterId] = ch;
//     }
//     return book;
// }

// Chapter &Book::addChapter(const QString &chapterId, const QString &title)
// {
//     if (chapters.contains(chapterId)) throw std::runtime_error("Chapter exists");
//     chapters[chapterId] = Chapter(chapterId, title);
//     return chapters[chapterId];
// }

// Sentence Book::addSentence(const QString &chapterId, const QString &text)
// {
//     if (!chapters.contains(chapterId)) throw std::runtime_error("No chapter");
//     Chapter& chapter = chapters[chapterId];
//     int position = chapter.size() + 1;
//     Sentence sent(bookId, chapterId, position, text);
//     chapter.addSentence(sent);
//     return sent;
// }

// QList<Sentence> Book::iterSentences() const
// {
//     QList<Sentence> all;
//     for (const Chapter& ch : chapters)
//         all << ch.sentences;
//     return all;
// }
