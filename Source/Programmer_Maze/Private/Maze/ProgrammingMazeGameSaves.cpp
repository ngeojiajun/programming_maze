// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing


#include "Maze/ProgrammingMazeGameSaves.h"

UProgrammingMazeSettings::UProgrammingMazeSettings(const FObjectInitializer& ObjectInitializer):USaveGame(ObjectInitializer),
MasterVolume(1.0f),
BGMVolume(0.6f),
SFXVolume(0.6f),
savefileIndex(0)
{
}