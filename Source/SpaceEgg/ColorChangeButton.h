// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveObject.h"
#include "Components/StaticMeshComponent.h"
#include "ColorChangerComponent.h"
#include "Toggleable.h"
#include "ColorChangeButton.generated.h"

UENUM()
enum ButtonState
{
	Off     UMETA(DisplayName = "Off"),
	On      UMETA(DisplayName = "On"),
	On2 UMETA(DisplayName = "On2")
};

UCLASS()
class SPACEEGG_API AColorChangeButton : public AInteractiveObject, public IToggleable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AColorChangeButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Mesh")
		UStaticMeshComponent* mesh = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "State")
		TEnumAsByte<ButtonState> state;

public:	


	virtual int GetState_Implementation() const override;
	virtual void SetState_Implementation(int value) override;

	UFUNCTION(BlueprintCallable)
		void Toggle();

	UFUNCTION(BlueprintCallable)
		void SetButtonState(TEnumAsByte<ButtonState> buttonState);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Color Change")
		UColorChangerComponent* colorChanger = nullptr;
};
