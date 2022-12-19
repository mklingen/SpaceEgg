// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Toggleable.h"
#include "ColorChangerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEEGG_API UColorChangerComponent : public UActorComponent, public IToggleable
{
	GENERATED_BODY()

public:
	UColorChangerComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mesh")
		UStaticMeshComponent* mesh = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "State")
		int State = false;

	void Update();

public:

	virtual int GetState_Implementation() const override;
	virtual void SetState_Implementation(int value) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Color Change")
		int MaterialIndex = 0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Color Change")
		TArray<UMaterialInterface*> Materials;
};
