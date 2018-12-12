// © 2019 Cul_Team All Rights Reserved

#include "MyPlayer.h"
#include "GameField.h"
#include "Cube.h"
#include "CommonBall.h"
#include "Heart.h"
#include "CommonBall.h"

#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"
#include "Camera/CameraActor.h"
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AMyPlayer::AMyPlayer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/Meshes/Cube.Cube'"));
	PlayerMesh->SetStaticMesh(MeshObj.Object);
	SetRootComponent(PlayerMesh);

	//SetActorTickInterval(Speed); label: uytuyt654654



}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
	UWorld* const World = GetWorld();
	if (World)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(World, AGameField::StaticClass(), OUT FoundActors);
		GameField = Cast<AGameField>(FoundActors[0]);
		EnableInput(World->GetFirstPlayerController());

		UGameplayStatics::GetAllActorsWithTag(World, FName("Camera"), OUT FoundActors);
		Camera = Cast<ACameraActor>(FoundActors[0]);
		GEngine->GetFirstLocalPlayerController(World)->SetViewTargetWithBlend(Camera);

		DistanceFromCameraX = -FMath::Abs(GetActorLocation().X - Camera->GetActorLocation().X);
		DistanceFromCameraY = FMath::Abs(GetActorLocation().Y - Camera->GetActorLocation().Y);
		DistanceFromCameraZ = FMath::Abs(GetActorLocation().Z - Camera->GetActorLocation().Z);

		ACube* a = GameField->Cubes[Pos.Y][Pos.X]; // warning: wtf, sometimes assertion failed
		FVector NewLocation = a->GetActorLocation();
		NewLocation.Z += GameField->sizeCell;
		NewLocation.X += DistanceFromCameraX;
		NewLocation.Y += DistanceFromCameraY;
		NewLocation.Z += DistanceFromCameraZ;
		Camera->SetActorLocation(NewLocation);

		InputComponent->BindAction("Up", IE_Pressed, this, &AMyPlayer::Up);
		InputComponent->BindAction("Down", IE_Pressed, this, &AMyPlayer::Down);
		InputComponent->BindAction("Right", IE_Pressed, this, &AMyPlayer::Right);
		InputComponent->BindAction("Left", IE_Pressed, this, &AMyPlayer::Left);
	}

	Flags.Reserve(GameField->Cubes.Num());
	for (int32 e = 0; e < GameField->Cubes.Num(); e++)
	{
		TArray<int32> t;
		t.Reserve(GameField->Cubes[e].Num());
		for (int32 e1 = 0; e1 < GameField->Cubes[e].Num(); e1++)
		{
			t.Add(0);
		}
		Flags.Add(t);
	}

	AmountOfCells = (GameField->Cubes.Num() - 1) * (GameField->Cubes[0].Num() - 1);

	for (int32 e = 0; e < GameField->Cubes.Num(); e++)
	{
		for (int32 e1 = 0; e1 < GameField->Cubes[e].Num(); e1++)
		{
			if (e == 0 || e == Flags.Num() - 1 || e1 == 0 || e1 == Flags[e].Num() - 1)
			{
				//static int32 uuu = 0;
				Flags[e][e1] = 2;
				//UE_LOG(LogTemp, Warning, TEXT("uuu: %d"), uuu++)
			}
		}
	}
	SetPos(Pos);



}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (Lives <= 0) // if we have lost all our lives
	{ // then go to level 1
		for (int32 y = 1; y < GameField->Cubes.Num() - 1; ++y) // free gamefield
		{// TODO: Endure into a separate function p1
			for (int32 x = 1; x < GameField->Cubes[y].Num() - 1; ++x)
			{
				GameField->Cubes[y][x]->SetActorHiddenInGame(true);
				GameField->Cubes[y][x]->SetActorEnableCollision(false);

				Flags[y][x] = 0;

			}
		}
		//-----------------------------------------
		this->Pos = { 10, 0 }; // reset poition and counters
		SetPos(this->Pos);
		currDirection = "None";
		AmountOfConqueredCells = 0;

		Lives = 10;

		Level = 1;

		// TODO: optimize all for conditions
		for (int32 i = 0; i < GameField->CommonBalls.Num() - GameField->BallsCount; ++i) // delete superfluous CommonBalls
		{
			GameField->CommonBalls[i]->Destroy();
			GameField->CommonBalls.RemoveAt(i);
		}

		//UE_LOG(LogTemp, Warning, TEXT("Lost!!!"))
	}

	static float counter = 0;
	if (counter > Speed) // may be want to uncomment  label: uytuyt654654  and remove this if statement
	{
		counter = 0;
		if (currDirection == "Up")
		{
			++Pos.Y;
			if (Pos.Y > GameField->Cubes.Num() - 1)
			{
				Pos.Y = GameField->Cubes.Num() - 1;
			}
			SetPos(Pos);
		}
		else if (currDirection == "Down")
		{
			--Pos.Y;
			if (Pos.Y < 0)
			{
				Pos.Y = 0;
			}
			SetPos(Pos);
		}
		else if (currDirection == "Right")
		{
			++Pos.X;
			if (Pos.X > GameField->Cubes[Pos.Y].Num() - 1)
			{
				Pos.X = GameField->Cubes[Pos.Y].Num() - 1;
			}
			SetPos(Pos);
		}
		else if (currDirection == "Left")
		{
			--Pos.X;
			if (Pos.X < 0)
			{
				Pos.X = 0;
			}
			SetPos(Pos);
		}
	}
	else
	{
		counter += 0.1;
	}
}

