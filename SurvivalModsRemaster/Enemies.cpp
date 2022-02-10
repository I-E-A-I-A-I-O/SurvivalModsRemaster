#include <main.h>
#include "pch.h"
#include "Enemies.hpp"

int ENEMIES::EnemiesData::currentAircraft;
int ENEMIES::EnemiesData::currentVehicles;
bool ENEMIES::EnemiesData::canSpawnMore;
bool ENEMIES::EnemiesData::limitReached;
int ENEMIES::EnemiesData::currentWaveSize;
int ENEMIES::EnemiesData::kills;
int ENEMIES::EnemiesData::currentDogCount;
bool jugSpawned;
bool dogLimitReached = false;
std::vector<Ped> footEnemies;
std::vector<Vehicle> enemyVehicles;
std::vector<Ped> deadEnemies;
JESUS::Jesus enemyJesus;
bool jesusSpawned;
Ped enemyJuggernaut;
std::vector<ENEMIES::Suicidal> suicidalEnemies;
bool suicidalLimitReached;

std::vector<Hash> alienWeapons =
{
    0x476BF155,
    0xB62D1F67
};

void ENEMIES::ResetCounters()
{
    EnemiesData::currentAircraft = 0;
    EnemiesData::currentVehicles = 0;
    EnemiesData::canSpawnMore = true;
    EnemiesData::limitReached = false;
    suicidalLimitReached = false;
    jesusSpawned = false;
    EnemiesData::currentDogCount = 0;
    dogLimitReached = false;
    jugSpawned = false;
    EnemiesData::currentWaveSize = 0;
    EnemiesData::kills = 0;
    enemyJuggernaut = 0;
}

void ENEMIES::ClearVectors()
{
    for (Ped enemy : footEnemies)
    {
        Blip blip = HUD::GET_BLIP_FROM_ENTITY(enemy);
        HUD::REMOVE_BLIP(&blip);
        ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&enemy);
    }

    for (Vehicle vehicle : enemyVehicles)
    {
        ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&vehicle);
    }

    for (Ped deadEnemy : deadEnemies)
    {
        ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&deadEnemy);
    }

    for (Suicidal suicidal : suicidalEnemies)
    {
        ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&suicidal.ped);
    }

    suicidalEnemies.clear();
    footEnemies.clear();
    enemyVehicles.clear();
    deadEnemies.clear();
}

int GetKillTime(Ped ped)
{
    Entity killer = PED::GET_PED_SOURCE_OF_DEATH(ped);

    if (killer != PLAYER::PLAYER_PED_ID())
    {
        return 0;
    }

    Hash model = ENTITY::GET_ENTITY_MODEL(ped);

    switch (model)
    {
    case 0x90EF5134:
        SCREEN::ShowNotification("Juggernaut: ~o~+20 seconds");
        return 15000;
    case 0xCE2CB751:
        SCREEN::ShowNotification("Jesus: ~o~+15 seconds");
        return 15000;
    case 0xAD340F5A:
    case 0x61D4C771:
        if (PED::GET_PED_MAX_HEALTH(ped) == 1250)
        {
            SCREEN::ShowNotification("Ragemode Sasquatch: ~o~+15 seconds");
            return 15000;
        }
        else
        {
            break;
        }
    case 0x9563221D:
        SCREEN::ShowNotification("Rottweiler: ~o~+8 seconds");
        return 8000;
    default:
        break;
    }

    if (FIRE::IS_ENTITY_ON_FIRE(ped))
    {
        SCREEN::ShowNotification("Burn!: ~r~+4 seconds");
        return 4000;
    }

    int lastDamagedBone;
    PED::GET_PED_LAST_DAMAGE_BONE(ped, &lastDamagedBone);

    switch (lastDamagedBone)
    {
        case eBone::SKEL_Head:
            SCREEN::ShowNotification("Headshot: ~g~+5 seconds");
            return 5000;
        case eBone::SKEL_Spine_Root:
        case eBone::SKEL_Spine0:
        case eBone::SKEL_Spine1:
        case eBone::SKEL_Spine2:
        case eBone::SKEL_Spine3:
            SCREEN::ShowNotification("Torso: ~y~+2.5 seconds");
            return 2500;
        case eBone::SKEL_L_UpperArm:
        case eBone::SKEL_R_UpperArm:
        case eBone::SKEL_L_Forearm:
        case eBone::SKEL_R_Forearm:
        case eBone::SKEL_L_Thigh:
        case eBone::SKEL_R_Thigh:
        case eBone::SKEL_L_Calf:
        case eBone::SKEL_R_Calf:
            SCREEN::ShowNotification("Limb: ~r~+1.25 seconds");
            return 1250;
        default:
            SCREEN::ShowNotification("Other: ~c~+1 second");
            return 1000;
    }
}

