// © 2019 Cul_Team All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyPlayer.generated.h"

UCLASS()
class XONIX_API AMyPlayer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<TArray<int32>> Flags;

	void SetPos(const FVector2D& Pos);

	FVector2D Pos = { 10, 0 };

	FString currDirection = "None";

	int32 AmountOfConqueredCells = 0;

	int32 Lives = 10;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	int32 Level = 1;

	UFUNCTION(BlueprintCallable, Category = "State"/*, meta = (AllowPrivateAccess = "true")*/)
	FString GetState() const; // constructs line to show on HUD

private:
	class UStaticMeshComponent* PlayerMesh;

	class AGameField* GameField;


	void Up();
	void Down();
	void Right();
	void Left();

	FString prevDirection = "None";


	float Speed = 0.2f; // TODO : adjust speed

	void Conquer();
	void _Conquer(int32 y, int32 x);


	TArray<TArray<int32>> round;
	//FVector PrevLocation;
	//FVector NewLocation;

	TArray<TArray<int32>> res;
	
	int32 AmountOfCells = 0;

	class ACameraActor* Camera;

	float DistanceFromCameraX;
	float DistanceFromCameraY;
	float DistanceFromCameraZ;

	// Limit of direction changes to change the direction of sight
	int32 LoDCtCtDoS = 15;
	int32 i_LoDCtCtDoS = 0;


};
