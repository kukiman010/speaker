#include "ifactory.h"

#include "stddef.h"

std::map<std::size_t, IFactory*> IFactory::_factoryMap;

IFactory::IFactory()
{

}

IFactory * IFactory::factory(const std::size_t &factId)
{
    if ( _factoryMap.find( factId ) == _factoryMap.end() )
        return NULL;

    return _factoryMap[factId];
}

void IFactory::registerFactory(const std::size_t &factId, IFactory *f)
{
    if ( _factoryMap.find( factId ) == _factoryMap.end() )
        _factoryMap.insert( std::make_pair( factId, f ) );
}

ICreator::~ICreator(){}