bool Contains(const std::vector<ENEMIES::Suicidal> &vector, const Ped &val, int *index) {
    for (size_t i = 0; i < vector.size(); i++)
    {
        if (vector.at(i).ped == val)
        {
            *index = i;
            return true;
        }
    }

    *index = 0;
    return false;
}

void RemoveDeadEnemies()
{
    for (size_t i = 0; i < footEnemies.size(); i++)
    {
        Ped body = footEnemies.at(i);

        if (!PED::IS_PED_DEAD_OR_DYING(body, 1))
        {
            if (PED::IS_PED_FLEEING(body) && !CALC::IsInRange_2(ENTITY::GET_ENTITY_COORDS(body, true), SURVIVAL::SpawnerData::location, 160))
            {
                ENTITY::SET_ENTITY_HEALTH(body, 0, 0);
            }
            else
            {
                continue;
            }
        }

        bool isJesus = ENTITY::GET_ENTITY_MODEL(body) == 0xCE2CB751;

        if (SURVIVAL::SurvivalData::timed)
        {
            SURVIVAL::SurvivalData::timedTimeLeft += GetKillTime(body);
        }

        if (jesusSpawned && isJesus)
        {
            jesusSpawned = false;
        }

        if (!PED::CAN_PED_RAGDOLL(body))
        {
            PED::SET_PED_CAN_RAGDOLL(body, true);
        }

        Blip blip = HUD::GET_BLIP_FROM_ENTITY(body);
        if (HUD::DOES_BLIP_EXIST(blip))
        {
            HUD::REMOVE_BLIP(&blip);
        }

        if (!jesusSpawned)
        {
            if (body == enemyJuggernaut)
            {
                enemyJuggernaut = 0;
            }

            int index;
            if (Contains(suicidalEnemies, body, &index))
            {
                if (!suicidalEnemies.at(index).exploded)
                    continue;

                suicidalEnemies.erase(suicidalEnemies.begin() + index);
            }

            ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&body);
            ENEMIES::EnemiesData::kills += 1;
        }
        else
        {
            int index;
            if (Contains(suicidalEnemies, body, &index))
            {
                if (!suicidalEnemies.at(index).exploded)
                    continue;

                ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&body);
                ENEMIES::EnemiesData::kills += 1;
                suicidalEnemies.erase(suicidalEnemies.begin() + index);
            } else
            {
                deadEnemies.push_back(body);
            }
        }

        footEnemies.erase(footEnemies.begin() + i);
        break;
    }

    if (!jesusSpawned && !deadEnemies.empty())
    {
        for (int body : deadEnemies)
        {
            if (body == enemyJuggernaut)
            {
                enemyJuggernaut = 0;
            }

            ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&body);
            ENEMIES::EnemiesData::kills += 1;
        }

        deadEnemies.clear();
    }
}

