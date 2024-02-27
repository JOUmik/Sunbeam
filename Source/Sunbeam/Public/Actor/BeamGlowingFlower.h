// Copyright Project SunBeam. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actor/BeamFlowerBase.h"
#include "BeamGlowingFlower.generated.h"

/**
 * 
 */
UCLASS()
class SUNBEAM_API ABeamGlowingFlower : public ABeamFlowerBase, public ILightSource
{
	GENERATED_BODY()
public:
	ABeamGlowingFlower();

	/* ILightSource interface */
	virtual FGameplayTag GetLightSourceTag_Implementation() const override;
	virtual bool CanInteractWithActor_Implementation(AActor* OtherActor) const override;
	/* End ILightSource interface */

	UFUNCTION()
	void OnGlowingRadiusBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
									 const FHitResult& SweepResult);

	UFUNCTION()
	void OnGlowingRadiusEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Flower")
	FGameplayTag SecondaryLightSourceTag;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flower")
	TObjectPtr<USphereComponent> GlowingRadiusComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Flower")
	TSet<AActor*> CurOverlappingInteractables;

private:
	
	UFUNCTION()
	void OnBloomStatusChanged(bool bBloomed);

	
};