void AMyPlayer::SetPos(const FVector2D& Pos) // TODO : Implement smooth movement
{
	if (GameField->Cubes.IsValidIndex(Pos.Y) && GameField->Cubes[Pos.Y].IsValidIndex(Pos.X)) // just in case, we'll check
	{
		ACube* a = GameField->Cubes[Pos.Y][Pos.X];
		FVector NewLocation = a->GetActorLocation();
		NewLocation.Z += GameField->sizeCell;
		/*
		try // TODO: remove this hillarious shit
		{ 
		*/
			SetActorLocation(NewLocation); // seems that try catch doesn't help every time... but still tolerantly
		/*								// seems not to happen at all but still watch your back
		}
		catch (const std::exception&) // if some strange shit happens
		{ // then
			//UE_LOG(LogTemp, Warning, TEXT("Shit happens!"))
			
			SetPos(Pos); // maybe try again until it'll be OK ;)
			return;
			// TODO: in case of infinite recursion reset level (in our case "Shit happens!" VERY rare so we can ommit this for while)
			//		Stack overflow - code c00000fd (first/second chance not available)
		}
		*/
		for (int32 i = 0; i < GameField->Hearts.Num(); ++i) // Check if we hit Heart 
		{ // TODO: Optimize this shit
			if (NewLocation.X == GameField->Hearts[i]->GetActorLocation().X // warning: wtf, sometimes kick out(stack overflow)
				&&
				NewLocation.Y == GameField->Hearts[i]->GetActorLocation().Y
				) // then add live, destroy Heart, SpawnSprikes
			{
				GameField->Hearts[i]->SpawnSprikes();
				GameField->Hearts[i]->Destroy();
				GameField->Hearts.RemoveAt(i);
				++Lives;
			}
		}


		// uncomment if camera folows player
		//NewLocation.X += DistanceFromCameraX;
		//NewLocation.Y += DistanceFromCameraY;
		//NewLocation.Z += DistanceFromCameraZ;
		//Camera->SetActorLocation(NewLocation); // TODO: Optimize

		if (!(Pos.Y == 0 || Pos.Y == GameField->Cubes.Num() - 1 || Pos.X == 0 || Pos.X == GameField->Cubes[Pos.Y].Num() - 1))
		{ // on move back
			if (GameField->Tail.IsValidIndex(GameField->Tail.Num() - 2) 
				&&
				GameField->Tail[GameField->Tail.Num() - 2] == Pos
			) // hide
			{
				//FVector2D PosToHide = GameField->Tail.Pop();
				//ACube* b = GameField->Cubes[PosToHide.Y][PosToHide.X];
				//b->SetActorHiddenInGame(true);
				//b->SetActorEnableCollision(false);
				//Flags[PosToHide.Y][PosToHide.X] = 0;
			}
			else // on move ahead
			{
				if (Flags[Pos.Y][Pos.X] == 3) // if the field is already captured
				{
					if (GameField->Tail.Num()) // if we step on the capture area after drawing tail
					{
						//UE_LOG(LogTemp, Warning, TEXT("GameField->Tail.Num() %d"), GameField->Tail.Num())
						currDirection = "None"; // stop moving
						Conquer();
					}
					else
					{
						//
					}
				}
				else // step forward
				{
					a->SetActorHiddenInGame(false);
					a->SetActorEnableCollision(true);
					GameField->Tail.Add(Pos);
					GameField->Cubes[Pos.Y][Pos.X]->Tags.Add(FName("tail"));
					Flags[Pos.Y][Pos.X] = 1;
				}
			}
		}
		else // on border
		{
			if (GameField->Tail.Num() == 1) // step back on border
			{
				FVector2D PosToHide = GameField->Tail.Pop();
				ACube* b = GameField->Cubes[PosToHide.Y][PosToHide.X];
				b->SetActorHiddenInGame(true);
				b->SetActorEnableCollision(false);
				Flags[PosToHide.Y][PosToHide.X] = 0;
			}
			else if (GameField->Tail.Num() > 1) // conquer
			{
				Conquer();
			}
		}
	}
}