void ENEMIES::RemoveUnusedVehicles()
{
    bool vehicleDestroyed;
    int vehiclePassengers;

    for (size_t i = 0; i < enemyVehicles.size(); i++)
    {
        vehicleDestroyed = ENTITY::IS_ENTITY_DEAD(enemyVehicles.at(i), 1);
        vehiclePassengers = VEHICLE::GET_VEHICLE_NUMBER_OF_PASSENGERS(enemyVehicles.at(i), true, false);

        if (vehicleDestroyed || vehiclePassengers == 0)
        {
            BLIPS::DeleteBlipForEntity(enemyVehicles.at(i));

            if (vehicleDestroyed && SURVIVAL::SurvivalData::timed)
            {
                SURVIVAL::SurvivalData::timedTimeLeft += 10000;
                SCREEN::ShowNotification("Vehicle: ~p~+10 seconds");
            }

            ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&enemyVehicles.at(i));
            enemyVehicles.erase(enemyVehicles.begin() + i);
            return;
        }
        else if (vehiclePassengers > 0)
        {
            Ped ped = VEHICLE::GET_PED_IN_VEHICLE_SEAT(enemyVehicles.at(i), -1, 0);
            if (PED::IS_PED_DEAD_OR_DYING(ped, 1))
            {
                BLIPS::DeleteBlipForEntity(enemyVehicles.at(i));
                ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&enemyVehicles.at(i));
                enemyVehicles.erase(enemyVehicles.begin() + i);
                return;
            }
        }
    }
}

void ProcessSuicidals()
{
    for (ENEMIES::Suicidal &enemy : suicidalEnemies)
    {
        if (PED::IS_PED_DEAD_OR_DYING(enemy.ped, 1) && !enemy.exploded)
        {
            if (!enemy.timer)
            {
                int lastDamagedBone;
                PED::GET_PED_LAST_DAMAGE_BONE(enemy.ped, &lastDamagedBone);

                if (lastDamagedBone == eBone::SKEL_Head)
                {
                    Vector3 coords = ENTITY::GET_ENTITY_COORDS(enemy.ped, true);
                    FIRE::ADD_EXPLOSION(coords.x, coords.y, coords.z, eExplosionType::ExplosionTypeStickyBomb, 5.0f, true, false, 2.0f, false);
                    enemy.exploded = true;
                    continue;
                }

                enemy.deadCoords = ENTITY::GET_ENTITY_COORDS(enemy.ped, true);
                enemy.timeOfDeath = MISC::GET_GAME_TIMER();
                enemy.timer = true;
            } else
            {
                if (MISC::GET_GAME_TIMER() - enemy.timeOfDeath >= 2000)
                {
                    FIRE::ADD_EXPLOSION(enemy.deadCoords.x, enemy.deadCoords.y, enemy.deadCoords.z, eExplosionType::ExplosionTypeStickyBomb, 5.0f, true, false, 2.0f, false);
                    enemy.exploded = true;
                }
            }
        }
        else
        {
            Vector3 coords = ENTITY::GET_ENTITY_COORDS(enemy.ped, true);
            Vector3 playerCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);

            if (CALC::IsInRange_2(coords, playerCoords, 2.0f))
            {
                FIRE::ADD_EXPLOSION(coords.x, coords.y, coords.z, eExplosionType::ExplosionTypeStickyBomb, 5.0f, true, false, 2.0f, false);
                enemy.exploded = true;
            }
        }
    }
}

std::vector<Hash> GetWeapons(Hash pedModel)
{
    if (pedModel == 0x64611296)
    {
        return alienWeapons;
    }

    if (SURVIVAL::SurvivalData::hardcore)
        return SURVIVAL::SpawnerData::strongWeapons;

    switch (SURVIVAL::SurvivalData::CurrentWave)
    {
        case 1:
        case 2:
        case 3:
            return SURVIVAL::SpawnerData::weakWeapons;
        case 4:
        case 5:
        case 6:
        case 7:
            return SURVIVAL::SpawnerData::medWeapons;
        default:
            return SURVIVAL::SpawnerData::strongWeapons;
    }
}

void InitializeJesus(Ped ped)
{
    enemyJesus = JESUS::Jesus(ped);
    jesusSpawned = true;
    PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, Data::enemiesRelGroup);
    PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, true);
    PED::SET_PED_SUFFERS_CRITICAL_HITS(ped, false);
    PED::SET_PED_CAN_RAGDOLL(ped, false);
    PED::SET_PED_CONFIG_FLAG(ped, 281, false);
    PED::SET_PED_MAX_HEALTH(ped, 1000);
    ENTITY::SET_ENTITY_HEALTH(ped, 1000, 0);
    BLIPS::CreateForEnemyPed(ped, 305, "Jesus Christ");
}

