// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"
#include "PlayerCharacterProperties.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyPlayerController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_DAY3_API UMyPlayerController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyPlayerController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;  // Like Start() in Unity.

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// Like Update in Unity.

	// We have .h file because ...
	// Normally C Language run from Top to Bottom.
	// That means we cannot call any functions that we write below.
	// We use .h to Predefine or Precreate everything.
	// So we can bypass that to call any function that write below.

	// Create a Pointer of ACharacter name playerCharacter.
	ACharacter* playerCharacter;
	APlayerCharacterProperties* playerCharacterComp;

	USkeletalMeshComponent* meshComponent;
	UAnimInstance* animInstance;

	FFloatProperty* animProperty_CurrentSpeed;
	FBoolProperty* animProperty_OnJump;
	FBoolProperty* animProperty_IsFalling;

	USceneComponent* attackZoneReference;
	UBoxComponent* attackZoneCollision;

	bool isInTheAir = false;
	bool onAttacking = false;

	// Predefine a function.
	void Setup_InputComponent();
	void Pressed_MoveForwardBackward(float axisValue);
	void Pressed_MoveLeftRight(float axisValue);
	void MouseMove_LookAround(float axisValue);
	void MouseMove_LookUpDown(float axisValue);
	void Pressed_Jump();

	void Updating_PlayerAnimationSpeed();

	void Pressed_Attack();
	void Released_Attack();
	void Updating_AnimationNotify();
	void End_Attacking();

	UFUNCTION()
		void OnAttackCollisionBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor
		, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnAttackCollisionEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor
			, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