void AMyPlayer::Conquer()
{
	//UE_LOG(LogTemp, Warning, TEXT("Tail.length: %d"), GameField->Tail.Num())

	TArray<ACommonBall*> CommonBalls = GameField->CommonBalls;
	for (int32 i = 0; i < CommonBalls.Num(); ++i)
	{

		FVector CommonBallLoc = CommonBalls[i]->GetActorLocation();
		[this, CommonBallLoc]() {

			for (int32 j = 0; j < GameField->Cubes.Num(); ++j)
			{

				for (int32 m = 0; m < GameField->Cubes[j].Num(); ++m)
				{

					FVector GameFieldCenter;
					FVector GameFieldBoxExtent;
					GameField->Cubes[j][m]->GetActorBounds(false, OUT GameFieldCenter, OUT GameFieldBoxExtent);

					FVector2D TopLeft = { GameFieldCenter.X + GameFieldBoxExtent.X, GameFieldCenter.Y - GameFieldBoxExtent.Y };
					FVector2D BottomRight = { GameFieldCenter.X - GameFieldBoxExtent.X, GameFieldCenter.Y + GameFieldBoxExtent.Y };


					if (TopLeft.X > CommonBallLoc.X && BottomRight.X > CommonBallLoc.X
						&&
						TopLeft.Y > CommonBallLoc.Y && BottomRight.Y > CommonBallLoc.Y
						) // find cube under the ball
					{

						_Conquer(j, m);

						return;
					}
				}
			}
		}();


	}

	//UE_LOG(LogTemp, Warning, TEXT("len: %d"), res.Num())
	if (res.Num())
	{
		bool hidden = !GameField->Cubes[res[0][0]][res[0][1]]->GetActorEnableCollision();

		for (int32 y = 1; y < GameField->Cubes.Num() - 1; ++y)
		{
			for (int32 x = 1; x < GameField->Cubes[y].Num() - 1; ++x)
			{
				GameField->Cubes[y][x]->SetActorHiddenInGame(false);
				GameField->Cubes[y][x]->SetActorEnableCollision(true);
				Flags[y][x] = 3;

			}
		}

		for (int32 eee = 0; eee < res.Num(); ++eee)
		{
			GameField->Cubes[res[eee][0]][res[eee][1]]->SetActorHiddenInGame(true);
			GameField->Cubes[res[eee][0]][res[eee][1]]->SetActorEnableCollision(false);
			Flags[res[eee][0]][res[eee][1]] = 0;

		}

		round.Reset();

		if (hidden)
		{
			GameField->Cubes[res[0][0]][res[0][1]]->SetActorHiddenInGame(true);
			GameField->Cubes[res[0][0]][res[0][1]]->SetActorEnableCollision(false);
		}
		else
		{
			GameField->Cubes[res[0][0]][res[0][1]]->SetActorHiddenInGame(false);
			GameField->Cubes[res[0][0]][res[0][1]]->SetActorEnableCollision(true);

			++AmountOfConqueredCells;
		}

		//AmountOfConqueredCells += res.Num();//-
		//UE_LOG(LogTemp, Warning, TEXT("AmountOfConqueredCells %d"), AmountOfConqueredCells)

		res.Reset();

		/*
		for (int32 i = 0; i < res.Num(); ++i)
		{
			GameField->Cubes[GameField->Tail[i].Y][GameField->Tail[i].X]->SetActorHiddenInGame(false);
			GameField->Cubes[GameField->Tail[i].Y][GameField->Tail[i].X]->SetActorEnableCollision(true);

			++AmountOfConqueredCells;
		}
		*/
		
		for (int32 y = 1; y < GameField->Cubes.Num() - 1; ++y) // count amount of captured cells
		{	// TODO: Optimize this shit (// remove repeatings from the res array)
			for (int32 x = 1; x < GameField->Cubes[y].Num() - 1; ++x)
			{
				if (Flags[y][x] == 3)
				{
					++AmountOfConqueredCells;

					for (int32 i = 0; i < GameField->Hearts.Num(); ++i) // Check if we Conquered Heart 
					{ // TODO: Optimize this shit!!!
						if (GameField->Cubes[y][x]->GetActorLocation().X == GameField->Hearts[i]->GetActorLocation().X
							&&
							GameField->Cubes[y][x]->GetActorLocation().Y == GameField->Hearts[i]->GetActorLocation().Y
						) // then add live, delete Heart, SpawnSprikes
						{
							GameField->Hearts[i]->SpawnSprikes();
							GameField->Hearts[i]->Destroy();
							GameField->Hearts.RemoveAt(i);
							++Lives;
						}
					}
				}

			}
		}
		
		
		if (AmountOfConqueredCells / static_cast<float>(AmountOfCells) >= GameField->PercentageOfFieldToWin) // if PercentageOfFieldToWin of the field captured
		{
			
			for (int32 y = 1; y < GameField->Cubes.Num() - 1; ++y) // free gamefield
			{ // TODO: Endure into a separate function p1
				for (int32 x = 1; x < GameField->Cubes[y].Num() - 1; ++x)
				{
					GameField->Cubes[y][x]->SetActorHiddenInGame(true);
					GameField->Cubes[y][x]->SetActorEnableCollision(false);
					
					Flags[y][x] = 0;
				}
			}
			
			Pos = { 10, 0 }; // reset poition and counters
			ACube* a = GameField->Cubes[Pos.Y][Pos.X];
			FVector NewLocation = a->GetActorLocation();
			NewLocation.Z += GameField->sizeCell;
			SetActorLocation(NewLocation);
			currDirection = "None";
			AmountOfConqueredCells = 0;

			++Level; // next level

			// TODO: Enharder game

			TFunction<void (void)> SpawnBallFun = [this, &SpawnBallFun]() { // add one more ball
				UWorld* const World = GetWorld();
				if (World)
				{
					FVector GameFieldCenter; // TODO: Endure into a separate function p2
					FVector GameFieldBoxExtent;
					GameField->GetActorBounds(false, OUT GameFieldCenter, OUT GameFieldBoxExtent);
					FVector FlattenedExtent = GameFieldBoxExtent;
					FlattenedExtent.Y *= 0.9;
					FlattenedExtent.X *= 0.9;

					GameFieldCenter.Z = 6.866975; // TODO: remove this magic number(hight above the bottom of the field)


					FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(GameFieldCenter, FlattenedExtent);

					FRotator SpawnRotation;
					SpawnRotation.Yaw = FMath::FRand() * 360.0f;
					SpawnRotation.Pitch = FMath::FRand() * 360.0f;
					SpawnRotation.Roll = FMath::FRand() * 360.0f;

					FActorSpawnParameters SpawnParams;
					SpawnParams.Owner = this;
					SpawnParams.Instigator = Instigator;

					ACommonBall* CommonBall = World->SpawnActor<ACommonBall>(GameField->CommonBall_BP, SpawnLocation, SpawnRotation, SpawnParams);

					if (CommonBall)
					{

						//UE_LOG(LogTemp, Warning, TEXT("Spawned %s"), *CommonBall->GetActorLocation().ToString())
						GameField->CommonBalls.Add(CommonBall);
						
						//++GameField->BallsCount; // don't realy need this
					}
					else // if failed to spawn
					{ // then try again
						//UE_LOG(LogTemp, Warning, TEXT("failed to spawn"))
						SpawnBallFun();
					}
				}
			};
			SpawnBallFun(); // TODO: optimize


			//UE_LOG(LogTemp, Warning, TEXT("WIN! Next level"))
		}


	}

	for (int32 i = 0; i < GameField->Tail.Num(); ++i)
	{
		GameField->Cubes[GameField->Tail[i].Y][GameField->Tail[i].X]->Tags.RemoveSingle(FName("tail"));
	}
	
	GameField->PercentageOfConquered = AmountOfConqueredCells / static_cast<float>(AmountOfCells) * 100;
	AmountOfConqueredCells = 0;
	GameField->Tail.Reset();
}

