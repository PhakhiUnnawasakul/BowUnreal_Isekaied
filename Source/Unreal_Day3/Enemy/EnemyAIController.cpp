// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

AEnemyAIController::AEnemyAIController()
{

}

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (Result.IsSuccess() == true)
	{
		enemyPublic->isMoving = false;
	}

	if (Result.IsInterrupted() == true)
	{
		enemyPublic->isMoving = false;
	}
}

void AEnemyAIController::OnTargetPerceptionUpdated(AActor* senseActor, FAIStimulus stimulusInfo)
{
	if (stimulusInfo.Type.Name.ToString() == sightStimulusName)
	{
		if (senseActor->ActorHasTag("Player") == true)
		{
			if (stimulusInfo.WasSuccessfullySensed() == true)			// See Player.
			{
				GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, "Sight Player in range.");

				enemyPublic->onChasing = true;
				enemyPublic->chasingTarget = senseActor;
			}
			else if (stimulusInfo.WasSuccessfullySensed() == false)     // Lose sight of player.
			{
				GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, "Lose sight of player.");

				enemyPublic->onChasing = false;
				enemyPublic->chasingTarget = NULL;
			}
		}
	}
	else if (stimulusInfo.Type.Name.ToString() == hearingStimulusName)
	{
		if (senseActor->ActorHasTag("Player") == true)
		{
			if (stimulusInfo.WasSuccessfullySensed() == true)			// Hear Player.
			{
				GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, "Hearing Player noise.");

				enemyPublic->onChasing = true;
				enemyPublic->chasingTarget = senseActor;
			}
		}
	}
}

void AEnemyAIController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn); // Get all commands from AIController and we will add more thing here.

	enemyPublic = GetPawn()->FindComponentByClass<UEnemyPublicProperties>();
	aiPerception = FindComponentByClass<UAIPerceptionComponent>();

	if (aiPerception != NULL)
	{
		aiPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
	}
}

