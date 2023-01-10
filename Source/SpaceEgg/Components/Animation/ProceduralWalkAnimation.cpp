// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralWalkAnimation.h"

#include "SpaceEgg/Components/Animation/ProceduralAnimationComponent.h"
#include "DrawDebugHelpers.h"
#include "SpaceEgg/Helpers/DebugDrawHelpers.h"
#include "SpaceEgg/Helpers/PrintHelper.h"
#include "Animation/PoseAsset.h"

namespace
{
	FTransform GetA_T_C(const FTransform& a_T_b, const FTransform& b_T_c)
	{
		FTransform a_T_c = FTransform(a_T_b.ToMatrixNoScale() * b_T_c.ToMatrixNoScale());
		return a_T_c;
	}
}

// Sets default values for this component's properties
UProceduralWalkAnimation::UProceduralWalkAnimation()
{


}


void FSimpleBoneChain::Initialize(USkeletalMeshComponent* skeleton)
{
	RootBone = FBoneReference(RootBoneName);
	EndBone = FBoneReference(EndBoneName);
	RootBone.Initialize(skeleton->SkeletalMesh->Skeleton);
	EndBone.Initialize(skeleton->SkeletalMesh->Skeleton);
	if (RootBone.BoneIndex < 0)
	{
		LOGE("Root bone %s didn't exist.", *(RootBoneName.ToString()));
	}
	if (EndBone.BoneIndex < 0)
	{
		LOGE("End Bone bone %s didn't exist.", *(EndBoneName.ToString()));
	}
	NominalPoseEndEwrtRoot = GetRootTBoneRecursive(EndBone.BoneIndex, RootBone.BoneIndex, skeleton);
	NominalLength = NominalPoseEndEwrtRoot.GetLocation().Length();
}

FTransform FSimpleBoneChain::GetRootTBoneRecursive(int boneIdx, int rootIdx, USkeletalMeshComponent* skeleton) const
{
	// Initialize the relative transform to the identity
	FTransform rootTBone(FQuat::Identity, FVector::ZeroVector, FVector::OneVector);

	// Start at the target bone and work up the hierarchy towards the root bone
	int currentBone = boneIdx;
	while (currentBone != rootIdx)
	{
		// Get the reference transform of the bone in local space
		FTransform parentTBone = skeleton->GetRefPoseTransform(currentBone);

		// Pre-multiply the relative transform by the local transform to accumulate the relative transform from the root bone to the target bone
		rootTBone = GetA_T_C(rootTBone, parentTBone);

		// Get the parent bone of the current bone
		FName parentName = skeleton->GetParentBone(skeleton->GetBoneName(currentBone));
		currentBone = skeleton->GetBoneIndex(parentName);

		// If the current bone has no parent, we have reached the root of the skeleton
		if (currentBone < 0)
		{
			break;
		}
	}

	return rootTBone;
}

FTransform UProceduralWalkAnimation::ComputeNominalHandPose(const FSimpleBoneChain& chain, const FRotator& armNominalRot, USkeletalMeshComponent* mesh)
{
	FTransform skeletonTRootNominal = GetA_T_C(mesh->GetRefPoseTransform(0), chain.GetRootTBoneRecursive(chain.RootBone.BoneIndex, 0, mesh));
	if (DebugDrawNominalShoulderPose)
	{
		DebugDrawPose(GetWorld(), GetA_T_C(mesh->GetComponentTransform(), skeletonTRootNominal), 25);
	}
	FRotator rootNominalRRootRotated = armNominalRot;
	FTransform skeletonTRootRotated(rootNominalRRootRotated.Quaternion() * skeletonTRootNominal.GetRotation(), skeletonTRootNominal.GetLocation());
	if (DebugDrawNominalShoulderPose)
	{
		DebugDrawPose(GetWorld(), GetA_T_C(mesh->GetComponentTransform(), skeletonTRootRotated), 25);
	}
	FTransform skeletonTHandRotated = GetA_T_C(chain.NominalPoseEndEwrtRoot, skeletonTRootRotated);
	if (DebugDrawNominalHandPose)
	{
		DebugDrawPose(GetWorld(), GetA_T_C(mesh->GetComponentTransform(), skeletonTHandRotated), 25);
	}
	return skeletonTHandRotated;
}

