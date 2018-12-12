// © 2019 Cul_Team All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Heart.generated.h"

UCLASS()
class XONIX_API AHeart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHeart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnSprikes() const;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* HeartMesh;
	
	UFUNCTION(BlueprintCallable, Category = "Hitting", meta = (AllowPrivateAccess = "true"))
	void OnHit(
		UPrimitiveComponent*	HitComponent,
		AActor*					OtherActor,
		UPrimitiveComponent*	OtherComponent,
		FVector					NormalImpulse,
		const FHitResult&		Hit
	);

	class AMyPlayer* MyPlayer;
	class AGameField* GameField;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* ParticleSystem; // spawn sprikes when heart destroyed
};