void AMyPlayer::_Conquer(int32 y, int32 x)
{
	TArray<TArray<int32>> r = {
		{ y - 1, x - 1 },
		{ y	   , x - 1 },
		{ y + 1, x - 1 },
		{ y + 1, x },
		{ y + 1, x + 1 },
		{ y    , x + 1 },
		{ y - 1, x + 1 },
		{ y - 1, x }
	};

	//Flags[y][x] = 0;
	// 0 -- empty
	// 1 -- tail
	// 2 -- border cube
	// 3 -- filled

	//static int32 e = 0;
	//UE_LOG(LogTemp, Warning, TEXT("e %d"), e++)
	if (round.Contains(TArray<int32>({ y, x })))
	{
		return;
	}
	if (y == 0 || y == GameField->Cubes.Num() - 1 || x == 0 || x == GameField->Cubes[y].Num() - 1)
	{
		round.Add({ y, x });
		return;
	}
	round.Add({ y, x });
	res.Add({ y, x });

	for (int i = 0; i < r.Num(); i++)
	{
		TArray<int32> p = r[i];
		if (Flags.IsValidIndex(p[0]) && Flags[p[0]].IsValidIndex(p[1]) && Flags[p[0]][p[1]] == 0)
		{
			_Conquer(p[0], p[1]);
		}
	}
}