void UProceduralWalkAnimation::ComputeNominalHandPoses(USkeletalMeshComponent* mesh)
{
	RightHandNominalEwrtBody = ComputeNominalHandPose(RightArm, ArmNominalSwingRotRight, mesh);
	RightHandNominalEwrtBody.AddToTranslation(ArmNominalSwingTransRight);
	LeftHandNominalEwrtBody = ComputeNominalHandPose(LeftArm, ArmNominalSwingRotLeft, mesh);
	LeftHandNominalEwrtBody.AddToTranslation(ArmNominalSwingTransLeft);
}

// Called when the game starts
void UProceduralWalkAnimation::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	TimeLastPhaseStarted = GetWorld()->GetTimeSeconds();
	Phase = 0.0f;
	UpdateNextStance(ProceduralGaitPhase::Left);
	UpdateNextStance(ProceduralGaitPhase::Left);
	UpdateNextStance(ProceduralGaitPhase::Right);
	UpdateNextStance(ProceduralGaitPhase::Right);
	FTransform actorTransform = GetActorTransformOrIdentity();
	GazeTargetEwrtWorld = actorTransform.TransformPosition(FVector::UnitX() * 500 + FVector::UnitZ() * 90);
	APawn* owner = TryGetPawnOwner();
	if (owner)
	{
		animationComponent = owner->FindComponentByClass<UProceduralAnimationComponent>();
	}
	USkeletalMeshComponent* meshComponent = GetOuterUSkeletalMeshComponent();
	if (meshComponent)
	{
		USkeletalMesh* mesh = meshComponent->SkeletalMesh.Get();
		if (mesh)
		{
			LeftArm.Initialize(meshComponent);
			RightArm.Initialize(meshComponent);
			ComputeNominalHandPoses(meshComponent);
			LeftLeg.Initialize(meshComponent);
			RightLeg.Initialize(meshComponent);
			NominalStanceHeight = LeftLeg.GetRootTBoneRecursive(LeftLeg.EndBone.BoneIndex, 0, meshComponent).GetLocation().Length();
		}
	}
	LeftHandTargetEwrtWorld = EvaluateHandSwing(actorTransform, LeftHandNominalEwrtBody, 0.5f);
	RightHandTargetEwrtWorld = EvaluateHandSwing(actorTransform, RightHandNominalEwrtBody, 0.5f);
}

void UProceduralWalkAnimation::DoFootIK()
{
	const float t = GetWorld()->GetTimeSeconds();
	switch (GaitPhase)
	{
	case ProceduralGaitPhase::Rest:
	{
		if (Phase > 1.0f)
		{
			TimeLastPhaseStarted = t;
		}
		if (!IsStopped)
		{
			GaitPhase = ProceduralGaitPhase::Right;
			TimeLastPhaseStarted = t;
		}
		break;
	}
	case ProceduralGaitPhase::Left:
	{
		if (Phase > 1.0f)
		{
			TimeLastPhaseStarted = t;
			GaitPhase = ProceduralGaitPhase::Right;
			Stance = ProceduralStancePhase::Down;
			break;
		}
		if (Stance == ProceduralStancePhase::Down)
		{
			UpdateNextStance(GaitPhase);
			if (IsStopped && GaitPhaseOnStop != GaitPhase)
			{
				if ((LeftFootState.CurrentTargetEwrtWorld.GetLocation() - LeftFootState.NextTargetEwrtWorld.GetLocation()).Length() < 10)
				{
					GaitPhase = ProceduralGaitPhase::Rest;
					break;
				}
			}
			Stance = ProceduralStancePhase::Up;
			break;
		}
		EvaluateSwingPhase(&LeftFootState);
		break;
	}
	case ProceduralGaitPhase::Right:
	{
		if (Phase > 1.0f)
		{
			TimeLastPhaseStarted = t;
			GaitPhase = ProceduralGaitPhase::Left;
			Stance = ProceduralStancePhase::Down;
			break;
		}
		if (Stance == ProceduralStancePhase::Down)
		{
			UpdateNextStance(GaitPhase);
			if (IsStopped && GaitPhaseOnStop != GaitPhase)
			{
				if ((RightFootState.CurrentTargetEwrtWorld.GetLocation() - RightFootState.NextTargetEwrtWorld.GetLocation()).Length() < 10)
				{
					GaitPhase = ProceduralGaitPhase::Rest;
					break;
				}
			}
			Stance = ProceduralStancePhase::Up;
			break;
		}
		EvaluateSwingPhase(&RightFootState);
		break;
	}
	}
}

FTransform UProceduralWalkAnimation::GetActorTransformOrIdentity() const
{
	APawn* owner = TryGetPawnOwner();
	USkeletalMeshComponent* mesh = GetOuterUSkeletalMeshComponent();
	if (mesh)
	{
		return mesh->GetComponentTransform();
	}
	return FTransform::Identity;
}

