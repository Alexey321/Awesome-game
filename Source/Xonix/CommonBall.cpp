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
	
	
	float angle = FMath::FRandRange(0.0f, 360.0f);
	int x = Velocity * FMath::Sin(angle);
	int y = Velocity * FMath::Cos(angle);
	CommonBallMesh->AddImpulse(FVector(x, y, 0.0f)); // add initial impulse to the ball

	// Begin: Correct ball coordinates along Z axis
	FVector Pos = GetActorLocation(); // TODO: endure into a separate function p432
	if (Pos.Z > GameField->HightOfBallAboveBottomOftheField) // if ball jumps up
	{ // then pacify it down
		Pos.Z = GameField->HightOfBallAboveBottomOftheField;
		SetActorLocation(Pos);
		//UE_LOG(LogTemp, Warning, TEXT("ball jumps up"))
	}
	// End: Correct ball coordinates along Z axis

	
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
	
	FVector VecVelocity = CommonBallMesh->GetPhysicsLinearVelocity();
	if (VecVelocity.Size() != Velocity) // if our ball accelerates or decelerates
	{ // then pacify it down
		//UE_LOG(LogTemp, Warning, TEXT("%f != %f --> %f"), VecVelocity.Size() , Velocity, (VecVelocity * VecVelocity.Size() / Velocity).Size())
		CommonBallMesh->SetPhysicsLinearVelocity(VecVelocity * Velocity / VecVelocity.Size());
	}

	FVector Pos = GetActorLocation();

	if (OtherActor->ActorHasTag(FName("tail"))) // if ball hits tail
	{
		//-----------------------------------------
		/*
		// Uncomment this if you want to enharder game
		for (int32 y = 1; y < GameField->Cubes.Num() - 1; ++y)
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
		MyPlayer->Pos = { 10, 0 };
		MyPlayer->SetPos(MyPlayer->Pos);
		MyPlayer->currDirection = "None";
		MyPlayer->AmountOfConqueredCells = 0;
		
		MyPlayer->Lives -= 1;
	}
	else if (OtherActor->ActorHasTag(FName("Player"))) // if ball hits Player
	{
		AMyPlayer* MyPlayer = Cast<AMyPlayer>(OtherActor); 
		// TODO: otimize if statements
		if (GameField->Cubes.IsValidIndex(MyPlayer->Pos.Y) && GameField->Cubes[MyPlayer->Pos.Y].IsValidIndex(MyPlayer->Pos.X) // just in case, we'll check
			&&
				(
					MyPlayer->Flags[MyPlayer->Pos.Y][MyPlayer->Pos.X] == 3 // if ball hits player who stands on the outline of conquered region
					||
					MyPlayer->Flags[MyPlayer->Pos.Y][MyPlayer->Pos.X] == 2 // if ball hits player who stands on the border of the field
				)
			)
		{ // then do nothing
			UE_LOG(LogTemp, Warning, TEXT("Ball hits player on the outline -- we do nothing"))
		}
		else
		{
			//-----------------------------------------
			/*
			// Uncomment this if you want to enharder game
			for (int32 y = 1; y < GameField->Cubes.Num() - 1; ++y)
			{// TODO: Endure into a separate function p1
				for (int32 x = 1; x < GameField->Cubes[y].Num() - 1; ++x)
				{
					GameField->Cubes[y][x]->SetActorHiddenInGame(true);
					GameField->Cubes[y][x]->SetActorEnableCollision(false);

					MyPlayer->Flags[y][x] = 0;
				}
			}
			*/
			for (int32 i = 0; i < GameField->Tail.Num(); ++i)
			{ // reset the line we were drawing
				GameField->Cubes[GameField->Tail[i].Y][GameField->Tail[i].X]->SetActorHiddenInGame(true);
				GameField->Cubes[GameField->Tail[i].Y][GameField->Tail[i].X]->SetActorEnableCollision(false);

				MyPlayer->Flags[GameField->Tail[i].Y][GameField->Tail[i].X] = 0;
			}
			//-----------------------------------------
			MyPlayer->Pos = { 10, 0 };
			MyPlayer->SetPos(MyPlayer->Pos);
			MyPlayer->currDirection = "None";
			MyPlayer->AmountOfConqueredCells = 0;

			MyPlayer->Lives -= 1;
		}
	}

}


