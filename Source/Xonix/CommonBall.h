// © 2019 Cul_Team All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CommonBall.generated.h"

UCLASS()
class XONIX_API ACommonBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACommonBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* CommonBallMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitting", meta = (AllowPrivateAccess = "true"))
	float Velocity = 125; 

	UFUNCTION(BlueprintCallable, Category = "Hitting", meta = (AllowPrivateAccess = "true"))
	void Energize(
		UPrimitiveComponent*	HitComponent,
		AActor*					OtherActor,
		UPrimitiveComponent*	OtherComponent,
		FVector					NormalImpulse,
		const FHitResult&		Hit
	);

	class AGameField* GameField;

	class AMyPlayer* MyPlayer;
};
