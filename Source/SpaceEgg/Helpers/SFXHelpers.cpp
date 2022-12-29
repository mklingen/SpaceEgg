#include "SFXHelpers.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

USFXHelpers::USFXHelpers()
{

}

bool USFXHelpers::PlayAudio(const FVector& pos, float volume, float pitch) const
{
	if (Audio)
	{
		UGameplayStatics::PlaySoundAtLocation(this, Audio, pos, volume, pitch, 0.0f, AudioAttenuation);
		return true;
	}
	return false;
}

bool USFXHelpers::MakeParticles(const FVector& pos, const FRotator& rot, const FVector& scale) const
{
	if (Particles)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, Particles,
			pos, rot, scale, true, true);
		return true;
	}
	return false;
}

bool USFXHelpers::Play(const FVector& pos, const FRotator& rot, const FVector& scale, float volume, float pitch) const
{
	bool audioPlayed = PlayAudio(pos, volume, pitch);
	bool particlesPlayed = MakeParticles(pos, rot, scale);
	return audioPlayed || particlesPlayed;
}
