#ifndef ABIGLOBAL_H
#define ABIGLOBAL_H

#include <QtGlobal>
#include <QTime>
#include <QElapsedTimer>

#if __cplusplus < 201103L
    #ifndef nullptr
    #define nullptr NULL
    #endif
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    #if __cplusplus >= 201103L
        #define Q_DECL_OVERRIDE override
        #define Q_DECL_FINAL final
    #else
        #define Q_DECL_OVERRIDE
        #define Q_DECL_FINAL
    #endif
#endif

#ifdef PRINT_FUNC_INFO_ENABLE
#define PRINT_FUNC_INFO qDebug() << QTime::currentTime().toString() << Q_FUNC_INFO;
#else
#define PRINT_FUNC_INFO
#endif

#ifdef PRINT_COMMAND_DEBUG_ENABLE
#define PRINT_COMMAND_DEBUG qDebug() << QTime::currentTime().toString() << Q_FUNC_INFO;
#else
#define PRINT_COMMAND_DEBUG
#endif

#ifdef PRINT_CHECKTIME_ENABLE
#define CONCAT(x,y) x##y
#define CHECKTIME(x) \
    QElapsedTimer CONCAT(sb_, __LINE__); \
    CONCAT(sb_, __LINE__).start(); \
    x \
    qint64 elpsd = CONCAT(sb_, __LINE__).elapsed();\
    if (elpsd > CHECKTIME_MAX)\
        qDebug() << QTime::currentTime().toString() << "Long execution time:" << elpsd << "ms." << __FILE__ << "Line:" << __LINE__;
#else
#define CHECKTIME(x) x
#endif

#endif // ABIGLOBAL_H
