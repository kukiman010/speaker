#include "scenario.h"
#include "source.h"

Scenario::Scenario()
{

}

Scenario::Scenario(ScenarioID id)
{
    _scenarionID = id;
}

Scenario::Scenario(const Scenario &other) :
    AbstractCommand(other)
{
    foreach(const CmdPtr &cmdPtr, other._cmdStack) {
        _cmdStack.push_back(TFuncs::make_shared(cmdPtr->copy()));
    }
}

AbstractCommand::CommandType Scenario::commandType() const
{
    return C_Scenario;
}

Scenario *Scenario::copy() const
{
    return new Scenario(*this);
}

ScenarioType Scenario::scanarioType() const
{
    return _scenarionID.scenarioType;
}

bool Scenario::isEmpty() const
{
    return _cmdStack.empty();
}

bool Scenario::nextCommand(CmdPtr &cmd)
{
    if (_cmdStack.empty())
        return false;

    cmd = _cmdStack.front();

    _cmdStack.pop_front();

    return true;
}

void Scenario::pushCmd(CmdPtr cmd)
{
    cmd->setScenarioID(_scenarionID);
    cmd->setFinished(false);
    _cmdStack.push_back(cmd);
}

void Scenario::toDataStream(QDataStream &stream) const
{
    stream << _scenarionID.uniqueId << _scenarionID.scenarioType;
    int cmdSize = _cmdStack.size();
    stream << cmdSize;

    //  WRITE CMD
    std::deque<CmdPtr>::const_iterator it1 = _cmdStack.begin();
    for (; it1 != _cmdStack.end(); ++it1)
    {
        AbstractCommand *cmd = (*it1).data();
        stream << cmd;
    }
}

void Scenario::fromDataStream(QDataStream &stream)
{
    int scType;
    int uniqueId;
    stream >> uniqueId >> scType;
    _scenarionID.uniqueId = uniqueId;
    _scenarionID.scenarioType = (ScenarioType) scType;

    int cmdSize;
    stream >> cmdSize;

    //  READ CMD
    for (int i(0); i < cmdSize; ++i)
    {
        AbstractCommand * cmd = 0;
        stream >> cmd;

        Q_ASSERT_X( cmd,
                    "Scenario::fromDataStream",
                    "Cannot create Command" );

        _cmdStack.push_back(CmdPtr(cmd));
    }
}
