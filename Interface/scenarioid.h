#ifndef SCENARIOID_H
#define SCENARIOID_H

enum ScenarioType {
    ST_undefine = -1,
    ST_getUsers,
    ST_resetUsers,
    ST_getTests,
    ST_getBackups,
    ST_addBackup,
    ST_updateBackup,
    ST_removeBackup,
    ST_updateProtRes,
    ST_addFileRules,
    ST_removeFileRules,
    ST_updCombatFlag
};

struct ScenarioID {
    ScenarioID() : uniqueId(-1), scenarioType(ST_undefine) {};
    ScenarioID(int uid, ScenarioType type) : uniqueId(uid), scenarioType(type) {};
    bool operator==(const ScenarioID &id) const {
        return uniqueId == id.uniqueId && scenarioType == id.scenarioType;
    }

    int uniqueId;
    ScenarioType scenarioType;
};

#endif // SCENARIOID_H
