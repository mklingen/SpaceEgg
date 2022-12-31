#include "SpaceEggNPCAIController.h"
#include "SpaceEggNPC.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SpaceEgg/Helpers/PrintHelper.h"
#include "SpaceEgg/Player/SpaceEggCharacter.h"

// Sets default values
ASpaceEggNPCAIController::ASpaceEggNPCAIController()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior tree"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
}

// Called when the game starts or when spawned
void ASpaceEggNPCAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ASpaceEggNPCAIController::OnPossess(class APawn* InPawn)
{
	Super::OnPossess(InPawn);

	npc = Cast<ASpaceEggNPC>(InPawn);
	if (!npc)
	{
		LOGE("Not posessing a SpaceEggNPC.");
	}
	else
	{
		if (BlackboardComponent && BlackboardAsset)
		{
			BlackboardComponent->InitializeBlackboard(*BlackboardAsset);
		}
		if (BehaviorTreeComponent && BehaviorTreeAsset)
		{
			BehaviorTreeComponent->StartTree(*BehaviorTreeAsset);
		}
	}
}

void ASpaceEggNPCAIController::OnUnPossess()
{
	Super::OnUnPossess();

	if (BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree();
	}
}

// Called every frame
void ASpaceEggNPCAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (npc && BehaviorTreeComponent && BlackboardComponent)
	{

		BlackboardComponent->SetValueAsObject(NPCKey, npc);

		if (npc->GetLastSeenCharacter())
		{
			BlackboardComponent->SetValueAsObject(PlayerKey, npc->GetLastSeenCharacter());
		}
	}
}

ASpaceEggNPC* ASpaceEggNPCAIController::GetNPC() const
{
	return npc;
}