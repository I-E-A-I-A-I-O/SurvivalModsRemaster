#include "pch.h"
#include "Initialization.hpp"

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

    TriggerPedsData::ClearTriggerPeds();

    for (int i = 0; i < names.size(); i++)
    {
        EntityPosition pos = EntityPosition();
        pos.coords.x = positionX.at(i);
        pos.coords.y = positionY.at(i);
        pos.coords.z = positionZ.at(i);
        pos.heading = headings.at(i);
        std::string name = names.at(i);

        TriggerPedsData::names.push_back(name);
        TriggerPedsData::models.push_back(models.at(i));
        TriggerPedsData::blips.push_back(BLIPS::CreateForMissionTriggerPed(pos.coords, name.c_str()));
        TriggerPedsData::positions.push_back(pos);
        TriggerPedsData::peds.push_back(0);
        TriggerPedsData::starTime.push_back(0);
        TriggerPedsData::tasks.push_back(tasks.at(i));
        TriggerPedsData::timerActive.push_back(false);
        TriggerPedsData::killedFlags.push_back(false);
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

Ped INIT::SpawnTriggerPed(size_t index)
{
    EntityPosition pos = TriggerPedsData::positions.at(index);
    Hash model = LoadModel(TriggerPedsData::models.at(index).c_str());
    Ped handle = PED::CREATE_PED(0, model, pos.coords.x, pos.coords.y, pos.coords.z, pos.heading, false, true);
    ENTITY::SET_ENTITY_LOAD_COLLISION_FLAG(handle, true);
    UnloadModel(model);

    switch (model)
    {
        case 0x5E3DA4A4:
        case 0x15F8700D:
        case 0x616C97B9:
        case 0x72C0CAD2:
        case 0x8D8F1B10:
            PED::SET_PED_CONFIG_FLAG(handle, 155, false);
            PED::SET_PED_CONFIG_FLAG(handle, 42, true);
            PED::SET_PED_CONFIG_FLAG(handle, 301, true);
            break;
        default:
            break;
    }

    PED::SET_PED_RELATIONSHIP_GROUP_HASH(handle, Data::neutralRelGroup);
    WEAPON::GIVE_WEAPON_TO_PED(handle, eWeapon::WeaponPistol, 100, true, false);
    AI::TASK_START_SCENARIO_IN_PLACE(handle, (char*)TriggerPedsData::tasks.at(index).c_str(), 0, true);
    return handle;
}
