#ifndef ABSTRACTOBJECT_H
#define ABSTRACTOBJECT_H

#include "abiglobal.h"

#include <QSharedPointer>
#include <QDataStream>
#include <QByteArray>
#include <QList>
#include <QObject>
#include <QMetaObject>
#include <QMetaType>
#include <vector>
#include <map>

class AbstractObject : public QObject
{

public:

    enum ObjectType {
            O_Undefined = 0,
            O_UserInfo = 1,
            O_GroupInfo,
            O_Mac,
            O_Test,
            O_ProtectedResource,
            O_Message,
            O_FileRule,
            O_OS,
            O_ArmType,
            O_Arm,
            O_Environment,
            O_Simple,
            O_MNI,
            O_MessageType,
            O_MessageSource,
            O_AideConfigure,
            O_AideType,
            O_FileObject,
            O_MniType,
            O_Backup,
            O_Config,
            O_Error,
            O_Source,
            O_Archive,
            O_ArchiveConf,
            O_Password,
            O_DionisComand
    };

    explicit AbstractObject( QObject * parent = nullptr );
    virtual ~AbstractObject();

    inline const ObjectType &           type() const    { return _type; }

    friend QDataStream & operator<<( QDataStream &stream, AbstractObject * obj );
    friend QDataStream & operator>>( QDataStream &stream, AbstractObject *& obj );

    virtual AbstractObject *            copy() const = 0;

protected:

    virtual void                        fromDataStream( QDataStream &stream ) = 0;
    virtual void                        toDataStream( QDataStream &stream ) const = 0;

    ObjectType                          _type;


};

typedef QList<AbstractObject*>                  ObjectList;
typedef QSharedPointer<AbstractObject>          ObjPtr;
typedef QList< QSharedPointer<AbstractObject> > ObjectPtrList;

void clear_object_list(ObjectList &ol);

Q_DECLARE_METATYPE(QSharedPointer<AbstractObject>)
Q_DECLARE_METATYPE(ObjectList)
Q_DECLARE_METATYPE(AbstractObject::ObjectType)

#endif