void UProceduralWalkAnimation::DoHandIK()
{
	USkeletalMeshComponent* mesh = GetOuterUSkeletalMeshComponent();
	if (mesh)
	{
		RightHandNominalEwrtBody = ComputeNominalHandPose(RightArm, ArmNominalSwingRotRight, mesh);
		RightHandNominalEwrtBody.AddToTranslation(ArmNominalSwingTransRight);
		LeftHandNominalEwrtBody = ComputeNominalHandPose(LeftArm, ArmNominalSwingRotLeft, mesh);
		LeftHandNominalEwrtBody.AddToTranslation(ArmNominalSwingTransLeft);
	}
	const float t = GetWorld()->GetTimeSeconds();
	float phaseLeft = Phase;
	float phaseRight = Phase;
	switch (GaitPhase)
	{
	case ProceduralGaitPhase::Rest:
	{
		FTransform actorTransform = GetActorTransformOrIdentity();
		if (ControlRightHandTarget)
		{
			LeftHandTargetEwrtWorld = EvaluateHandSwing(actorTransform, RightHandNominalEwrtBody, 0.5f);
		}
		if (ControlLeftHandTarget)
		{
			RightHandTargetEwrtWorld = EvaluateHandSwing(actorTransform, LeftHandNominalEwrtBody, 0.5f);
		}
		break;
	}
	case ProceduralGaitPhase::Left:
	{
		phaseRight = 1.0f - phaseRight;
		break;
	}
	case ProceduralGaitPhase::Right:
	{
		phaseLeft = 1.0f - phaseLeft;
		break;
	}
	}
	FTransform actorTransform = GetActorTransformOrIdentity();
	if (ControlRightHandTarget)
	{
		LeftHandTargetEwrtWorld = EvaluateHandSwing(actorTransform, RightHandNominalEwrtBody, phaseRight);
	}
	if (ControlLeftHandTarget)
	{
		RightHandTargetEwrtWorld = EvaluateHandSwing(actorTransform, LeftHandNominalEwrtBody, phaseLeft);
	}
}

float UProceduralWalkAnimation::GetRelativeBodyBounce()
{
	float tBounce = 0.0f;
	if (BodyBounceCurve)
	{
		tBounce = BodyBounceCurve->GetFloatValue(Phase);
	}
	else
	{
		tBounce = 0.5 + 0.5 * cos(Phase * 2 * PI);
	}
	return  tBounce * BodyBounceHeight;
}

void UProceduralWalkAnimation::DoBodyBounce()
{
	FTransform actorTransform = GetActorTransformOrIdentity();
	USkeletalMeshComponent* mesh = GetOuterUSkeletalMeshComponent();
	if (!mesh)
	{
		return;
	}
	BodyTargetEwrtWorld = mesh->GetComponentTransform() * mesh->GetRefPoseTransform(0);
	BodyTargetEwrtWorld.SetComponents(BodyTargetEwrtWorld.GetRotation(), BodyTargetEwrtWorld.GetTranslation() +
		FVector::UnitZ() * GetRelativeBodyBounce(), BodyTargetEwrtWorld.GetScale3D());
}

// Called every frame
void UProceduralWalkAnimation::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (animationComponent)
	{
		DesiredVelocity = animationComponent->DesiredVelocity;
		DesiredYawVelocity = animationComponent->DesiredYawVelocity;
	}
	bool noDesiredMovement = (DesiredVelocity.SquaredLength() < 10 &&
		FMath::Abs(DesiredYawVelocity) < 10);
	if (noDesiredMovement && !IsStopped)
	{
		Stop();
	}
	else if (IsStopped && !noDesiredMovement)
	{
		IsStopped = false;
	}

	if (IsStopped)
	{
		if (GaitPhase == ProceduralGaitPhase::Rest)
		{
			GaitTime = StoppedGaitTime;
		}
		else
		{
			GaitTime = StoppedGaitTime / 4;
		}
	}

	const float t = GetWorld()->GetTimeSeconds();
	Phase = (t - TimeLastPhaseStarted) / GaitTime;
	if (ControlGazeTarget)
	{
		FTransform actorTransform = GetActorTransformOrIdentity();
		GazeTargetEwrtWorld = actorTransform.TransformPosition(GazePointEwrtBodyDuringWalk);
	}
	DoFootIK();
	DoBodyBounce();
	DoHandIK();
}

