#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "SpaceEggNPCAIController.generated.h"

class ASpaceEggNPC;
class UBehaviorTreeComponent;
class UBlackboardComponent;

UCLASS()
class SPACEEGG_API ASpaceEggNPCAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASpaceEggNPCAIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		ASpaceEggNPC* npc = nullptr;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(class APawn* InPawn) override;
	virtual void OnUnPossess() override;
	UFUNCTION(BlueprintCallable)
		ASpaceEggNPC* GetNPC() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BT")
		UBehaviorTree* BehaviorTreeAsset = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BT")
		UBehaviorTreeComponent* BehaviorTreeComponent = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BT")
		UBlackboardComponent* BlackboardComponent = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BT")
		UBlackboardData* BlackboardAsset = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BT")
		FName PlayerKey = "Player";
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BT")
		FName NPCKey = "NPC";
};
