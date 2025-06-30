#ifndef GENERATOR_H
#define GENERATOR_H

#include "scenario.h"
#include "singleton.h"
#include <QMutex>

class Generator : public Singleton<Generator>
{
public:
    Generator();
    Scenario * updComandFlag(const bool & value) const;

private:

    ScenarioID      createID(ScenarioType type) const;

    static int      scenarioIdUnique;
    mutable QMutex  mtx;
};

#define GENERATOR Singleton<Generator>::instance()

#endif // GENERATOR_H
