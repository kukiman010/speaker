#include "abstractcommand.h"

#include "ifactory.h"
#include "source.h"

AbstractCommand::AbstractCommand() :
    _commandDestination(DESTINATION_UNKNOWN),
    _infoFlag(0),
    _priority(0.0f),
    _finished(false)
{
    PRINT_COMMAND_DEBUG

    qRegisterMetaType< CmdPtr >( "CmdPtr" );
    qRegisterMetaType< QSharedPointer<AbstractCommand> >("QSharedPointer<AbstractCommand>");
}

AbstractCommand::AbstractCommand(QString userId) :
    _commandDestination(DESTINATION_UNKNOWN),
    _userId(userId),
    _infoFlag(0),
    _priority(0.0f),
    _finished(false)
{
    PRINT_COMMAND_DEBUG

    qRegisterMetaType< CmdPtr >( "CmdPtr" );
    qRegisterMetaType< QSharedPointer<AbstractCommand> >("QSharedPointer<AbstractCommand>");
}

AbstractCommand::AbstractCommand(const AbstractCommand &other) :
    _commandDestination(other._commandDestination),
    _userId(other._userId),
    _infoFlag(other._infoFlag),
    _priority(other._priority),
    _scenarionID(other._scenarionID),
    _finished(other._finished)
{
    PRINT_COMMAND_DEBUG

    foreach(AbstractObject *obj, other._objects)
        _objects.append(obj->copy());
}

AbstractCommand::~AbstractCommand()
{
    PRINT_COMMAND_DEBUG

    clearObjectList();
}

void AbstractCommand::clearObjectList()
{
    qDeleteAll( _objects );

    _objects.clear();
}

QDataStream & operator >>(QDataStream &stream, AbstractCommand *&command)
{
    if ( command )
    {
        delete command;
    }

    int type, size, destination, uniqueId, scenarioType;
    float priority;
    quint64 infoFlag;
    QString userId;

    stream >> type >> destination >> size >> userId >> infoFlag >> priority >> uniqueId >> scenarioType;

    IFactory * factory = IFactory::factory( CommandFactoryId );

    Factory<AbstractCommand> * fC = dynamic_cast<Factory<AbstractCommand>*>( factory );

    if ( !fC )
        return stream;

    command = fC->create( type );

    if ( !command )
        return stream;

    // command->setArmId( armId );
    command->setUserId(userId);
    command->setDestination( static_cast<AbstractCommand::CommandDestination>( destination ) );
    command->_infoFlag = infoFlag;
    command->_priority = priority;
    ScenarioID scID(uniqueId, (ScenarioType) scenarioType);
    command->setScenarioID(scID);

    command->fromDataStream( stream );

    for ( int i(0); i < size; ++i )
    {
        AbstractObject * obj = nullptr;
        stream >> obj;
        command->_objects.push_back( obj );
    }

    return stream;
}

QDataStream & operator <<(QDataStream &stream, AbstractCommand *&command)
{
    stream << static_cast<int>( command->commandType() );
    stream << static_cast<int>( command->_commandDestination );
    stream << static_cast<int>( command->_objects.size() );
    // stream << static_cast<int>( command->armId() );
    stream << command->userId();
    stream << command->_infoFlag;
    stream << command->_priority;
    stream << command->scenarionID().uniqueId;
    stream << command->scenarionID().scenarioType;

    command->toDataStream( stream );

    ObjectList::const_iterator it = command->_objects.constBegin();

    int i = 1;

    for ( ; it != command->_objects.constEnd(); ++it )
    {
        stream << (*it);
        ++i;
    }

    return stream;
}

