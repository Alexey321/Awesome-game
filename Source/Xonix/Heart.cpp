// © 2019 Cul_Team All Rights Reserved

#include "Heart.h"
#include "MyPlayer.h"
#include "GameField.h"
//#include "Prickle.h"
#include "Particles/ParticleSystem.h"

#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values
AHeart::AHeart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HeartMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeartMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/Meshes/Heart.Heart'"));
	HeartMesh->SetStaticMesh(MeshObj.Object);
	SetRootComponent(HeartMesh);

}

// Called when the game starts or when spawned
void AHeart::BeginPlay()
{
	Super::BeginPlay();

	UWorld* const World = GetWorld();
	if (World)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(World, AMyPlayer::StaticClass(), OUT FoundActors);
		MyPlayer = Cast<AMyPlayer>(FoundActors[0]);

		UGameplayStatics::GetAllActorsOfClass(World, AGameField::StaticClass(), OUT FoundActors);
		GameField = Cast<AGameField>(FoundActors[0]);
	}
}

// Called every frame
void AHeart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHeart::OnHit(
	UPrimitiveComponent*	HitComponent,
	AActor*					OtherActor,
	UPrimitiveComponent*	OtherComponent,
	FVector					NormalImpulse,
	const FHitResult&		Hit
)
{
	if (OtherActor->ActorHasTag("Prickle")) // if Prickle hits Heart
	{ // then destroy Heart
		GameField->Hearts.RemoveSingle(this);
		SpawnSprikes();
		Destroy();
		//UE_LOG(LogTemp, Warning, TEXT("APrickle hit AHeart!!!"))
	}
}

void AHeart::SpawnSprikes() const
{
	UWorld* const World = GetWorld();
	if (World && ParticleSystem)
	{
		UGameplayStatics::SpawnEmitterAtLocation(World, ParticleSystem, GetActorLocation());
	}
}