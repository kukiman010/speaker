#ifndef SOURCE_H
#define SOURCE_H

#include <list>
#include <QtCore>

#include <sys/stat.h>
#include "abiglobal.h"
#include "abstractobject.h"


static const QString dateTimeFormat = QString("yyyy-MM-dd hh:mm:ss");

enum FactoryId {
    WidgetFactoryId  =   1,
    ObjectFactoryId,
    CommandFactoryId,
    StorageFactoryId
};


namespace TFuncs {

    template <typename T >
    static QSharedPointer<T> make_shared( T* data );
    template <typename T >
    static QSharedPointer<T> make_shared();

    template <typename T >
    static QWeakPointer<T> make_weak( T * data );
    template <typename T >
    static QWeakPointer<T> make_weak( const QSharedPointer<T> &ptr );

    template <typename T>
    static int abstractObjectType();

    template <class T, typename D>
    static T * findClassByVariableInList( const std::list<T*> * const &_list,
                                          const D &var ,
                                          const D &(T::*_get)()const );
}

template < typename T >
static inline QSharedPointer<T> TFuncs::make_shared( T *data )
{
    return QSharedPointer<T>( data );
}
template < typename T >
static inline QSharedPointer<T> TFuncs::make_shared()
{
    return QSharedPointer<T>( new T() );
}

template <typename T >
static inline QWeakPointer<T> TFuncs::make_weak( T * data )
{
    return QWeakPointer<T>( data );
}

template <typename T >
static inline QWeakPointer<T> TFuncs::make_weak( const QSharedPointer<T> &ptr )
{
    return TFuncs::make_weak( ptr.data() );
}

template <typename T>
static inline int TFuncs::abstractObjectType()
{
    AbstractObject * obj = new T();

    int t = obj->type();

    delete obj;

    return t;
}

template <class T, typename D>
static inline T * TFuncs::findClassByVariableInList( const std::list<T*> * const &_list,
                                                     const D &var ,
                                                     const D &(T::*_get)() const )
{
    typedef typename std::list<T*>::const_iterator _it;
    for ( _it i = _list->begin(); i != _list->end(); ++i )
        if ( ((*i)->*_get)() == var )
            return (*i);

    return nullptr;
}


#endif
