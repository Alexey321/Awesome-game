// © 2019 Cul_Team All Rights Reserved

#include "GameField.h"
#include "Cube.h"
#include "CommonBall.h"
#include "Heart.h"
#include "MyPlayer.h"

#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Public/TimerManager.h"

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
	
	static ConstructorHelpers::FObjectFinder<UClass> Found3(TEXT("Class'/Game/Blueprints/Heart_BP.Heart_BP_C'"));
	Heart_BP = Found3.Object;

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

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;

		FVector GameFieldCenter; // TODO: Endure into a separate function p2
		FVector GameFieldBoxExtent;
		GetActorBounds(false, OUT GameFieldCenter, OUT GameFieldBoxExtent);

		FVector2D TopLeft = { GameFieldCenter.X + GameFieldBoxExtent.X, GameFieldCenter.Y - GameFieldBoxExtent.Y };
		FVector2D BottomRight = { GameFieldCenter.X - GameFieldBoxExtent.X, GameFieldCenter.Y + GameFieldBoxExtent.Y };
	
		float i = BottomRight.X;// + sizeCell;
		const float il = TopLeft.X;// - sizeCell;

		Cubes.Reserve(il / sizeCell);
		for (; i < il; i += sizeCell)
		{
			float j = TopLeft.Y;// + sizeCell;
			const float jl = BottomRight.Y;// - sizeCell;
			TArray<ACube*> temp;
			temp.Reserve(jl / sizeCell);
			for (; j < jl; j += sizeCell)
			{
				FVector SpawnLocation = { i, j, 0.0f };
				ACube* a = World->SpawnActor<ACube>(ACube::StaticClass(), SpawnLocation, FRotator::ZeroRotator, SpawnParams);
				a->SetActorHiddenInGame(true);
				a->SetActorEnableCollision(false);
				temp.Add(a);
			}
			Cubes.Add(temp);
		}
		//----------
		FVector FlattenedExtent = GameFieldBoxExtent; // TODO: Endure into a separate function p2
		FlattenedExtent.Y *= 0.9;
		FlattenedExtent.X *= 0.9;
		GameFieldCenter.Z = HightOfBallAboveBottomOftheField;
		for (int32 i = 0; i < BallsCount; i++)
		{

			FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(GameFieldCenter, FlattenedExtent);

			FRotator SpawnRotation;
			SpawnRotation.Yaw = FMath::FRand() * 360.0f;
			SpawnRotation.Pitch = FMath::FRand() * 360.0f;
			SpawnRotation.Roll = FMath::FRand() * 360.0f;

			ACommonBall* CommonBall = World->SpawnActor<ACommonBall>(CommonBall_BP, SpawnLocation, SpawnRotation, SpawnParams);

			if (CommonBall)
			{
				CommonBalls.Add(CommonBall);
			}
			else
			{
				--i;
				continue;
			}
		}
		//----------
		GetWorldTimerManager().SetTimer(HeartSpawnTimerHandle, this, &AGameField::SpawnHeart, HeartSpawnRate, true, 0);
	}
}

void AGameField::SpawnHeart()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		int32 y = FMath::RandRange(0, Cubes.Num() - 1);
		int32 x = FMath::RandRange(0, Cubes[y].Num() - 1);

		FVector SpawnLocation = Cubes[y][x]->GetActorLocation();
		SpawnLocation.Z += 2 * sizeCell + 3; // raize Heart a bit up

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;

		if (MyPlayer->Flags[y][x] != 0) // if we are above the filled area
		{ // then try again
			SpawnHeart();
		}
		else
		{
			AHeart* Heart = World->SpawnActor<AHeart>(Heart_BP, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
			if (Heart)
			{
				Hearts.Add(Heart);
			}
			else // if failed to spawn
			{ // then try again
				SpawnHeart();
			}
		}
	}

}










