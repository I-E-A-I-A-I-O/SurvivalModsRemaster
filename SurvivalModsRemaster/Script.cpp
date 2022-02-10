#include "pch.h"
#include "Script.hpp"
#include "nlohmann/json.hpp"

std::vector<SurvivalAllies> TriggerPedsData::allies;
std::vector<std::string> TriggerPedsData::names;
std::vector<std::string> TriggerPedsData::models;
std::vector<std::string> TriggerPedsData::tasks;
std::vector<Ped> TriggerPedsData::peds;
std::vector<EntityPosition> TriggerPedsData::positions;
std::vector<Blip> TriggerPedsData::blips;
std::vector<bool> TriggerPedsData::timerActive;
std::vector<bool> TriggerPedsData::killedFlags;
std::vector<int> TriggerPedsData::starTime;
std::vector<int> TriggerPedsData::playerRel;
int Data::intermissionDuration;
Controls Data::infiniteWaveControl;
Controls Data::timedSurvivalControl;
Controls Data::cancelControl;
Controls Data::reloadTriggerPedsControl;
Controls Data::hardcoreSurvivalControl;
bool Data::showControls;
bool canStart;
int cancelStartTime;
int cancelCurrentTime;
Hash Data::enemiesRelGroup;
Hash Data::neutralRelGroup;
nlohmann::json j;
int Data::TPIndex;
bool tpPointsEnabled;
int playerId;
SurvivalModes currentMode = SurvivalModes::TenWaves;

struct TPPoint {
    float x;
    float y;
    float z;

    TPPoint(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

enum eMarkers {
    BunkerEntrance,
    BunkerExit,
    LabEntrance,
    LabExit
};

static std::vector<TPPoint> teleportPoints = std::vector<TPPoint>{
        TPPoint(1571.56f, 2225.47f, 77.32f),
        TPPoint(893.14f, -3245.89f, -99.25251f),
        TPPoint(456.766663f, 5571.864f, 780.1841f),
        TPPoint(244.57f, 6163.39f, -160.42f),
};

static std::vector<Blip> entranceBlips = std::vector<Blip>();

void SetAllies() {
    size_t size = TriggerPedsData::allies.size();
    std::string name = TriggerPedsData::names.at(Data::TPIndex);

    for (size_t i = 0; i < size; i++) {
        SurvivalAllies ally = TriggerPedsData::allies.at(i);

        if (ally.MissionID == name) {
            PED::SET_RELATIONSHIP_BETWEEN_GROUPS(1, Data::neutralRelGroup,
                                                 MISC::GET_HASH_KEY(ally.RelGroupName.c_str()));
            PED::SET_RELATIONSHIP_BETWEEN_GROUPS(1, MISC::GET_HASH_KEY(ally.RelGroupName.c_str()),
                                                 Data::neutralRelGroup);
        }
    }
}

void ClearAllies(bool all = false) {
    size_t size = TriggerPedsData::allies.size();
    std::string name = TriggerPedsData::names.at(Data::TPIndex);

    for (size_t i = 0; i < size; i++) {
        SurvivalAllies ally = TriggerPedsData::allies.at(i);

        if (ally.MissionID == name || all) {
            PED::SET_RELATIONSHIP_BETWEEN_GROUPS(3, Data::neutralRelGroup,
                                                 MISC::GET_HASH_KEY(ally.RelGroupName.c_str()));
            PED::SET_RELATIONSHIP_BETWEEN_GROUPS(3, MISC::GET_HASH_KEY(ally.RelGroupName.c_str()),
                                                 Data::neutralRelGroup);
        }
    }
}

void IsPlayerInMissionStartRange() {
    if (SURVIVAL::SurvivalData::IsActive || PLAYER::GET_PLAYER_WANTED_LEVEL(playerId) > 1
        || MISC::GET_MISSION_FLAG()) {
        Data::showControls = false;
        canStart = false;
        return;
    }

    Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
    size_t size = TriggerPedsData::peds.size();

    for (int i = 0; i < size; i++) {
        if (CALC::IsInRange_2(playerPosition, TriggerPedsData::positions.at(i).coords, 80.0f)) {
            Data::TPIndex = i;
            canStart = true;

            Ped ped = TriggerPedsData::peds.at(i);

            if (ped == 0) {
                Data::showControls = false;
                return;
            }

            Vector3 coords = ENTITY::GET_ENTITY_COORDS(ped, false);
            Data::showControls = CALC::IsInRange_2(playerPosition, coords, 4.5f) &&
                                 !PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false);
            return;
        }
    }

    Data::showControls = false;
    canStart = false;
}

void ReadConfig() {
    std::ifstream i("SurvivalsData\\config.json");
    i >> j;
    i.close();

    Data::intermissionDuration = j["Gameplay"]["IntermissionDuration"];
    tpPointsEnabled = j["Gameplay"]["TPMarkers"];
    Data::infiniteWaveControl = static_cast<Controls>(j["Controls"]["StartInfiniteWaves"]);
    Data::timedSurvivalControl = static_cast<Controls>(j["Controls"]["StartTimedSurvival"]);
    Data::cancelControl = static_cast<Controls>(j["Controls"]["CancelSurvival"]);
    Data::reloadTriggerPedsControl = static_cast<Controls>(j["Controls"]["ReloadTriggerPeds"]);
    Data::hardcoreSurvivalControl = static_cast<Controls>(j["Controls"]["StartHardcoreSurvival"]);

    Hash playerGroupHash = MISC::GET_HASH_KEY("PLAYER");
    Hash cougarGroupHash = MISC::GET_HASH_KEY("COUGAR");
    PED::ADD_RELATIONSHIP_GROUP("SURVIVAL_MISSION_ENEMIES_REL_GROUP", &Data::enemiesRelGroup);
    PED::ADD_RELATIONSHIP_GROUP("SURVIVAL_MISSION_TRIGGER_REL_GROUP", &Data::neutralRelGroup);
    PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, Data::enemiesRelGroup, playerGroupHash);
    PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, playerGroupHash, Data::enemiesRelGroup);
    PED::SET_RELATIONSHIP_BETWEEN_GROUPS(0, Data::enemiesRelGroup, cougarGroupHash);
    PED::SET_RELATIONSHIP_BETWEEN_GROUPS(0, cougarGroupHash, Data::enemiesRelGroup);
    PED::SET_RELATIONSHIP_BETWEEN_GROUPS(3, Data::neutralRelGroup, playerGroupHash);
    PED::SET_RELATIONSHIP_BETWEEN_GROUPS(3, playerGroupHash, Data::neutralRelGroup);

    INIT::LoadTriggerPeds();
}

