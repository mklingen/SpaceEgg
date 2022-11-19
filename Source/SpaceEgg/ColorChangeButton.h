// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveObject.h"
#include "Components/StaticMeshComponent.h"
#include "ColorChangeButton.generated.h"

UENUM()
enum ButtonState
{
	Off     UMETA(DisplayName = "Off"),
	On      UMETA(DisplayName = "On"),
};

UCLASS()
class SPACEEGG_API AColorChangeButton : public AInteractiveObject
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

		
	UFUNCTION(BlueprintCallable)
		void Toggle();

	UFUNCTION(BlueprintCallable)
		void SetState(TEnumAsByte<ButtonState> buttonState);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Color Change")
		int MaterialIndex = 0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Color Change")
		UMaterialInterface* OffMaterial = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Color Change")
		UMaterialInterface* OnMaterial = nullptr;
};
