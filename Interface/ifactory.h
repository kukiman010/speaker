#ifndef IFACTORY_H
#define IFACTORY_H

#include <QDebug>

class ICreator
{
public:
    ICreator(){}
    virtual ~ICreator();
};

template < typename T >
class TCreator : public ICreator
{
public:
    TCreator() : _data(NULL) {}
    virtual ~TCreator(){}

    inline T* create()
    {
        prepare();
        return _data;
    }

protected:

    virtual void prepare() = 0;

    T * _data;
};

template <typename T, class D>
class Creator : public TCreator<T>
{
public:
    Creator() { }
    ~Creator() { qDebug() << "~Creator";}

protected:
     void prepare() { this->_data = new D(); }
};

template < typename T > class Factory;

class IFactory
{
public:
    IFactory();
    virtual ~IFactory(){}

    template < typename T >
    static inline Factory<T> *  factory_cast( const std::size_t &factId )
    {
        return dynamic_cast< Factory<T> * > ( factory( factId ) );
    }

    static IFactory *           factory( const std::size_t &factId );

protected:

    void                        registerFactory( const std::size_t &factId, IFactory* f);

    static std::map<std::size_t, IFactory*> _factoryMap;

};

template <class D>
class Factory : public IFactory
{
public:

    Factory( const std::size_t &factId )
    {
        registerFactory( factId, this );
    }
    ~Factory(){}

    inline void                 addCreator ( const std::size_t &id, ICreator * creator )
    {
        if ( dynamic_cast< TCreator<D>* >( creator ) )
            if ( _map.find( id ) == _map.end() )
                _map.insert( std::make_pair( id, creator ) );
    }

    inline D *                  create( const std::size_t &id ) const
    {
        if ( _map.find( id ) == _map.end() )
            return 0;

        ICreator * cr = _map[id];
        TCreator<D> * td = dynamic_cast<TCreator<D>*>(cr);
        D * d = td->create();
        return d;
    }

    inline D * getObject (const int objId)
    {
        if ( _map.find(objId) == _map.end() )
            return 0;

       return dynamic_cast<D*>(_map[objId]);
    }

private:

    Factory(){}

    mutable std::map<std::size_t,ICreator*> _map;

};

#endif // IFACTORY_H
