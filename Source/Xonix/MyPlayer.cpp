// © 2019 Cul_Team All Rights Reserved

#include "MyPlayer.h"
#include "GameField.h"
#include "Cube.h"
#include "CommonBall.h"


// Sets default values
AMyPlayer::AMyPlayer()
{

}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}

void AMyPlayer::SetPos(const FVector2D& Pos) // TODO : Implement smooth movement
{

}

void AMyPlayer::Conquer()
{
	// TODO: implement 
}


void AMyPlayer::Up()
{

	currDirection = "Up";

}

void AMyPlayer::Down()
{

	currDirection = "Down";
}

void AMyPlayer::Right()
{

	currDirection = "Right";
}

void AMyPlayer::Left()
{


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