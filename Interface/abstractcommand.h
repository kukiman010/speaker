#ifndef ABSTRACTCOMMAND_H
#define ABSTRACTCOMMAND_H

#include <QByteArray>
#include <QList>
#include <QVariantMap>
#include <QSharedPointer>
#include <QDateTime>
// #include <set>

#include "abstractobject.h"
#include "scenarioid.h"

class AbstractCommand
{
public:

    enum CommandType {
            C_Undefined = 0,
            C_Users = 1,
            C_ProtRes,
            C_Message,
            C_DataBase,
            C_Scenario
    };

    enum CommandDestination {
            DESTINATION_UNKNOWN = 0,
            CD_Runner,
            CD_Storage
    };

    AbstractCommand();
    explicit AbstractCommand( QString userId );
    explicit AbstractCommand( const AbstractCommand &other );
    virtual ~AbstractCommand();

    virtual void                        toDataStream( QDataStream & stream ) const = 0;
    virtual void                        fromDataStream( QDataStream & stream ) = 0;

    virtual QString                     sayMyName() const { return "IStorage"; }

    virtual CommandType                 commandType() const = 0;
    virtual AbstractCommand*            copy() const = 0;

    inline void                         setDestination( const CommandDestination &dist )    { _commandDestination = dist; }
    inline const CommandDestination &   destination() const                                 { return _commandDestination; }

    inline void                         setPriority( const float &priority )                { _priority = priority; }
    inline float                        priority() const                                    { return _priority; }

    inline bool                         isFinished() const                                  { return _finished; }
    inline void                         setFinished(bool value)                             { _finished = value; }

    const ScenarioID&                   scenarionID() const                                 { return  _scenarionID; }
    void                                setScenarioID( ScenarioID value )                   { _scenarionID = value; }

    // inline quint32                      armId() const { return _armId; }
    // inline void                         setArmId( quint32 armId ) { _armId = armId; }
    inline QString                      userId() const                                      { return _userId; }
    inline void                         setUserId( QString userId )                         { _userId = userId; }

    inline quint64 &                    infoFlag()                                          { return _infoFlag; }
    inline int                          objectsCount() const                                { return _objects.size(); }

    inline void                         addObject( const ObjectList &list )                 { _objects.append( list );   }
    inline void                         addObject( AbstractObject * const &obj )            { _objects.push_back( obj ); }

    void                                clearObjectList();

    inline const ObjectList &           objectList() const                                  { return _objects; }

    friend QDataStream & operator <<( QDataStream &stream, AbstractCommand *& command );
    friend QDataStream &operator  >>( QDataStream &stream, AbstractCommand *& command );

protected:

    CommandDestination                  _commandDestination;

    ObjectList                          _objects;
    QString                             _userId;
    // quint32                             _armId;

    quint64                             _infoFlag;

    float                               _priority;
    ScenarioID                          _scenarionID;
    bool                                _finished;
};




typedef QSharedPointer<AbstractCommand>                   CmdPtr;
typedef QList<QSharedPointer<AbstractCommand> >           CmdList;
typedef QList<QSharedPointer<AbstractCommand> >::iterator CmdIterator;
typedef std::vector<AbstractCommand*>                     CmdVec;

Q_DECLARE_METATYPE(QSharedPointer<AbstractCommand>)

#endif // ABSTRACTCOMMAND_H
