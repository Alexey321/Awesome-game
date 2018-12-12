// © 2019 Cul_Team All Rights Reserved

#include "XonixGameModeBase.h"

#include "Blueprint/UserWidget.h"
#include "Engine/World.h"

void AXonixGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	UWorld* const World = GetWorld();
	if (World)
	{
		CurrentWidget = CreateWidget<UUserWidget>(World, HUDWidgetClass);
		CurrentWidget->AddToViewport();
	}
}