void InitializeAnimal(Ped ped)
{
    BLIPS::CreateForEnemyPed(ped, 463, "Enemy Animal");
    PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, MISC::GET_HASH_KEY("COUGAR"));
    TASK::TASK_PUT_PED_DIRECTLY_INTO_MELEE(ped, PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0);
    ENEMIES::EnemiesData::currentDogCount += 1;

    if (ENEMIES::EnemiesData::currentDogCount >= 3)
    {
        dogLimitReached = true;
    }
}

void InitializeJuggernaut(Ped ped)
{
    enemyJuggernaut = ped;
    PED::SET_PED_SUFFERS_CRITICAL_HITS(ped, false);
    PED::SET_PED_CAN_RAGDOLL(ped, false);
    PED::SET_PED_CONFIG_FLAG(ped, 281, true);
    PED::SET_PED_MAX_HEALTH(ped, 1000);
    ENTITY::SET_ENTITY_HEALTH(ped, 1000, 0);
    BLIPS::CreateForEnemyPed(ped, 543, "Enemy Juggernaut");
    PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, Data::enemiesRelGroup);
    int accuracyModifier = SURVIVAL::SurvivalData::CurrentWave > 10 ? 10 : SURVIVAL::SurvivalData::CurrentWave;

    if (SURVIVAL::SurvivalData::hardcore)
        accuracyModifier = 10;

    PED::SET_PED_ARMOUR(ped, SURVIVAL::SurvivalData::hardcore ? 300 : 150);
    PED::SET_PED_ACCURACY(ped, accuracyModifier * 3);
    WEAPON::GIVE_WEAPON_TO_PED(ped, eWeapon::WeaponMinigun, 9000, true, true);
    PED::SET_PED_FIRING_PATTERN(ped, eFiringPattern::FiringPatternFullAuto);
    PED::SET_PED_COMBAT_MOVEMENT(ped, 2);
    TASK::TASK_COMBAT_PED(ped, PLAYER::PLAYER_PED_ID(), 0, 16);
}

void InitializeRageEnemy(Ped ped)
{
    PED::SET_PED_SUFFERS_CRITICAL_HITS(ped, false);
    PED::SET_PED_CAN_RAGDOLL(ped, false);
    PED::SET_PED_CONFIG_FLAG(ped, 281, true);
    PED::SET_PED_MAX_HEALTH(ped, 1250);
    ENTITY::SET_ENTITY_HEALTH(ped, 1250, 0);
    TASK::TASK_PUT_PED_DIRECTLY_INTO_MELEE(ped, PLAYER::PLAYER_PED_ID(), 0, -1, 0, 0);
    PED::SET_AI_MELEE_WEAPON_DAMAGE_MODIFIER(100);
    BLIPS::CreateForEnemyPed(ped, 671, "Ragemode Sasquatch");
    PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, Data::enemiesRelGroup);
}

void InitializeSuicidal(Ped ped)
{
    PED::SET_PED_MAX_HEALTH(ped, 420);
    ENTITY::SET_ENTITY_HEALTH(ped, 420, 0);
    PED::SET_PED_CONFIG_FLAG(ped, 281, true);
    WEAPON::GIVE_WEAPON_TO_PED(ped, eWeapon::WeaponKnife, 1, true, true);
    WEAPON::GIVE_WEAPON_TO_PED(ped, eWeapon::WeaponHatchet, 1, true, true);
    WEAPON::GIVE_WEAPON_TO_PED(ped, eWeapon::WeaponHammer, 1, true, true);
    TASK::TASK_PUT_PED_DIRECTLY_INTO_MELEE(ped, PLAYER::PLAYER_PED_ID(), 0, -1, 0, 0);
    BLIPS::CreateForEnemyPed(ped, 486, "Suicide Bomber");
    PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, Data::enemiesRelGroup);
}

