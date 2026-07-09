#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;

UCLASS()
class LEARNING_PROJECT_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	virtual void Tick(float DeltaTime) override;
	ABreakableActor();


	virtual void GetHit(const FVector& ImpactPoint) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* GeometryCollection;
};
