// © 2019 Cul_Team All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "XonixGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class XONIX_API AXonixGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	
protected:
	void BeginPlay() override;

	/** The widget class to use for our HUD screen */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	/** The instance of the HUD */
	UPROPERTY() // P.S.: as we set it via a blueprint we have to ensure it will be propertly garbage collected
				// be trully just a copy of HUDWidgetClass ;)
	class UUserWidget*  CurrentWidget;
};