void createTPBlips() {
    STREAMING::REQUEST_IPL("gr_case6_bunkerclosed");
    STREAMING::REQUEST_IPL("gr_grdlc_interior_placement_interior_0_grdlc_int_01_milo_");
    STREAMING::REQUEST_IPL("gr_grdlc_interior_placement_interior_1_grdlc_int_02_milo_");
    TPPoint coords = teleportPoints.at(eMarkers::BunkerEntrance);
    entranceBlips.push_back(BLIPS::Create(coords.x, coords.y, coords.z, 557, eBlipColor::BlipColorWhite, "Bunker"));
    coords = teleportPoints.at(eMarkers::LabEntrance);
    entranceBlips.push_back(BLIPS::Create(coords.x, coords.y, coords.z, 499, eBlipColor::BlipColorWhite, "Secret Lab"));
}

const char* getHelpText(size_t index) {
    switch (index) {
        case eMarkers::BunkerEntrance:
            return "Press ~INPUT_CONTEXT~ to enter the bunker.";
        case eMarkers::BunkerExit:
            return "Press ~INPUT_CONTEXT~ to exit the bunker.";
        case eMarkers::LabEntrance:
            return "Press ~INPUT_CONTEXT~ to enter the lab.";
        case eMarkers::LabExit:
            return "Press ~INPUT_CONTEXT~ to exit the lab.";
        default:
            return "INVALID INDEX";
    }
}

