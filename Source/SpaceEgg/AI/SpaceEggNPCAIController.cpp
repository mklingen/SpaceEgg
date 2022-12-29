#include "SpaceEggNPCAIController.h"
#include "SpaceEggNPC.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SpaceEgg/Helpers/PrintHelper.h"
// Sets default values
ASpaceEggNPCAIController::ASpaceEggNPCAIController()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BehaviorTree = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior tree"));
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
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
		if (Blackboard && BlackboardAsset)
		{
			Blackboard->InitializeBlackboard(*BlackboardAsset);
		}
		if (BehaviorTree && BehaviorTreeAsset)
		{
			BehaviorTree->StartTree(*BehaviorTreeAsset);
		}
	}
}

void ASpaceEggNPCAIController::OnUnPossess()
{
	Super::OnUnPossess();

	if (BehaviorTree)
	{
		BehaviorTree->StopTree();
	}
}

// Called every frame
void ASpaceEggNPCAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ASpaceEggNPC* ASpaceEggNPCAIController::GetNPC() const
{
	return npc;
}