void AMyPlayer::Up()
{
	if (i_LoDCtCtDoS == LoDCtCtDoS) // it's about time to change direction of sight
	{
		InputComponent->ClearActionBindings();
		InputComponent->BindAction("Up", IE_Pressed, this, &AMyPlayer::Up);
		InputComponent->BindAction("Down", IE_Pressed, this, &AMyPlayer::Down);
		InputComponent->BindAction("Right", IE_Pressed, this, &AMyPlayer::Right);
		InputComponent->BindAction("Left", IE_Pressed, this, &AMyPlayer::Left);

		Camera->SetActorLocation(FVector(-32.841423, 109.823692, 147.206299));// TODO: remove all magic numbers(fix relative to GameField)
																			// cause it may spoil view in different screen resolutions
		Camera->SetActorRotation(FRotator(-45.192566, 0.000117, 0.000323));

		i_LoDCtCtDoS = 0;
	}
	else
	{
		++i_LoDCtCtDoS;
	}

	currDirection = "Up";

}

void AMyPlayer::Down()
{
	if (i_LoDCtCtDoS == LoDCtCtDoS) // it's about time to change direction of sight
	{
		InputComponent->ClearActionBindings();
		InputComponent->BindAction("Up", IE_Pressed, this, &AMyPlayer::Down);
		InputComponent->BindAction("Down", IE_Pressed, this, &AMyPlayer::Up);
		InputComponent->BindAction("Right", IE_Pressed, this, &AMyPlayer::Left);
		InputComponent->BindAction("Left", IE_Pressed, this, &AMyPlayer::Right);

		Camera->SetActorLocation(FVector(357.792755, 115.71772, 157.80426));// TODO: remove all magic numbers(fix relative to GameField)
																			// cause it may spoil view in different screen resolutions
		Camera->SetActorRotation(FRotator(-42.191467, 180, 0));

		i_LoDCtCtDoS = 0;
	}
	else
	{
		++i_LoDCtCtDoS;
	}

	currDirection = "Down";
}