void toggleInterior(size_t index) {
    switch (index) {
        case eMarkers::BunkerEntrance: {
            TPPoint v = teleportPoints.at(eMarkers::BunkerExit);
            Interior i = INTERIOR::GET_INTERIOR_AT_COORDS(v.x, v.y, v.z);
            INTERIOR::DEACTIVATE_INTERIOR_ENTITY_SET(i, "bunker_style_a");
            INTERIOR::DEACTIVATE_INTERIOR_ENTITY_SET(i, "bunker_style_b");
            INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(i, "bunker_style_c");
            INTERIOR::DEACTIVATE_INTERIOR_ENTITY_SET(i, "standard_bunker_set");
            INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(i, "upgrade_bunker_set");
            INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(i, "upgrade_bunker_set_more");
            INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(i, "security_upgrade");
            INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(i, "Office_Upgrade_set");
            INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(i, "gun_range_lights");
            INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(i, "gun_schematic_set");
            INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(i, "gun_locker_upgrade");
            INTERIOR::REFRESH_INTERIOR(i);
            AUDIO::PLAY_SOUND_FRONTEND(-1, "Door_Open_Limit", "DLC_GR_Bunker_Door_Sounds", true);
            break;
        }
        case eMarkers::BunkerExit: {
            AUDIO::PLAY_SOUND_FRONTEND(-1, "Enter_Car_Ramp_Deploy", "DLC_GR_MOC_Enter_Exit_Sounds", true);
            break;
        }
        case eMarkers::LabEntrance: {
            TPPoint v = teleportPoints.at(eMarkers::LabExit);
            Interior i = INTERIOR::GET_INTERIOR_AT_COORDS(v.x, v.y, v.z);
            INTERIOR::PIN_INTERIOR_IN_MEMORY(i);
            AUDIO::PLAY_SOUND_FRONTEND(-1, "Door_Open_Limit", "DLC_XM_Silo_Secret_Door_Sounds", true);
            break;
        }
        case eMarkers::LabExit: {
            TPPoint v = teleportPoints.at(eMarkers::LabExit);
            Interior i = INTERIOR::GET_INTERIOR_AT_COORDS(v.x, v.y, v.z);
            INTERIOR::UNPIN_INTERIOR(i);
            AUDIO::PLAY_SOUND_FRONTEND(-1, "DOOR_CLOSE", "CABLE_CAR_SOUNDS", true);
            break;
        }
        default:
            break;
    }
}

void processMarkers() {
    size_t size = teleportPoints.size();
    
    for (size_t i = 0; i < size; i++) {
        TPPoint coords = teleportPoints.at(i);
        Vector3 v3Coords = Vector3();
        v3Coords.x = coords.x;
        v3Coords.y = coords.y;
        v3Coords.z = coords.z;
        Vector3 playerCoords = ENTITY::GET_ENTITY_COORDS(playerId, true);

        if (!CALC::IsInRange_2(v3Coords, playerCoords, 200))
            continue;

        GRAPHICS::DRAW_MARKER(1, coords.x, coords.y, coords.z, 0, 1, 0, 0, 0, 0, 2, 2, 2, 255, 255, 0, 100, false, false, 2, false, nullptr, nullptr, false);

        if (!CALC::IsInRange_2(v3Coords, playerCoords, 2))
            continue;

        SCREEN::ShowHelpText(getHelpText(i), true);

        if (PAD::IS_CONTROL_JUST_PRESSED(0, 51)) {
            CAM::DO_SCREEN_FADE_OUT(1000);
            WAIT(1200);
            toggleInterior(i);
            size_t index;

            if (i % 2 == 0)
                index = i + 1;
            else
                index = i - 1;

            coords = teleportPoints.at(index);
            ENTITY::SET_ENTITY_COORDS(playerId, coords.x, coords.y, coords.z + (i == 0 ? 1.0f : 0.0f), 1, 0, 0, 1);
            WAIT(2000);
            CAM::DO_SCREEN_FADE_IN(1000);
        }
    }
}

void ControlsWatch() {
    if (!SURVIVAL::SurvivalData::IsActive && !canStart) {
        if (PAD::IS_CONTROL_JUST_PRESSED(0, static_cast<int>(Data::reloadTriggerPedsControl))) {
            INIT::LoadTriggerPeds();
        }
    }

    if (Data::showControls) {
        if (PAD::IS_CONTROL_JUST_PRESSED(0, static_cast<int>(Data::infiniteWaveControl))) {
            currentMode = SurvivalModes::Endless;
            SCREEN::ShowNotification("Survival mode set to: Endless survival.");
        }

        if (PAD::IS_CONTROL_JUST_PRESSED(0, static_cast<int>(Data::timedSurvivalControl))) {
            currentMode = SurvivalModes::Timed;
            SCREEN::ShowNotification("Survival mode set to: Timed survival.");
        }

        if (PAD::IS_CONTROL_JUST_PRESSED(0, static_cast<int>(Data::hardcoreSurvivalControl))) {
            currentMode = SurvivalModes::Hardcore;
            SCREEN::ShowNotification("Survival mode set to: Hardcore survival.");
        }
    } else {
        if (SURVIVAL::SurvivalData::IsActive) {
            if (PAD::IS_CONTROL_JUST_PRESSED(0, static_cast<int>(Data::cancelControl))) {
                cancelStartTime = MISC::GET_GAME_TIMER();
            }

            if (PAD::IS_CONTROL_PRESSED(0, static_cast<int>(Data::cancelControl))) {
                cancelCurrentTime = MISC::GET_GAME_TIMER();
                if (cancelCurrentTime - cancelStartTime >= 3000) {
                    SURVIVAL::QuitSurvival(false);
                }
            }
        }
        else {
            if (currentMode != SurvivalModes::TenWaves) {
                currentMode = SurvivalModes::TenWaves;
                SCREEN::ShowNotification("Survival mode set to: Ten Waves");
            }
        }
    }
}

