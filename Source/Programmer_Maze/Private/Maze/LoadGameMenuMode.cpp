// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing


#include "Maze/LoadGameMenuMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/GridSlot.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/Border.h"
#include "MazeGameInstance.h"
#include "Kismet/GameplayStatics.h"

ALoadGameMenuMode::ALoadGameMenuMode() :AGameModeBase() {
	static ConstructorHelpers::FClassFinder<UUserWidget> ContinueMenuClassFinder(TEXT("/Game/UI/Flow/LoadGameMenuBP.LoadGameMenuBP_C"));
	//follow the typedefs
	static ConstructorHelpers::FObjectFinder<UObject>FontFinder(TEXT("/Engine/EngineFonts/Roboto.Roboto"));
	ContinueMenuClass = ContinueMenuClassFinder.Class;
	fontToUse = FontFinder.Object;
}

void ALoadGameMenuMode::BeginPlay()
{
	Super::BeginPlay();
	//Step 1:
	//Construct the menu and add it to the viewport
	ContinueMenuHandle = NewObject<UUserWidget>(this, ContinueMenuClass);
	ContinueMenuHandle->SetVisibility(ESlateVisibility::Visible);
	ContinueMenuHandle->AddToViewport();
	//Step 2:
	//Enable mouse cursor
	APlayerController* controller = UGameplayStatics::GetPlayerController(this, 0);
	controller->SetShowMouseCursor(true);
	//Step3:: get the handle to the grid panel
	FObjectProperty* prop = FindFieldChecked<FObjectProperty>(ContinueMenuClass, FName(TEXT("SavesPanel")));
	UGridPanel* gridPanel = Cast<UGridPanel>(prop->GetObjectPropertyValue(prop->ContainerPtrToValuePtr<UObject>(ContinueMenuHandle)));
	check(gridPanel);
	//Finally initialize the panel
	fillPanel(gridPanel);
}

void ALoadGameMenuMode::fillPanel(UGridPanel* panel)
{
	//this function should fill the panel with the data from the saves
	const FSlateFontInfo levelNameFont = FSlateFontInfo(fontToUse, 39, FName(TEXT("Bold")));
	const FSlateColor black = FSlateColor(FLinearColor(0, 0, 0));
	const FSlateChildSize slotSize = FSlateChildSize(ESlateSizeRule::Fill);
	for (int i = 0; i < 8; i++) {
		FString name = FString::Printf(TEXT("PROGRAMMING_MAZE_SAV_%d"), i);
		UProgrammingMazeLevelSaves* save = NULL;
		if (UGameplayStatics::DoesSaveGameExist(name,0)) {
			//if it is exists lets load it
			save = Cast<UProgrammingMazeLevelSaves>(UGameplayStatics::LoadGameFromSlot(name, 0));
		}
		loadedSaves.Push(save);
		if (save) {
			//initialize the things
			//the root button
			UButton* root = NewObject<UButton>(panel);
			//vertical box for holding the content
			UVerticalBox* contentRoot = NewObject<UVerticalBox>(panel);
			//levelName
			UTextBlock* levelName = NewObject<UTextBlock>(panel);
			levelName->SetColorAndOpacity(black);
			levelName->SetFont(levelNameFont);

			//get the name and replace _ with spaces
			FString levelNameTxt = save->LevelName.ToString();
			levelNameTxt = levelNameTxt.Replace(TEXT("_"), TEXT(" "));

			levelName->SetText(FText::FromString(levelNameTxt));
			//Save time
			UTextBlock* date = NewObject<UTextBlock>(panel);
			date->SetColorAndOpacity(black);
			date->SetText(FText::FromString(save->SaveTime.ToString().RightChop(3)));
			//add the levelName to the box
			UVerticalBoxSlot* slot = contentRoot->AddChildToVerticalBox(levelName);
			slot->SetSize(slotSize);
			slot->SetPadding(FMargin(50.0f));
			//add the date to the slot
			slot = contentRoot->AddChildToVerticalBox(date);
			slot->SetSize(slotSize);
			slot->SetPadding(FMargin(50.0f));
			//add it to the root then the grid
			root->AddChild(contentRoot);
			UGridSlot* rootSlot = panel->AddChildToGrid(root, ((i >= 4) ? 1 : 0), (i % 4));
			rootSlot->SetPadding(FMargin(50.0f));
			//bind the handler to the button
			//cannot optimize because the delegate dont pass this pointer
			switch (i) {
			case 0:
				root->OnClicked.AddDynamic(this, &ALoadGameMenuMode::LoadSave0);
				break;
			case 1:
				root->OnClicked.AddDynamic(this, &ALoadGameMenuMode::LoadSave1);
				break;
			case 2:
				root->OnClicked.AddDynamic(this, &ALoadGameMenuMode::LoadSave2);
				break;
			case 3:
				root->OnClicked.AddDynamic(this, &ALoadGameMenuMode::LoadSave3);
				break;
			case 4:
				root->OnClicked.AddDynamic(this, &ALoadGameMenuMode::LoadSave5);
				break;
			case 5:
				root->OnClicked.AddDynamic(this, &ALoadGameMenuMode::LoadSave6);
				break;
			case 6:
				root->OnClicked.AddDynamic(this, &ALoadGameMenuMode::LoadSave6);
				break;
			case 7:
				root->OnClicked.AddDynamic(this, &ALoadGameMenuMode::LoadSave7);
				break;
			}
		}
		else {
			//add grey border as the replacement
			UBorder* border = NewObject<UBorder>(panel);
			border->SetBrushColor(FLinearColor(0.20f, 0.20f, 0.20f));
			UGridSlot* rootSlot = panel->AddChildToGrid(border, ((i >= 4) ? 1 : 0), (i % 4));
			rootSlot->SetPadding(FMargin(50.0f));
		}
	}
}

void ALoadGameMenuMode::loadSaveInternal(UProgrammingMazeLevelSaves* save)
{
	check(save); //the button should not appear when the save is not here QwQ
	//prepare for the loading
	UMazeGameInstance* instance = Cast<UMazeGameInstance>(UGameplayStatics::GetGameInstance(this));
	//set the save ID so the main mode know what to load
	instance->saveLoading = save;
	//finally load it
	UGameplayStatics::OpenLevel(this, save->LevelName);
}

#define DEFINE_LOAD_SAVE_IMPL(idx) void ALoadGameMenuMode::LoadSave##idx(){loadSaveInternal(loadedSaves[idx]);}

DEFINE_LOAD_SAVE_IMPL(0);
DEFINE_LOAD_SAVE_IMPL(1);
DEFINE_LOAD_SAVE_IMPL(2);
DEFINE_LOAD_SAVE_IMPL(3);
DEFINE_LOAD_SAVE_IMPL(4);
DEFINE_LOAD_SAVE_IMPL(5);
DEFINE_LOAD_SAVE_IMPL(6);
DEFINE_LOAD_SAVE_IMPL(7);