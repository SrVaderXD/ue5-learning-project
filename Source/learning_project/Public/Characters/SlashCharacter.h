#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "SlashCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class LEARNING_PROJECT_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* SlashMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* SlashMoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* SlashLookAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);


public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
