#include "pch.h"
#include "Initialization.hpp"

std::map<std::string, SpawnData> INIT::InitData::missionTriggerPeds;

void INIT::LoadTriggerPeds()
{
    nlohmann::json js;
    std::ifstream i("SurvivalsData\\TriggerPeds.json");
    i >> js;
    i.close();

    std::vector<std::string> names = js["SurvivalNames"];
    std::vector<std::string> models = js["Models"];
    std::vector<float> positionX = js["Positions"]["x"];
    std::vector<float> positionY = js["Positions"]["y"];
    std::vector<float> positionZ = js["Positions"]["z"];
    std::vector<float> headings = js["Headings"];
    std::vector<std::string> tasks = js["Tasks"];

    if (!InitData::missionTriggerPeds.empty())
    {
        InitData::missionTriggerPeds.clear();
    }

    for (int i = 0; i < names.size(); i++)
    {
        InitData::missionTriggerPeds.insert(std::pair<std::string, SpawnData>(names.at(i), SpawnData(models.at(i), positionX.at(i), positionY.at(i), positionZ.at(i), headings.at(i), tasks.at(i))));
    }
}

void INIT::UnloadModel(Hash model)
{
    STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
}

void INIT::LoadModel(Hash model)
{
    STREAMING::REQUEST_MODEL(model);

    while (!STREAMING::HAS_MODEL_LOADED(model))
    {
        WAIT(50);
    }
}

Hash INIT::LoadModel(const char* modelName)
{
    Hash modelHash = GAMEPLAY::GET_HASH_KEY((char*)modelName);
    STREAMING::REQUEST_MODEL(modelHash);
    while (!STREAMING::HAS_MODEL_LOADED(modelHash))
    {
        WAIT(50);
    }
    return modelHash;
}

Ped INIT::SpawnTriggerPed(std::string id)
{
    SpawnData pedData = InitData::missionTriggerPeds.at(id);
    Hash model = LoadModel(pedData.modelName.c_str());
    Ped handle = PED::CREATE_PED(0, model, pedData.position.x, pedData.position.y, pedData.position.z, pedData.heading, false, true);
    UnloadModel(model);

    PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(handle, true);
    ENTITY::SET_ENTITY_INVINCIBLE(handle, true);
    BLIPS::CreateForMissionTriggerPed(handle, (char*)id.c_str());
    AI::TASK_START_SCENARIO_IN_PLACE(handle, (char*)pedData.taskName.c_str(), 0, true);

    return handle;
}

std::map<std::string, Ped> INIT::SpawnMissionTriggerPeds()
{
    LoadTriggerPeds();

    std::map<std::string, Ped> pedMap;

    for (const auto& [key, value] : InitData::missionTriggerPeds)
    {
        Hash model = LoadModel(value.modelName.c_str());
        Ped handle = PED::CREATE_PED(0, model, value.position.x, value.position.y, value.position.z, value.heading, false, true);
        UnloadModel(model);

        PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(handle, true);
        ENTITY::SET_ENTITY_INVINCIBLE(handle, true);
        BLIPS::CreateForMissionTriggerPed(handle, key.c_str());
        AI::TASK_START_SCENARIO_IN_PLACE(handle, (char*)value.taskName.c_str(), 0, true);

        pedMap.insert(std::pair<std::string, Ped>(key, handle));
    }

    return pedMap;
}
