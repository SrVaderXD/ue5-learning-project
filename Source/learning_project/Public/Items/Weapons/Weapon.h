#pragma once

#include "CoreMinimal.h"
#include "Items/Items.h"
#include "Weapon.generated.h"

class USoundBase;
class UBoxComponent;

UCLASS()
class LEARNING_PROJECT_API AWeapon : public AItems
{
	GENERATED_BODY()

public:
	AWeapon();
	void Equip(USceneComponent* Parent, FName SocketName);
	void AttachMeshToSocket(USceneComponent* Parent, const FName& SocketName);
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }

protected:
	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* EquipSound;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent* WeaponBox;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;
};
