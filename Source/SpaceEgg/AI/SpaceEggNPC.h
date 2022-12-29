// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpaceEggNPC.generated.h"

class UHealthComponent;
class UNPCAttackComponent;
class UDeathSFXComponent;
class UDamageSFXComponent;

UCLASS()
class SPACEEGG_API ASpaceEggNPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASpaceEggNPC();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		UHealthComponent* Health = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		UNPCAttackComponent* Attack = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
		UDamageSFXComponent* DamageSFX = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
		UDeathSFXComponent* DeathSFX = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
