// © 2019 Cul_Team All Rights Reserved

#include "CommonBall.h"
#include "GameField.h"
#include "Cube.h"
#include "MyPlayer.h"

#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values
ACommonBall::ACommonBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CommonBallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CommonBallMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/Meshes/CommonBall.CommonBall'"));
	CommonBallMesh->SetStaticMesh(MeshObj.Object);
	SetRootComponent(CommonBallMesh);


	//EnemyBallMeshComponent->OnComponentHit.AddDynamic(this, &AEnemyBall::Energize);
}

// Called when the game starts or when spawned
void ACommonBall::BeginPlay()
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
void ACommonBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACommonBall::Energize( // TODO : Remove from blueprint via .AddDynamic
	UPrimitiveComponent*	HitComponent,
	AActor*					OtherActor,
	UPrimitiveComponent*	OtherComponent,
	FVector					NormalImpulse,
	const FHitResult&		Hit
)
{
	
}


