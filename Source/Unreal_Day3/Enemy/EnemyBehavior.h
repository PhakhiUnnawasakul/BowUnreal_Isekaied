// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "EnemyAIController.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemyPublicProperties.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyBehavior.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_DAY3_API UEnemyBehavior : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyBehavior();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	ACharacter* enemyCharacter;
	AEnemyAIController* aiController;
	USkeletalMeshComponent* meshComponent;
	UAnimInstance* animInstance;
	UEnemyPublicProperties* enemyPublic;

	FFloatProperty* animProperty_CurrentSpeed;

	float defaultMoveSpeed = 0.0f;

	float chasingTimer = 0.0f;
	float chasingDuration = 10.0f;
	float chasingStopDistance = 500.0f;

	void Updating_AnimationSpeed();

	void Movement_Patrol();
	void Set_AIDestination(FVector destinationLocation);

	void Update_ChasingBehavior();
};