void InitializeEnemy(Ped ped)
{
    Hash pedModel = ENTITY::GET_ENTITY_MODEL(ped);

    if (pedModel == 0xCE2CB751 &&  !jesusSpawned)
    {
        InitializeJesus(ped);
        return;
    }

    if ((pedModel == 0xAD340F5A || pedModel == 0x61D4C771))
    {
        int rageChance = CALC::RanInt(100, 1);

        if (rageChance <= SURVIVAL::SurvivalData::CurrentWave * 2)
        {
            InitializeRageEnemy(ped);
            return;
        }
    }

    switch (pedModel)
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
            PED::SET_PED_CONFIG_FLAG(ped, 155, false);
            PED::SET_PED_CONFIG_FLAG(ped, 42, true);
            PED::SET_PED_CONFIG_FLAG(ped, 301, true);
            break;
        default:
            break;
    }

    if (pedModel == 0x64611296)
    {
        PED::SET_PED_DEFAULT_COMPONENT_VARIATION(ped);
        AUDIO::DISABLE_PED_PAIN_AUDIO(ped, true);
    }

    PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, Data::enemiesRelGroup);
    int accuracyModifier = SURVIVAL::SurvivalData::CurrentWave > 10 ? 10 : SURVIVAL::SurvivalData::CurrentWave;

    if (SURVIVAL::SurvivalData::hardcore)
        accuracyModifier = 10;

    PED::SET_PED_ARMOUR(ped, SURVIVAL::SurvivalData::hardcore ? 150 : SURVIVAL::SurvivalData::CurrentWave * 10);
    PED::SET_PED_ACCURACY(ped, 7 + (accuracyModifier * 3));
    std::vector<Hash> weapons = GetWeapons(pedModel);
    size_t index = CALC::RanInt(weapons.size() - (size_t)1, (size_t)0);
    Hash weaponHash = weapons.at(index);
    PED::SET_PED_CONFIG_FLAG(ped, 281, true);
    WEAPON::GIVE_WEAPON_TO_PED(ped, weaponHash, 1000, true, true);
    TASK::TASK_COMBAT_PED(ped, PLAYER::PLAYER_PED_ID(), 0, 16);
    BLIPS::CreateForEnemyPed(ped);
    PED::SET_PED_COMBAT_MOVEMENT(ped, 2);
    int i = CALC::RanInt(100, 1);

    if (i <= accuracyModifier * 3)
    {
        PED::SET_PED_FIRING_PATTERN(ped, eFiringPattern::FiringPatternFullAuto);
    }
}

void InitializeEnemyInAircraft(Ped ped, bool passenger)
{
    Hash pedModel = ENTITY::GET_ENTITY_MODEL(ped);

    switch (pedModel)
    {
        case 0x5E3DA4A4:
        case 0x15F8700D:
        case 0x616C97B9:
        case 0x72C0CAD2:
        case 0x8D8F1B10:
            PED::SET_PED_CONFIG_FLAG(ped, 155, false);
            PED::SET_PED_CONFIG_FLAG(ped, 42, true);
            PED::SET_PED_CONFIG_FLAG(ped, 301, true);
            break;
        default:
            break;
    }

    if (pedModel == 0x64611296)
    {
        PED::SET_PED_DEFAULT_COMPONENT_VARIATION(ped);
        AUDIO::DISABLE_PED_PAIN_AUDIO(ped, true);
    }

    PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, Data::enemiesRelGroup);
    int accuracyModifier = SURVIVAL::SurvivalData::CurrentWave > 10 ? 10 : SURVIVAL::SurvivalData::CurrentWave;

    if (SURVIVAL::SurvivalData::hardcore)
        accuracyModifier = 10;

    PED::SET_PED_ARMOUR(ped, SURVIVAL::SurvivalData::hardcore ? 150 : SURVIVAL::SurvivalData::CurrentWave * 10);
    PED::SET_PED_ACCURACY(ped, 7 + (accuracyModifier * 3));
    std::vector<DWORD> weapons = GetWeapons(pedModel);
    size_t index = CALC::RanInt(weapons.size() - (size_t)1, (size_t)0);
    DWORD weaponHash = weapons.at(index);
    PED::SET_PED_CONFIG_FLAG(ped, 281, true);
    WEAPON::GIVE_WEAPON_TO_PED(ped, weaponHash, 1000, true, true);

    if (passenger)
        TASK::TASK_SHOOT_AT_ENTITY(ped, PLAYER::PLAYER_PED_ID(), 18000000, eFiringPattern::FiringPatternBurstFire);
    else
        TASK::TASK_COMBAT_PED(ped, PLAYER::PLAYER_PED_ID(), 0, 16);

    BLIPS::CreateForEnemyPed(ped);
    PED::SET_PED_COMBAT_MOVEMENT(ped, 2);
    int i = CALC::RanInt(100, 1);

    if (i <= accuracyModifier * 3)
    {
        PED::SET_PED_FIRING_PATTERN(ped, eFiringPattern::FiringPatternFullAuto);
    }
}

