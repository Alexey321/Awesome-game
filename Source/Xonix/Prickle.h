// © 2019 Cul_Team All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Prickle.generated.h"

UCLASS()
class XONIX_API APrickle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APrickle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PrickleMesh;
	
	UFUNCTION(BlueprintCallable, Category = "Hitting", meta = (AllowPrivateAccess = "true"))
	void OnHit(
		UPrimitiveComponent*	HitComponent,
		AActor*					OtherActor,
		UPrimitiveComponent*	OtherComponent,
		FVector					NormalImpulse,
		const FHitResult&		Hit
	);


	class AGameField* GameField;

	class AMyPlayer* MyPlayer;

};