bool KilledByPlayer(Ped ped) {
    int player = PLAYER::PLAYER_PED_ID();
    Entity killer = PED::GET_PED_SOURCE_OF_DEATH(ped);

    if (killer == player || killer == 0)
        return true;

    return false;
}

void ProcessTriggerPeds() {
    size_t size = TriggerPedsData::timerActive.size();

    for (size_t i = 0; i < size; i++) {
        bool active = TriggerPedsData::timerActive.at(i);

        if (!active) {
            if (!SURVIVAL::SurvivalData::IsActive) {
                Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
                EntityPosition TPPos = TriggerPedsData::positions.at(i);
                bool inRange = CALC::IsInRange_2(playerPosition, TPPos.coords, 80.0f);
                Ped ped = TriggerPedsData::peds.at(i);

                if (ped == 0 && canStart) {
                    if (inRange && !TriggerPedsData::killedFlags.at(i)) {
                        TriggerPedsData::peds.at(i) = INIT::SpawnTriggerPed(i);
                        SetAllies();
                    } else if (!inRange)
                        TriggerPedsData::killedFlags.at(i) = false;
                } else if (ped != 0) {
                    if (!inRange) {
                        ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);
                        TriggerPedsData::peds.at(i) = 0;
                        ClearAllies();
                    } else if (!canStart) {
                        ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);
                        TriggerPedsData::peds.at(i) = 0;
                        TriggerPedsData::killedFlags.at(i) = true;
                        ClearAllies();
                    } else if (ENTITY::IS_ENTITY_DEAD(ped, true)) {
                        if (KilledByPlayer(ped)) {
                            HUD::REMOVE_BLIP(&TriggerPedsData::blips.at(i));
                            TriggerPedsData::blips.at(i) = 0;
                            ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);
                            TriggerPedsData::peds.at(i) = 0;
                            SURVIVAL::StartMission(
                                    currentMode == SurvivalModes::Endless,
                                    currentMode == SurvivalModes::Timed,
                                    currentMode == SurvivalModes::Hardcore
                                    );
                            currentMode = SurvivalModes::TenWaves;
                        } else {
                            ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);
                            TriggerPedsData::peds.at(i) = 0;
                            TriggerPedsData::killedFlags.at(i) = true;
                        }

                        ClearAllies();
                    }
                }
            }

            continue;
        }

        if (MISC::GET_GAME_TIMER() - TriggerPedsData::starTime.at(i) < 300000) {
            continue;
        }

        TriggerPedsData::timerActive.at(i) = false;
        TriggerPedsData::blips.at(i) = BLIPS::CreateForMissionTriggerPed(TriggerPedsData::positions.at(i).coords,
                                                                         TriggerPedsData::names.at(i).c_str());
    }
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

int main() {
    while (DLC::GET_IS_LOADING_SCREEN_ACTIVE()) {
        WAIT(0);
    }

    if (SURVIVAL::SurvivalData::IsActive) {
        OnAbort();
    }

    ReadConfig();

    if (tpPointsEnabled)
        createTPBlips();

    while (true) {
        playerId = PLAYER::PLAYER_PED_ID();
        IsPlayerInMissionStartRange();
        ProcessTriggerPeds();

        if (SURVIVAL::SurvivalData::IsActive) {
            if (SURVIVAL::SurvivalData::Started) {
                SURVIVAL::ProcessSurvival();

                if (PLAYER::IS_PLAYER_DEAD(0)) {
                    SURVIVAL::QuitSurvival(true);
                }
            }
        }
        else
            processMarkers();

        ControlsWatch();
        WAIT(0);
    }
}

#pragma clang diagnostic pop

void ScriptMain() {
    srand(GetTickCount64());
    main();
}

void OnAbort() {
    ClearAllies(true);
    PLAYER::SET_PLAYER_CONTROL(playerId, true, 0);
    TASK::CLEAR_PED_TASKS(PLAYER::PLAYER_PED_ID());
    PLAYER::SET_DISPATCH_COPS_FOR_PLAYER(playerId, true);
    TriggerPedsData::ClearTriggerPeds();

    if (SURVIVAL::SurvivalData::IsActive) {
        SURVIVAL::ScriptQuit();
    }
    
    if (!entranceBlips.empty()) {
        for (Blip& blip : entranceBlips) {
            HUD::REMOVE_BLIP(&blip);
        }
        
        entranceBlips.clear();
    }
}
