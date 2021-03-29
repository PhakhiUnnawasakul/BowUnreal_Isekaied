// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "TimerManager.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"

#include "EnemyPublicProperties.h"

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_DAY3_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public :

	// Create Class Constructor
	AEnemyAIController();

	UEnemyPublicProperties* enemyPublic;
	UAIPerceptionComponent* aiPerception;

	FString sightStimulusName = "Default__AISense_Sight";       // Use double Underscroll !!!!
	FString hearingStimulusName = "Default__AISense_Hearing";   // Use double Underscroll !!!!

	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	UFUNCTION()
		void OnTargetPerceptionUpdated(AActor* senseActor, FAIStimulus stimulusInfo);

	virtual void OnPossess(APawn* pawn) override;
};
