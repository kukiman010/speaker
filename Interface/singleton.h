#ifndef SINGLETON_H
#define SINGLETON_H


template <class T, class D = T>
class Singleton
{
    friend D;

public:
    ~Singleton(){}
    static T& instance()
    {
        static D _SingletonInstance;
        return _SingletonInstance;
    }
private:
    Singleton(){}
    Singleton( const Singleton& other );
    Singleton& operator=(  const Singleton& other );

};

#endif // SINGLETON_H
