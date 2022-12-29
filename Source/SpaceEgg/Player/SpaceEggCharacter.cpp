// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpaceEggCharacter.h"
#include "SpaceEgg/Actors/SpaceEggProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "GameFramework/InputSettings.h"
#include "TriggerInteractionsComponent.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "SpaceEgg/Helpers/ActorHelpers.h"

namespace
{
	void SetOutlinesRecursive(AActor* actor, bool turnOn)
	{
		TArray<UPrimitiveComponent*> primitives = UActorHelpers::FindComponentsRecursive<UPrimitiveComponent>(actor);
		for (UPrimitiveComponent* primitive : primitives)
		{
			primitive->SetRenderCustomDepth(turnOn);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// ASpaceEggCharacter

ASpaceEggCharacter::ASpaceEggCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// set our turn rates for input
	TurnRateGamepad = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	WidgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Widget Interaction"));
	WidgetInteraction->InteractionSource = EWidgetInteractionSource::CenterScreen;
	ObjectInteraction = CreateDefaultSubobject<UTriggerInteractionsComponent>(TEXT("Object Interaction"));

	PrimaryActorTick.bCanEverTick = true;

}

void ASpaceEggCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	ui = CreateWidget<UUserWidget>(GetWorld(), UIClass);
	
	if (ui)
	{
		ui->AddToViewport();
		reticle = ui->WidgetTree->FindWidget<UBorder>(FName("Reticle"));
	}

}

//////////////////////////////////////////////////////////////////////////// Input

void ASpaceEggCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &ASpaceEggCharacter::ShootPressed);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &ASpaceEggCharacter::ShootReleased);
	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &ASpaceEggCharacter::OnPrimaryAction);
	PlayerInputComponent->BindAction("PrimaryAction", IE_Released, this, &ASpaceEggCharacter::OnPrimaryActionReleased);



	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ASpaceEggCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ASpaceEggCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Mouse" versions handle devices that provide an absolute delta, such as a mouse.
	// "Gamepad" versions are for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ASpaceEggCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ASpaceEggCharacter::LookUpAtRate);
}

void ASpaceEggCharacter::OnPrimaryActionReleased()
{
	WidgetInteraction->ReleasePointerKey(EKeys::LeftMouseButton);
}


void ASpaceEggCharacter::OnPrimaryAction()
{
	// Trigger the OnItemUsed Event
	OnUseItem.Broadcast();
	WidgetInteraction->PressPointerKey(EKeys::LeftMouseButton);
	ObjectInteraction->Trigger();
}

void ASpaceEggCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnPrimaryAction();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void ASpaceEggCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

void ASpaceEggCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ASpaceEggCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ASpaceEggCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ASpaceEggCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

bool ASpaceEggCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ASpaceEggCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &ASpaceEggCharacter::EndTouch);

		return true;
	}
	
	return false;
}


void ASpaceEggCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (reticle)
	{
		AActor* actorHit = nullptr;
		auto underMouse = ObjectInteraction->GetObjectUnderCursorOrNull(actorHit);

		if (underMouse)
		{
			FSlateBrush brush = reticle->Background;
			brush.OutlineSettings.Color = FLinearColor(1, 1, 1, 0.8f);
			reticle->SetBrush(brush);
		}
		else
		{
			FSlateBrush brush = reticle->Background;
			brush.OutlineSettings.Color = FLinearColor(1, 1, 1, 0.3f);
			reticle->SetBrush(brush);
		}
		if (actorHit != HighlightedActor)
		{
			if (HighlightedActor)
			{
				SetOutlinesRecursive(HighlightedActor, false);
			}

			HighlightedActor = actorHit;
			
			if (HighlightedActor)
			{
				SetOutlinesRecursive(HighlightedActor, true);
			}
		}
	}
}

void ASpaceEggCharacter::ShootPressed()
{
	OnShoot.Broadcast();
}

void ASpaceEggCharacter::ShootReleased()
{
	OnShootReleased.Broadcast();
}