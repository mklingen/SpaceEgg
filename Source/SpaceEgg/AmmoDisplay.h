// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AmmoDisplay.generated.h"

class UTP_WeaponComponent;
class UTextRenderComponent;

UENUM()
enum AmmoDisplayType
{
	Ammo     UMETA(DisplayName = "Ammo"),
	Charge      UMETA(DisplayName = "Charge")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEEGG_API UAmmoDisplay : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAmmoDisplay();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	int lastCheckedAmmo = -1;
	float timeAmmoChanged = 0;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Display")
		TEnumAsByte<AmmoDisplayType> DisplayType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Display")
		FString Symbol = TEXT("|");
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Display")
		FString OffSymbol = TEXT(" ");
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Display")
		int NumSymbols = 5;

	UPROPERTY()
		UTP_WeaponComponent* Weapon = nullptr;
	UPROPERTY()
		UTextRenderComponent* TextRenderer = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Color")
		FColor AmmoGreaterThanZeroColor = FColor(255, 255, 255, 255);
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Color")
		FColor AmmoZeroColor = FColor(255, 0, 0, 255);
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Color")
		FColor FlashColor = FColor(255, 255, 255, 255);
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Color")
		float FlashTime = 0.25f;
};
