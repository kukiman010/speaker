#ifndef SCENARIO_H
#define SCENARIO_H

#include "abstractcommand.h"

#include <deque>

class Scenario : public AbstractCommand
{
public:
    Scenario();
    explicit Scenario(ScenarioID id);
    explicit Scenario(const Scenario &other);

    virtual CommandType                 commandType() const Q_DECL_OVERRIDE;
    virtual Scenario*                   copy() const Q_DECL_OVERRIDE;
    ScenarioType                        scanarioType() const;
    bool                                isEmpty() const;
    bool                                nextCommand( CmdPtr &cmd );
    void                                pushCmd( CmdPtr cmd );

    virtual void                        toDataStream( QDataStream & stream ) const Q_DECL_OVERRIDE;
    virtual void                        fromDataStream( QDataStream & stream ) Q_DECL_OVERRIDE;
protected:
    std::deque< CmdPtr >                _cmdStack;
};

#endif // SCENARIO_H
