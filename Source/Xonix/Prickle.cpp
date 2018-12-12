// © 2019 Cul_Team All Rights Reserved

#include "Prickle.h"
#include "GameField.h"
#include "Cube.h"
#include "MyPlayer.h"
//#include "Heart.h"

#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values
APrickle::APrickle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PrickleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrickleMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/Meshes/Prickle.Prickle'"));
	PrickleMesh->SetStaticMesh(MeshObj.Object);
	SetRootComponent(PrickleMesh);

	//PrickleMesh->OnComponentHit.AddDynamic(this, &APrickle::OnHit);
}

// Called when the game starts or when spawned
void APrickle::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* const World = GetWorld();
	if (World)
	{

		TArray<AActor*> FoundActors;

		UGameplayStatics::GetAllActorsOfClass(World, AGameField::StaticClass(), OUT FoundActors);
		GameField = Cast<AGameField>(FoundActors[0]);

		UGameplayStatics::GetAllActorsOfClass(World, AMyPlayer::StaticClass(), OUT FoundActors);
		MyPlayer = Cast<AMyPlayer>(FoundActors[0]);

	}


}

// Called every frame
void APrickle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APrickle::OnHit(
	UPrimitiveComponent*	HitComponent,
	AActor*					OtherActor,
	UPrimitiveComponent*	OtherComponent,
	FVector					NormalImpulse,
	const FHitResult&		Hit
)
{
	
	if (OtherActor->ActorHasTag(FName("Player"))/*OtherActor->StaticClass() == MyPlayer->StaticClass()*/) // if prickle hits player
	{
		//----------------------------------------
		/*
		// Uncomment this if you want to enharder game
		for (int32 y = 1; y < GameField->Cubes.Num() - 1; ++y) // free gamefield
		{// TODO: Endure into a separate function p1
			for (int32 x = 1; x < GameField->Cubes[y].Num() - 1; ++x)
			{
				GameField->Cubes[y][x]->SetActorHiddenInGame(true);
				GameField->Cubes[y][x]->SetActorEnableCollision(false);

				MyPlayer->Flags[y][x] = 0;
			}
		}
		*/
		
		// Comment this if you want to enharder game
		for (int32 i = 0; i < GameField->Tail.Num(); ++i)
		{ // reset the line we were drawing
			GameField->Cubes[GameField->Tail[i].Y][GameField->Tail[i].X]->SetActorHiddenInGame(true);
			GameField->Cubes[GameField->Tail[i].Y][GameField->Tail[i].X]->SetActorEnableCollision(false);

			MyPlayer->Flags[GameField->Tail[i].Y][GameField->Tail[i].X] = 0;
		}
		//-----------------------------------------
		MyPlayer->Pos = { 10, 0 }; // reset poition and counters
		MyPlayer->SetPos(MyPlayer->Pos);
		MyPlayer->currDirection = "None";
		MyPlayer->AmountOfConqueredCells = 0;

		MyPlayer->Lives -= 1;
		

		//UE_LOG(LogTemp, Warning, TEXT("APrickle hit Player!!!"))
	}
	
}