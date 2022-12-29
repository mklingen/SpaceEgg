// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoDisplay.h"

#include "Components/TextRenderComponent.h"
#include "SpaceEgg/Player/TP_WeaponComponent.h"

// Sets default values for this component's properties
UAmmoDisplay::UAmmoDisplay()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAmmoDisplay::BeginPlay()
{
	Super::BeginPlay();

	Weapon = GetOwner()->FindComponentByClass<UTP_WeaponComponent>();
	if (!Weapon)
	{
		auto parent = GetOwner()->GetParentActor();
		if (parent)
		{
			Weapon = parent->FindComponentByClass<UTP_WeaponComponent>();
		}
	}
	TextRenderer = GetOwner()->FindComponentByClass<UTextRenderComponent>();
}


// Called every frame
void UAmmoDisplay::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!Weapon)
	{
		return;
	}
	int ammo = Weapon->Ammo;
	if (DisplayType == AmmoDisplayType::Charge)
	{
		ammo = static_cast<int>((NumSymbols) * (Weapon->Charge / Weapon->MaxCharge));
	}
	if (lastCheckedAmmo != ammo)
	{
		lastCheckedAmmo = ammo;
		timeAmmoChanged = GetWorld()->GetTimeSeconds();
		if (TextRenderer)
		{
			if (DisplayType == AmmoDisplayType::Ammo)
			{
				TextRenderer->Text = FText::FromString(FString::Printf(TEXT("%d"), lastCheckedAmmo));
			}
			else
			{
				FString displayString;
				for (int i = 0; i < lastCheckedAmmo; i++)
				{
					displayString += Symbol;
				}
				for (int k = lastCheckedAmmo; k < NumSymbols; k++)
				{
					displayString += OffSymbol;
				}
				TextRenderer->Text = FText::FromString(displayString);
			}
		}
	}
	float t = GetWorld()->GetTimeSeconds();
	if (t - timeAmmoChanged < FlashTime)
	{
		TextRenderer->SetTextRenderColor(FlashColor);
	}
	else
	{
		if (DisplayType == Ammo)
		{
			TextRenderer->SetTextRenderColor(lastCheckedAmmo > 0 ? this->AmmoGreaterThanZeroColor : this->AmmoZeroColor);
		}
		else
		{
			TextRenderer->SetTextRenderColor(lastCheckedAmmo >= NumSymbols ? this->AmmoZeroColor : this->AmmoGreaterThanZeroColor);
		}
	}
}

