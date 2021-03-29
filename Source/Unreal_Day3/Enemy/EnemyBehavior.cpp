// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBehavior.h"

// Sets default values for this component's properties
UEnemyBehavior::UEnemyBehavior()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyBehavior::BeginPlay()
{
	Super::BeginPlay();

	enemyCharacter = Cast<ACharacter>(GetOwner());
	aiController = Cast<AEnemyAIController>(enemyCharacter->GetController());
	enemyPublic = GetOwner()->FindComponentByClass<UEnemyPublicProperties>();

	defaultMoveSpeed = enemyCharacter->GetCharacterMovement()->MaxWalkSpeed;

	meshComponent = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();

	if (meshComponent != NULL)
	{
		animInstance = meshComponent->GetAnimInstance();

		if (animInstance != NULL)
		{
			// Get CurrentSpeed variable from Animation Blueprint.
			animProperty_CurrentSpeed = FindFProperty<FFloatProperty>(animInstance->GetClass(), "CurrentSpeed");
		}
	}
}


// Called every frame
void UEnemyBehavior::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Updating_AnimationSpeed();

	if (enemyPublic->isMoving == false && enemyPublic->onChasing == false)
	{
		Movement_Patrol();
	}

	if (enemyPublic->onChasing == true && enemyPublic->chasingTarget != NULL)
	{
		Update_ChasingBehavior();
	}
}

void UEnemyBehavior::Updating_AnimationSpeed()
{
	if (animProperty_CurrentSpeed != NULL)
	{
		FVector groundVelocity = enemyCharacter->GetCharacterMovement()->Velocity;
		groundVelocity.Z = 0.0f;
		float velocityLenght = groundVelocity.Size();
		float speedPercent = (velocityLenght / defaultMoveSpeed) * 100.0f;
		animProperty_CurrentSpeed->SetPropertyValue_InContainer(animInstance, speedPercent);
	}
}

void UEnemyBehavior::Movement_Patrol()
{
	if (aiController != NULL)
	{
		UNavigationSystemV1* navSystem = UNavigationSystemV1::GetNavigationSystem(aiController);
		FNavLocation moveToLocation;
		float moveRadius = 1000;

		FVector currentLocation = GetOwner()->GetActorLocation();

		if (navSystem != NULL)
		{
			if (navSystem->GetRandomReachablePointInRadius(currentLocation, moveRadius, moveToLocation) == true)
			{
				// MoveToLocation( TargetLocation, AcceptanceRadius, StopOnOverlap )
				// AcceptanceRadius -> If it is close to the target less than this value, it will stop.
				enemyPublic->isMoving = true;

				FTimerDelegate timerDelegate = FTimerDelegate::CreateUObject(this, &UEnemyBehavior::Set_AIDestination, moveToLocation.Location);
				FTimerHandle timer;
				float delayBeforeMove = FMath::RandRange(1.0f, 3.0f);
				GetWorld()->GetTimerManager().SetTimer(timer, timerDelegate, delayBeforeMove, false);
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, "Navigation System is NULL.");
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, "AI Controller is NULL.");
	}
}

void UEnemyBehavior::Set_AIDestination(FVector destinationLocation)
{
	if (enemyPublic->isMoving == true)
	{
		enemyCharacter->GetCharacterMovement()->MaxWalkSpeed = defaultMoveSpeed / 2.0f;
	}
	else
	{
		enemyCharacter->GetCharacterMovement()->MaxWalkSpeed = defaultMoveSpeed;
	}

	aiController->MoveToLocation(destinationLocation, 5.0f, false);
}

void UEnemyBehavior::Update_ChasingBehavior()
{
	if (enemyPublic->isMoving == true)
	{
		enemyPublic->isMoving = false;
		aiController->StopMovement();
	}

	FVector currentLocation = GetOwner()->GetActorLocation();
	FVector targetLocation = enemyPublic->chasingTarget->GetActorLocation();
	float distanceToTarget = FVector::Distance(currentLocation, targetLocation);

	if(distanceToTarget > chasingStopDistance)
	{ 
		chasingTimer += GetWorld()->GetDeltaSeconds();    // Time.deltaTime in Unreal.
	}

	if (chasingTimer > chasingDuration && distanceToTarget > chasingStopDistance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, "Stop Chasing Player");
		chasingTimer = 0.0f;
		enemyPublic->onChasing = false;
		enemyPublic->chasingTarget = NULL;
		aiController->StopMovement();
	}
	else
	{
		Set_AIDestination(targetLocation);
	}
}