void InitializeEnemyInVehicle(Ped ped, bool passenger)
{
    Hash pedModel = ENTITY::GET_ENTITY_MODEL(ped);

    switch (pedModel)
    {
        case 0x5E3DA4A4:
        case 0x15F8700D:
        case 0x616C97B9:
        case 0x72C0CAD2:
        case 0x8D8F1B10:
            PED::SET_PED_CONFIG_FLAG(ped, 155, false);
            PED::SET_PED_CONFIG_FLAG(ped, 42, true);
            PED::SET_PED_CONFIG_FLAG(ped, 301, true);
            break;
        default:
            break;
    }

    if (pedModel == 0x64611296)
    {
        PED::SET_PED_DEFAULT_COMPONENT_VARIATION(ped);
        AUDIO::DISABLE_PED_PAIN_AUDIO(ped, true);
    }

    PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, Data::enemiesRelGroup);
    int accuracyModifier = SURVIVAL::SurvivalData::CurrentWave > 10 ? 10 : SURVIVAL::SurvivalData::CurrentWave;

    if (SURVIVAL::SurvivalData::hardcore)
        accuracyModifier = 10;

    PED::SET_PED_ARMOUR(ped, SURVIVAL::SurvivalData::hardcore ? 150 : SURVIVAL::SurvivalData::CurrentWave * 10);
    PED::SET_PED_ACCURACY(ped, 7 + (accuracyModifier * 3));
    std::vector<Hash> weapons = GetWeapons(pedModel);
    size_t index = CALC::RanInt(weapons.size() - (size_t)1, (size_t)0);
    Hash weaponHash = weapons.at(index);
    WEAPON::GIVE_WEAPON_TO_PED(ped, weaponHash, 1000, true, true);

    if (passenger)
        TASK::TASK_VEHICLE_SHOOT_AT_PED(ped, PLAYER::PLAYER_PED_ID(), 40.0f);
    else
        TASK::TASK_COMBAT_PED(ped, PLAYER::PLAYER_PED_ID(), 0, 16);

    BLIPS::CreateForEnemyPed(ped);
    PED::SET_PED_COMBAT_MOVEMENT(ped, 2);
    int i = CALC::RanInt(100, 1);

    if (i <= accuracyModifier * 3)
    {
        PED::SET_PED_FIRING_PATTERN(ped, eFiringPattern::FiringPatternFullAuto);
    }
}

void ProcessJesus()
{
    if (jesusSpawned)
    {
        if (deadEnemies.empty() && !enemyJesus.waiting)
        {
            enemyJesus.StartWaiting();
        }
        else if (!deadEnemies.empty())
        {
            if (enemyJesus.waiting)
            {
                enemyJesus.SetTarget(deadEnemies.front());
                enemyJesus.MoveToTarget();
            }
            else if (enemyJesus.movingToPed && enemyJesus.IsInRange())
            {
                enemyJesus.StartReviving();
            }
            else if (enemyJesus.revivingPed && enemyJesus.CanRevive())
            {
                enemyJesus.ReviveTarget();
                WAIT(1);

                for (int n = 0; n < deadEnemies.size(); n++)
                {
                    if (deadEnemies[n] == enemyJesus.targetPed)
                    {
                        deadEnemies.erase(deadEnemies.begin() + n);
                        break;
                    }
                }

                if (enemyJesus.targetPed == enemyJuggernaut)
                {
                    InitializeJuggernaut(enemyJesus.targetPed);
                }
                else if (ENTITY::GET_ENTITY_MODEL(enemyJesus.targetPed) == 0x9563221D)
                {
                    InitializeAnimal(enemyJesus.targetPed);
                }
                else
                {
                    InitializeEnemy(enemyJesus.targetPed);
                }

                footEnemies.push_back(enemyJesus.targetPed);
            }
        }
    }
}

