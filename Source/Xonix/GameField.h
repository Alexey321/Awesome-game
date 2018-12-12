// © 2019 Cul_Team All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameField.generated.h"

UCLASS()
class XONIX_API AGameField : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameField();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<TArray<class ACube*>> Cubes;

	const int32 sizeCell = 10; // don't change this unless reexport all corrected models from maya und alter each magic number used in project


	TArray<FVector2D> Tail;

	TArray<class ACommonBall*> CommonBalls;
	
	TArray<class AHeart*> Hearts;

	TSubclassOf<class ACommonBall> CommonBall_BP;
	

	int32 BallsCount = 2; // initial balls count


	float PercentageOfFieldToWin = 0.55; // [0 - 1]

	float PercentageOfConquered = 0; // [0 - 100]


private:
	class UStaticMeshComponent* GameFieldMesh;
	
	class AMyPlayer* MyPlayer;

	// Filles game field with cubes
	void GenerateGameField();

	UFUNCTION()
	void SpawnHeart();
	TSubclassOf<class AHeart> Heart_BP;
	FTimerHandle HeartSpawnTimerHandle;
	
	float HeartSpawnRate = 7.5;

};
