#include "Characters/SlashCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "GroomComponent.h"

#include "Items/Items.h"
#include "Items/Weapons/Weapon.h"

#include "Animation/AnimMontage.h"

#include "Components/BoxComponent.h"

ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 200.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("EyeBrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");
}

void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(SlashMappingContext, 0);
		}
	}
	
}

void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(SlashMoveAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Move);
		EnhancedInputComponent->BindAction(SlashLookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);
		EnhancedInputComponent->BindAction(SlashJumpAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Jump);
		EnhancedInputComponent->BindAction(SlashEKeyAction, ETriggerEvent::Triggered, this, &ASlashCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(SlashAttackAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Attack);
	}
}

void ASlashCharacter::Move(const FInputActionValue& Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;

	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector SideDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	const FVector2D MovementVector = Value.Get<FVector2D>();

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(SideDirection, MovementVector.X);
}

void ASlashCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

void ASlashCharacter::EKeyPressed()
{
	AWeapon* OverlappedWeapon = Cast<AWeapon>(OverlappedItem);
	if (OverlappedWeapon)
	{
		OverlappedWeapon->Equip(GetMesh(), FName("hand_r_socket"));
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		OverlappedItem = nullptr;
		EquippedWeapon = OverlappedWeapon;
	}

	else
	{
		if (CanSheath())
		{
			PlayDrawSheathMontage(FName("Sheath"));
			CharacterState = ECharacterState::ECS_Unarmed;
			ActionState = EActionState::EAS_DrawingSheathingSword;
		}
		else if (CanDraw())
		{
			PlayDrawSheathMontage(FName("Draw"));
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			ActionState = EActionState::EAS_DrawingSheathingSword;
		}
	}
}

void ASlashCharacter::Attack()
{
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

bool ASlashCharacter::CanAttack()
{
	return (
		ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unarmed
		);
}

void ASlashCharacter::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		const int32 Selection = FMath::RandRange(0, 2);
		FName SectionName = FName();

		switch (Selection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		case 2:
			SectionName = FName("Attack3");
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void ASlashCharacter::AttackMontageEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool ASlashCharacter::CanSheath()
{
	return (
		ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unarmed
		);
}

bool ASlashCharacter::CanDraw()
{
	return (
		ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unarmed &&
		EquippedWeapon
	);
}

void ASlashCharacter::SheathSword()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("PelvisSocket"));
	}
}

void ASlashCharacter::DrawSword()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("hand_r_socket"));
	}
}

void ASlashCharacter::FinishDrawingSword()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::PlayDrawSheathMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DrawSheathMontage)
	{
		AnimInstance->Montage_Play(DrawSheathMontage);
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void ASlashCharacter::SetWeaponCollision(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
	}
}
