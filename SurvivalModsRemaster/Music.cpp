#include "pch.h"
#include "Music.hpp"

MUSIC::TrackGroups MUSIC::MusicData::currentTrackGroup;
bool MUSIC::MusicData::finalWaveHI;

void MUSIC::PrepareTracks()
{
    int value = CALC::RanInt(100, 1);

    if (value <= 20)
    {
        MusicData::currentTrackGroup = TrackGroups::BikerContracts;
    }
    else if (value <= 40)
    {
        MusicData::currentTrackGroup = TrackGroups::BusinessBattle;
    }
    else
    {
        MusicData::currentTrackGroup = TrackGroups::SurvivalPrimary;
    }
}

void MUSIC::StopTrack()
{
    AUDIO::TRIGGER_MUSIC_EVENT((char*)"MP_MC_STOP");
}

void MUSIC::StartTrack()
{
    switch (MusicData::currentTrackGroup)
    {
    case TrackGroups::SurvivalPrimary:
    {
        AUDIO::TRIGGER_MUSIC_EVENT((char*)"SM_W1_START_ALL");
        break;
    }
    case TrackGroups::BusinessBattle:
    {
        AUDIO::TRIGGER_MUSIC_EVENT((char*)"BTL_IDLE_START");
        break;
    }
    case TrackGroups::BikerContracts:
    {
        AUDIO::TRIGGER_MUSIC_EVENT((char*)"BIKER_SYG_START");
        break;
    }
    default:
        break;
    }
}

void MUSIC::LowIntensityTrack()
{
    switch (MusicData::currentTrackGroup)
    {
    case TrackGroups::SurvivalPrimary:
    {
        AUDIO::TRIGGER_MUSIC_EVENT((char*)"SM_W1_END");
        break;
    }
    case TrackGroups::BusinessBattle:
    {
        AUDIO::TRIGGER_MUSIC_EVENT((char*)"BTL_IDLE");
        break;
    }
    case TrackGroups::BikerContracts:
    {
        AUDIO::TRIGGER_MUSIC_EVENT((char*)"BIKER_SYG_CALM");
        break;
    }
    default:
        break;
    }
}

void MUSIC::MidIntensityTrack(int wave)
{
    switch (MusicData::currentTrackGroup)
    {
    case TrackGroups::SurvivalPrimary:
    {
        char evName[100];
        strcpy_s(evName, "SM_W");
        strcat_s(evName, std::to_string(wave > 10 ? 10 : wave).c_str());
        strcat_s(evName, "_MED");
        AUDIO::TRIGGER_MUSIC_EVENT(evName);
        break;
    }
    case TrackGroups::BusinessBattle:
    {
        AUDIO::TRIGGER_MUSIC_EVENT((char*)"BTL_MED_INTENSITY");
        break;
    }
    case TrackGroups::BikerContracts:
    {
        AUDIO::TRIGGER_MUSIC_EVENT((char*)"BIKER_DEFEND_POLICE_RAID_FIGHT");
        break;
    }
    default:
        break;
    }
}

void MUSIC::HighIntensityTrack(int wave)
{
    switch (MusicData::currentTrackGroup)
    {
    case TrackGroups::SurvivalPrimary:
    {
        char evName[100];
        strcpy_s(evName, "SM_W");
        strcat_s(evName, std::to_string(wave > 10 ? 10 : wave).c_str());
        strcat_s(evName, "_HIGH");
        AUDIO::TRIGGER_MUSIC_EVENT(evName);
        break;
    }
    case TrackGroups::BusinessBattle:
    {
        AUDIO::TRIGGER_MUSIC_EVENT((char*)"BTL_GUNFIGHT");
        break;
    }
    case TrackGroups::BikerContracts:
    {
        AUDIO::TRIGGER_MUSIC_EVENT((char*)"BIKER_SYG_ATTACKED");
        break;
    }
    default:
        break;
    }
}

void MUSIC::MissionCompletedSound()
{
    AUDIO::TRIGGER_MUSIC_EVENT((char*)"MP_DM_COUNTDOWN_KILL");
}

void MUSIC::FinalWaveMusic()
{
    AUDIO::TRIGGER_MUSIC_EVENT((char*)"BST_START");
}

void MUSIC::FinalWaveMusicHighIntensity()
{
    AUDIO::TRIGGER_MUSIC_EVENT((char*)"FIN1_SHOOTOUT_4");
}