FTransform UProceduralWalkAnimation::EvaluateHandSwing(const FTransform& worldTformActor, const FTransform& nominalEwrtActor, float phase)
{
	float tSwing = phase;
	if (ArmSwingCurve)
	{
		tSwing = ArmSwingCurve->GetFloatValue(tSwing);
	}
	if (IsStopped)
	{
		tSwing = 0.5f;
	}
	FVector nominalPt = GetA_T_C(nominalEwrtActor, worldTformActor).GetLocation();
	FQuat nominalRot = GetA_T_C(nominalEwrtActor, worldTformActor).GetRotation();
	FVector fwd = worldTformActor.TransformVector(FVector::UnitY());
	FVector ta = nominalPt - fwd * ArmSwingDist * 0.5f;
	FVector tb = nominalPt + fwd * ArmSwingDist * 0.5f;
	FQuat ra = nominalRot;
	FQuat rb = nominalRot;
	FVector interpPos = FMath::Lerp(ta, tb, tSwing);
	interpPos += FVector(0, 0, GetRelativeBodyBounce());
	FQuat interpRot = FQuat::Slerp(ra, rb, tSwing).GetNormalized();
	return FTransform(interpRot, interpPos);
}

void UProceduralWalkAnimation::EvaluateSwingPhase(FFootState* footState)
{
	float tSwing = Phase;
	if (FootSwingCurve)
	{
		tSwing = FootSwingCurve->GetFloatValue(tSwing);
	}
	FVector interpPos = footState->CurrentSwingTarget.GetLocation();
	FQuat interpRot = footState->CurrentSwingTarget.GetRotation();
	if (tSwing < 0.5)
	{
		interpPos = FMath::Lerp(footState->PrevTargetEwrtWorld.GetLocation(), footState->CurrentSwingTarget.GetLocation(), tSwing * 2);
		interpRot = FQuat::Slerp(footState->PrevTargetEwrtWorld.GetRotation(), footState->CurrentSwingTarget.GetRotation(), tSwing * 2).GetNormalized();
	}
	else
	{
		interpPos = FMath::Lerp(footState->CurrentSwingTarget.GetLocation(), footState->NextTargetEwrtWorld.GetLocation(), tSwing * 2 - 1.0);
		interpRot = FQuat::Slerp(footState->CurrentSwingTarget.GetRotation(), footState->NextTargetEwrtWorld.GetRotation(), tSwing * 2 - 1.0).GetNormalized();
	}
	footState->CurrentTargetEwrtWorld = FTransform(interpRot, interpPos);
}

void UProceduralWalkAnimation::UpdateNextStance(ProceduralGaitPhase forPhase)
{
	FTransform actorTransform = GetActorTransformOrIdentity();
	// TODO: raycasting to terrain and such.
	const FTransform tCenter = actorTransform;
	FVector vel = DesiredVelocity * GaitTime;
	if (vel.Length() > 500)
	{
		vel.Normalize();
		vel *= 500;
	}
	FTransform tProjected(tCenter.Rotator().Add(0, DesiredYawVelocity * GaitTime, 0), tCenter.GetTranslation() + vel);
	FTransform tHalfway(tCenter.Rotator().Add(0, DesiredYawVelocity * GaitTime * 0.5f, 0), tCenter.GetTranslation() + vel * 0.5f);
	const FVector& upDirectionEwrtBody = tCenter.TransformVector(FVector::UpVector);
	const FVector sideDirectionEwrtBody = forPhase == ProceduralGaitPhase::Right ? -1.0f * FVector::UnitX() : FVector::UnitX();
	FTransform nextFootPose(tProjected.Rotator(),
		tProjected.TransformPosition(sideDirectionEwrtBody * StanceWidth - upDirectionEwrtBody * NominalStanceHeight));
	FTransform swingTarget(tHalfway.Rotator(),
		tHalfway.TransformPosition(sideDirectionEwrtBody * StanceWidth - upDirectionEwrtBody * NominalStanceHeight + upDirectionEwrtBody * SwingLiftHeight));

	auto updateState = [&](FFootState& state)
	{
		state.CurrentSwingTarget = swingTarget;
		state.PrevTargetEwrtWorld = state.NextTargetEwrtWorld;
		state.NextTargetEwrtWorld = nextFootPose;
	};
	if (forPhase == ProceduralGaitPhase::Left)
	{
		updateState(LeftFootState);
	}
	else
	{
		updateState(RightFootState);
	}
}

void UProceduralWalkAnimation::Stop()
{
	IsStopped = true;
	GaitPhaseOnStop = this->GaitPhase;
}