void ENEMIES::Process()
{
    if (!MUSIC::MusicData::finalWaveHI && SURVIVAL::SurvivalData::CurrentWave == 10 && !SURVIVAL::SurvivalData::InfiniteWaves && !SURVIVAL::SurvivalData::timed && EnemiesData::currentWaveSize >= SURVIVAL::SurvivalData::MaxWaveSize / 2)
    {
        MUSIC::FinalWaveMusicHighIntensity();
        MUSIC::MusicData::finalWaveHI = true;
    }

    if (EnemiesData::canSpawnMore && !EnemiesData::limitReached)
    {
        if ((SURVIVAL::SurvivalData::CurrentWave >= 7 || SURVIVAL::SurvivalData::hardcore) && SURVIVAL::SpawnerData::hasDogs && !dogLimitReached)
        {
            if (TIMERS::ProcessDogTimer())
            {
                Ped ped = SURVIVAL::SpawnDog();
                InitializeAnimal(ped);
                footEnemies.push_back(ped);
                EnemiesData::currentDogCount += 1;

                if (EnemiesData::currentDogCount > 2)
                    dogLimitReached = true;

                TIMERS::RestartDogTimer();
            }
        }

        if (TIMERS::ProcessSpawnMoreTimer())
        {
            EnemiesData::currentWaveSize += 1;
            Ped ped;

            if ((SURVIVAL::SurvivalData::CurrentWave >= 8 || SURVIVAL::SurvivalData::hardcore) && SURVIVAL::SpawnerData::hasJuggernaut && !jugSpawned && EnemiesData::currentWaveSize >= SURVIVAL::SurvivalData::MaxWaveSize / 2)
            {
                ped = SURVIVAL::SpawnJuggernaut();
                InitializeJuggernaut(ped);
                jugSpawned = true;
            }
            else if ((SURVIVAL::SurvivalData::CurrentWave >= 5 || SURVIVAL::SurvivalData::hardcore) && SURVIVAL::SpawnerData::hasSuicidal && CALC::RanInt(100, 1) <= 20 && !suicidalLimitReached)
            {
                ped = SURVIVAL::SpawnEnemy(SURVIVAL::SurvivalData::CurrentWave, !jesusSpawned);
                InitializeSuicidal(ped);
                Suicidal sus = Suicidal();
                sus.ped = ped;
                suicidalEnemies.push_back(sus);

                if (suicidalEnemies.size() >= 2)
                    suicidalLimitReached = true;
            }
            else
            {
                ped = SURVIVAL::SpawnEnemy(SURVIVAL::SurvivalData::CurrentWave, !jesusSpawned);
                InitializeEnemy(ped);
            }

            footEnemies.push_back(ped);
            TIMERS::RestartEnemyTimer();

            if (EnemiesData::currentWaveSize >= SURVIVAL::SurvivalData::MaxWaveSize && !SURVIVAL::SurvivalData::timed)
            {
                EnemiesData::canSpawnMore = false;
            }
        }

        if (SURVIVAL::SpawnerData::hasVehicles && (SURVIVAL::SurvivalData::CurrentWave >= 3 || SURVIVAL::SurvivalData::hardcore) && EnemiesData::currentVehicles < SURVIVAL::SurvivalData::MaxVehicles)
        {
            if (TIMERS::ProcessVehicleTimer())
            {
                bool boat = SURVIVAL::SpawnerData::hasBoats && CALC::RanInt(100, 1) <= 35;
                Vehicle vehicle = SURVIVAL::SpawnVehicle(boat);
                std::vector<Ped> peds = SURVIVAL::SpawnEnemiesInVehicle(vehicle, SURVIVAL::SurvivalData::CurrentWave);

                for (size_t i = 0; i < peds.size(); i++)
                {
                    InitializeEnemyInVehicle(peds.at(i), i != 0);
                    footEnemies.push_back(peds.at(i));
                }

                BLIPS::CreateForEnemyVehicle(vehicle);
                enemyVehicles.push_back(vehicle);
                EnemiesData::currentVehicles += 1;
                EnemiesData::currentWaveSize += peds.size();
                TIMERS::RestartVehicleTimer();

                if (EnemiesData::currentWaveSize >= SURVIVAL::SurvivalData::MaxWaveSize && !SURVIVAL::SurvivalData::timed)
                {
                    EnemiesData::canSpawnMore = false;
                }
            }
        }

        if (SURVIVAL::SpawnerData::hasAircraft && (SURVIVAL::SurvivalData::CurrentWave >= 5 || SURVIVAL::SurvivalData::hardcore) && EnemiesData::currentAircraft < SURVIVAL::SurvivalData::MaxAircraft)
        {
            if (TIMERS::ProcessAircraftTimer())
            {
                Vehicle aircraft = SURVIVAL::SpawnAircraft();
                std::vector<Ped> peds = SURVIVAL::SpawnEnemiesInVehicle(aircraft, SURVIVAL::SurvivalData::CurrentWave);

                for (size_t i = 0; i < peds.size(); i++)
                {
                    InitializeEnemyInAircraft(peds.at(i), i != 0);
                    footEnemies.push_back(peds.at(i));
                }

                BLIPS::CreateForEnemyVehicle(aircraft);
                enemyVehicles.push_back(aircraft);

                EnemiesData::currentAircraft += 1;
                EnemiesData::currentWaveSize += peds.size();
                TIMERS::RestartAircraftTimer();

                if (EnemiesData::currentWaveSize >= SURVIVAL::SurvivalData::MaxWaveSize && !SURVIVAL::SurvivalData::timed)
                {
                    EnemiesData::canSpawnMore = false;
                }
            }
        }
    }

    if (SURVIVAL::SurvivalData::timed)
    {
        if (TIMERS::ProcessTimedSurvivalTimer((SURVIVAL::SurvivalData::timedTimeLeft)))
        {
            SURVIVAL::QuitSurvival(true);
            return;
        }
    }

    if (!footEnemies.empty())
    {
        if (!TIMERS::LeavingZone::Started)
            SCREEN::ShowSubtitle("Kill the ~r~enemies.", 8000);

        if (SURVIVAL::SpawnerData::hasSuicidal)
            ProcessSuicidals();

        RemoveDeadEnemies();
        ProcessJesus();
        MUSIC::Process(EnemiesData::currentWaveSize, SURVIVAL::SurvivalData::MaxWaveSize);
        
        if (!SURVIVAL::SurvivalData::hardcore)
        {
            switch (SURVIVAL::SurvivalData::CurrentWave)
            {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    EnemiesData::limitReached = SURVIVAL::SurvivalData::timed ? footEnemies.size() >= 14 : footEnemies.size() >= 15;
                    break;
                case 6:
                case 7:
                    EnemiesData::limitReached = SURVIVAL::SurvivalData::timed ? footEnemies.size() >= 13 : footEnemies.size() >= 14;
                    break;
                default:
                    EnemiesData::limitReached = SURVIVAL::SurvivalData::timed ? footEnemies.size() >= 12 : footEnemies.size() >= 13;
                    break;
            }
        }

        return;
    }
    else if (!EnemiesData::canSpawnMore)
    {
        if (SURVIVAL::SurvivalData::InfiniteWaves || SURVIVAL::SurvivalData::CurrentWave < 10)
        {
            SCREEN::ShowSubtitle("", 1);
            INTERMISSION::Start();
        }
        else
        {
            SURVIVAL::CompleteSurvival();
        }
    }
}
