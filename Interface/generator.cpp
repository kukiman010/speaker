#include "generator.h"

int Generator::scenarioIdUnique = 0;

Generator::Generator()
{

}


ScenarioID Generator::createID(ScenarioType type) const
{
    QMutexLocker creator_locker(&mtx);
    scenarioIdUnique++;
    ScenarioID id;
    id.uniqueId = scenarioIdUnique;
    id.scenarioType = type;
    return id;
}
