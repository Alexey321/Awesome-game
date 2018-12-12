// © 2019 Cul_Team All Rights Reserved

#include "GameField.h"
#include "Cube.h"
#include "CommonBall.h"
#include "MyPlayer.h"

#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values
AGameField::AGameField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GameFieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GameFieldMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/Meshes/GameField.GameField'"));
	GameFieldMesh->SetStaticMesh(MeshObj.Object);
	SetRootComponent(GameFieldMesh);

	static ConstructorHelpers::FObjectFinder<UClass> Found1(TEXT("Class'/Game/Blueprints/CommonBall_BP.CommonBall_BP_C'"));
	CommonBall_BP = Found1.Object;

}

// Called when the game starts or when spawned
void AGameField::BeginPlay()
{
	Super::BeginPlay();
	GenerateGameField();
}

// Called every frame
void AGameField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Filles game field with cubes
void AGameField::GenerateGameField()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(World, AMyPlayer::StaticClass(), OUT FoundActors);
		MyPlayer = Cast<AMyPlayer>(FoundActors[0]);

		// TODO: fill gamefield with cubes 
	}
}



