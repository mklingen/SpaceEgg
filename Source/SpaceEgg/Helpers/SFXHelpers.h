#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SFXHelpers.generated.h"

class USoundCue;
class UNiagaraSystem;
class USoundAttenuation;

UCLASS()
class SPACEEGG_API USFXHelpers : public UObject
{
	GENERATED_BODY()

public:
	// Sets default values for this empty's properties
	USFXHelpers();

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "SFX")
		USoundCue* Audio = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "SFX")
		UNiagaraSystem* Particles = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "SFX")
		USoundAttenuation* AudioAttenuation = nullptr;

	bool PlayAudio(const FVector& pos, float volume = 1.0f, float pitch = 1.0f) const;
	bool MakeParticles(const FVector& pos, const FRotator& rot = FRotator::ZeroRotator, const FVector& scale = FVector::OneVector) const;
	bool Play(const FVector& pos, const FRotator& rot = FRotator::ZeroRotator, const FVector& scale = FVector::OneVector, float volume = 1.0f, float pitch = 1.0f) const;
};