void AMyPlayer::Right()
{
	if (i_LoDCtCtDoS == LoDCtCtDoS) // it's about time to change direction of sight
	{
		InputComponent->ClearActionBindings();
		InputComponent->BindAction("Up", IE_Pressed, this, &AMyPlayer::Right);
		InputComponent->BindAction("Down", IE_Pressed, this, &AMyPlayer::Left);
		InputComponent->BindAction("Right", IE_Pressed, this, &AMyPlayer::Down);
		InputComponent->BindAction("Left", IE_Pressed, this, &AMyPlayer::Up);

		Camera->SetActorLocation(FVector(156.046616, -39.86599, 211.64299));// TODO: remove all magic numbers(fix relative to GameField)
																			// cause it may spoil view in different screen resolutions
		Camera->SetActorRotation(FRotator(-47.395142, 90.000847, 0.000108));

		i_LoDCtCtDoS = 0;
	}
	else
	{
		++i_LoDCtCtDoS;
	}

	currDirection = "Right";
}

void AMyPlayer::Left()
{
	if (i_LoDCtCtDoS == LoDCtCtDoS) // it's about time to change direction of sight
	{
		InputComponent->ClearActionBindings();
		InputComponent->BindAction("Up", IE_Pressed, this, &AMyPlayer::Left);
		InputComponent->BindAction("Down", IE_Pressed, this, &AMyPlayer::Right);
		InputComponent->BindAction("Right", IE_Pressed, this, &AMyPlayer::Up);
		InputComponent->BindAction("Left", IE_Pressed, this, &AMyPlayer::Down);

		Camera->SetActorLocation(FVector(157.729156, 236.215561, 214.959976));// TODO: remove all magic numbers(fix relative to GameField)
																			// cause it may spoil view in different screen resolutions
		Camera->SetActorRotation(FRotator(-53.595673, -89.998337, 0.000095));

		i_LoDCtCtDoS = 0;
	}
	else
	{
		++i_LoDCtCtDoS;
	}

	currDirection = "Left";
}

FString AMyPlayer::GetState() const
{
	FString res = "";
	res += "Level: " + FString::FromInt(Level);
	res += "      Lives: " + FString::FromInt(Lives);
	res += "      Conquered: " + FString::SanitizeFloat(GameField->PercentageOfConquered) + "%";
	return res;
}