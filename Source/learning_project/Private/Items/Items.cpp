#include "Items/Items.h"
#include "learning_project/DebugMacros.h"

AItems::AItems()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;
}

void AItems::BeginPlay()
{
	Super::BeginPlay();
}

float AItems::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItems::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AItems::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;

	//float DeltaZ = TransformedSin(RunningTime);

	//AddActorWorldOffset(FVector(0.f, 0.f, DeltaZ));

	AddActorWorldRotation(FRotator(RotationPitch * DeltaTime, RotationYaw * DeltaTime, RotationRoll * DeltaTime));

	DRAW_SPHERE_SingleFrame(GetActorLocation());
	DRAW_VECTOR_SingleFrame(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f);

	FVector AvgVector = Avg<FVector>(GetActorLocation(), FVector::ZeroVector);
	DRAW_POINT_SingleFrame(AvgVector);
}

