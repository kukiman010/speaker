#ifndef COMMAND_H
#define COMMAND_H

#include <QString>
#include <QDebug>
#include <list>
#include <memory>
#include <QDataStream>

//using namespace std;

enum commandAction
{
    CA_null = 0,
    CA_login,           // вход с проверкой подленности
    CA_sing_up,         // регистрация
    CA_sing_in,         // вход в систему
    CA_rename,          // изменить имя
    CA_find_users,      // поиск пользователей
    CA_chat_mess,       // сообщение из чата
    CA_win,             // победа
    CA_louse,           // поражение
    CA_mess_system,     // сообщение от системы
    CA_start_game,      // начало игры
    CA_reconect,        // переподключение
    CA_connect,         // подключение
    CA_connected,       // подключино
    CA_game_move,       //
    CA_client_id,       // вернуть id client
    CA_
};

enum commandRevers
{
    CR_null = 0,
    CR_status_connect,
    //        CR_game_move,
    CR_
};

enum commandSObj
{
    CSO_null = 0,
    CSO_login,
    CSO_pass,
    CSO_user,
    CSO_message,
    CSO_
};

struct simpleObject
{
    commandSObj         com_SO = CSO_null;
    commandRevers       com_R = CR_null;
    QString             object = "";
};

class Command
{
public:
    enum commandType
    {
        CT_null = 0,
        CT_storage,
        CT_users,
        CT_system,
        CT_error,
        CT_

    };

protected:
    commandAction               _action = CA_null;
    commandRevers               _revers = CR_null;
    commandType                 _type = CT_null;
    QString                     _user;
    // simpleObject                _info;
    // std::list<simpleObject>     _objects;
    uint                        _id;
    std::vector<uint>           _destination;


public:
    Command(){}

    inline void                             setAction(const commandAction ac)               {_action = ac;}
    inline commandAction                    getAction()                                     {return _action;}

    inline void                             setType(const commandType ty)                   {_type = ty;}
    inline commandType                      getType()                                       {return _type;}

    inline void                             setRevers(const commandRevers re)               {_revers = re;}
    inline commandRevers                    getRevers()                                     {return _revers;}

    inline void                             setUser(const QString user)                     {_user = user;}
    inline QString                          getUser()                                       {return _user;}

    // inline void                             setInfo(const simpleObject info)                {_info = info;}
    // inline simpleObject                     getInfo()                                       {return _info;}

    inline void                             setId(const uint id)                            {_id = id;}
    inline uint                             getId()                                         {return _id;}

    inline void                             setDestination(const std::vector<uint> vec )    {_destination = vec;}
    inline std::vector<uint>                getDestination()                                {return _destination;}

    // inline void                             addObject(const simpleObject obj)               {_objects.push_back(obj);}
    // inline void                             addObject(const std::list<simpleObject> &L_obj) {_objects.insert( _objects.end(), L_obj.begin(), L_obj.end() );}
    // inline std::list<simpleObject>          get_object()                                    {return _objects;}

    friend QDataStream & operator <<( QDataStream &stream, Command &command );
    friend QDataStream & operator >>( QDataStream &stream, Command &command );

    // friend QDataStream & operator <<( QDataStream &stream, simpleObject &sObjd );
    // friend QDataStream & operator >>( QDataStream &stream, simpleObject &sObj );



};

using Cmd =  QSharedPointer<Command>;
//using Cmd =  unique_ptr<Command>;
//#define Cmd unique_ptr<Command>

#endif // COMMAND_H
