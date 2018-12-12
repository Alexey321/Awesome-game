// © 2019 Cul_Team All Rights Reserved

#include "Cube.h"

#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"

// Sets default values
ACube::ACube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/Meshes/Cube.Cube'"));
	CubeMesh->SetStaticMesh(MeshObj.Object);
	SetRootComponent(CubeMesh);

	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/Meshes/Green.Green'"));
	CubeMesh->SetMaterial(0, Material.Object);

}

// Called when the game starts or when spawned
void ACube::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
