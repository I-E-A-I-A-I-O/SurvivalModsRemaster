#include "pch.h"
#include "Initialization.hpp"

void INIT::LoadTriggerPeds()
{
    nlohmann::json js;
    std::ifstream i("SurvivalsData\\TriggerPeds.json");
    i >> js;
    i.close();

    std::vector<std::string> names = js["SurvivalNames"];
    std::vector<int> playerRelshps = js["RelationWithPlayer"];
    std::vector<std::string> models = js["Models"];
    std::vector<float> positionX = js["Positions"]["x"];
    std::vector<float> positionY = js["Positions"]["y"];
    std::vector<float> positionZ = js["Positions"]["z"];
    std::vector<float> headings = js["Headings"];
    std::vector<std::string> tasks = js["Tasks"];
    TriggerPedsData::ClearTriggerPeds();

    for (size_t n = 0; n < names.size(); n++)
    {
        EntityPosition pos = EntityPosition();
        pos.coords.x = positionX.at(n);
        pos.coords.y = positionY.at(n);
        pos.coords.z = positionZ.at(n);
        pos.heading = headings.at(n);
        std::string name = names.at(n);
        std::vector<std::string> allies = js["Allies"][name];

        for (const std::string& ally : allies)
        {
            SurvivalAllies sa = SurvivalAllies();
            sa.MissionID = name;
            sa.RelGroupName = ally;
            TriggerPedsData::allies.push_back(sa);
        }

        TriggerPedsData::names.push_back(name);
        TriggerPedsData::models.push_back(models.at(n));
        TriggerPedsData::blips.push_back(BLIPS::CreateForMissionTriggerPed(pos.coords, name.c_str()));
        TriggerPedsData::positions.push_back(pos);
        TriggerPedsData::peds.push_back(0);
        TriggerPedsData::starTime.push_back(0);
        TriggerPedsData::tasks.push_back(tasks.at(n));
        TriggerPedsData::timerActive.push_back(false);
        TriggerPedsData::killedFlags.push_back(false);
        TriggerPedsData::playerRel.push_back(playerRelshps.at(n));
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
    Hash modelHash = MISC::GET_HASH_KEY(modelName);
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
    Ped handle = PED::CREATE_PED(0, model, pos.coords, pos.heading, false, true);
    UnloadModel(model);
    ENTITY::SET_ENTITY_LOAD_COLLISION_FLAG(handle, true, true);
    float zCoord;
    MISC::GET_GROUND_Z_FOR_3D_COORD(pos.coords, &zCoord, 0, 0);
    ENTITY::SET_ENTITY_COORDS(handle, pos.coords.x, pos.coords.y, zCoord, 1, 0, 0, 1);

    switch (model)
    {
        case 0x585C0B52:
        case 0x3AE4A33B:
        case 0x5CDEF405:
        case 0xEDBC7546:
        case 0x26F067AD:
        case 0x7B8B434B:
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

    Hash pKey = MISC::GET_HASH_KEY("PLAYER");
    int rel = TriggerPedsData::playerRel.at(index);
    PED::SET_PED_RELATIONSHIP_GROUP_HASH(handle, Data::neutralRelGroup);
    PED::SET_RELATIONSHIP_BETWEEN_GROUPS(rel, Data::neutralRelGroup, pKey);
    PED::SET_RELATIONSHIP_BETWEEN_GROUPS(rel, pKey, Data::neutralRelGroup);
    WEAPON::GIVE_WEAPON_TO_PED(handle, eWeapon::WeaponPistol, 100, true, false);
    TASK::TASK_START_SCENARIO_IN_PLACE(handle, TriggerPedsData::tasks.at(index).c_str(), 0, true);
    return handle;
}
