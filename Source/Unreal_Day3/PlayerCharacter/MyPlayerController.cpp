// Fill out your copyright notice in the Description page of Project Settings.

// ActorComponent -> We will not set any Public Value here, for example, Montage to Play when Attack.
// Because It still has a bug that when we build this script again, it will reset.

#include "MyPlayerController.h"

// Sets default values for this component's properties
UMyPlayerController::UMyPlayerController() // Constructor -> Like Awake() but can only do Resources, Asset or Value setting. Cannot do action.
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Call to setup Input Component
	playerCharacter = Cast<ACharacter>(GetOwner());
	playerCharacterComp = Cast<APlayerCharacterProperties>(GetOwner());

	Setup_InputComponent();

	meshComponent = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();

	if (meshComponent != NULL)
	{
		animInstance = meshComponent->GetAnimInstance();

		if (animInstance != NULL)
		{
			// Get CurrentSpeed variable from Animation Blueprint.
			animProperty_CurrentSpeed = FindFProperty<FFloatProperty>(animInstance->GetClass(), "CurrentSpeed");
			animProperty_OnJump = FindFProperty<FBoolProperty>(animInstance->GetClass(), "OnJump");
			animProperty_IsFalling = FindFProperty<FBoolProperty>(animInstance->GetClass(), "IsFalling");
		}
	}

	TArray<USceneComponent*> sceneComps;
	GetOwner()->GetComponents<USceneComponent>(sceneComps);

	for (int index = 0 ; index < sceneComps.Num() ; index++)
	{
		if (sceneComps[index]->GetFName().ToString() == "AttackZoneReference")
		{
			attackZoneReference = sceneComps[index];
		}
	}

	TArray<UBoxComponent*> boxComps;
	GetOwner()->GetComponents<UBoxComponent>(boxComps);

	for (int index = 0; index < boxComps.Num(); index++)
	{
		if (boxComps[index]->GetFName().ToString() == "AttackZoneCollision")
		{
			attackZoneCollision = boxComps[index];
			attackZoneCollision->SetCollisionProfileName("NoCollision");
			attackZoneCollision->OnComponentBeginOverlap.AddDynamic(this, &UMyPlayerController::OnAttackCollisionBeginOverlap);
			attackZoneCollision->OnComponentEndOverlap.AddDynamic(this, &UMyPlayerController::OnAttackCollisionEndOverlap);
		}
	}
}

void UMyPlayerController::Setup_InputComponent()
{
	// GetOwner() -> Like gameObject in Unity -> Get an object that this script attach to.
	// BindAxis( "KeyName that we set in Input", this, FunctionName that we want to use when the event is trigger )
	GetOwner()->InputComponent->BindAxis("MoveForwardBackward", this, &UMyPlayerController::Pressed_MoveForwardBackward);
	GetOwner()->InputComponent->BindAxis("MoveLeftRight", this, &UMyPlayerController::Pressed_MoveLeftRight);
	GetOwner()->InputComponent->BindAxis("LookAround", this, &UMyPlayerController::MouseMove_LookAround);
	GetOwner()->InputComponent->BindAxis("LookUpDown", this, &UMyPlayerController::MouseMove_LookUpDown);

	GetOwner()->InputComponent->BindAction("DoJump", IE_Pressed, this, &UMyPlayerController::Pressed_Jump);

	GetOwner()->InputComponent->BindAction("DoAttack", IE_Pressed, this, &UMyPlayerController::Pressed_Attack);
	GetOwner()->InputComponent->BindAction("DoAttack", IE_Released, this, &UMyPlayerController::Released_Attack);
}

void UMyPlayerController::Pressed_MoveForwardBackward(float axisValue)   // <---- Don't forget float axisValue
{
	if (onAttacking == false)
	{
		// Get current Rotation of the character.
		FRotator currentRotation = playerCharacter->GetControlRotation();
		currentRotation.Pitch = 0;   // Remove look up and look down value.

		// Get Forward Direction from Calculate Matrix of currentRotation.
		// And use EAxis::X for Forward. (Around 90% of object will use X. Sometime Forward is not X base on a Character.)
		FVector forwardDirection = FRotationMatrix(currentRotation).GetScaledAxis(EAxis::X);

		// AddMovementInput(FVector Direction, float power);
		playerCharacter->AddMovementInput(forwardDirection, axisValue);
	}
}

void UMyPlayerController::Pressed_MoveLeftRight(float axisValue)
{
	if (onAttacking == false)
	{
		// Get current Rotation of the character.
		FRotator currentRotation = playerCharacter->GetControlRotation();
		currentRotation.Pitch = 0;   // Remove look up and look down value.

		FVector rightDirection = FRotationMatrix(currentRotation).GetScaledAxis(EAxis::Y);

		// AddMovementInput(FVector Direction, float power);
		playerCharacter->AddMovementInput(rightDirection, axisValue);
	}
}

void UMyPlayerController::MouseMove_LookAround(float axisValue)
{
	playerCharacter->AddControllerYawInput(axisValue);
}

void UMyPlayerController::MouseMove_LookUpDown(float axisValue)
{
	playerCharacter->AddControllerPitchInput(axisValue);
}

void UMyPlayerController::Pressed_Jump()
{
	playerCharacter->bPressedJump = true;
	animProperty_OnJump->SetPropertyValue_InContainer(animInstance, true);
}

// Called every frame
void UMyPlayerController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Updating_PlayerAnimationSpeed();
	Updating_AnimationNotify();
}

void UMyPlayerController::Updating_PlayerAnimationSpeed()
{
	if (animProperty_CurrentSpeed != NULL)
	{
		FVector groundVelocity = playerCharacter->GetCharacterMovement()->Velocity;
		groundVelocity.Z = 0.0f;
		float velocityLenght = groundVelocity.Size();
		animProperty_CurrentSpeed->SetPropertyValue_InContainer(animInstance, velocityLenght);
	}

	if (animProperty_OnJump != NULL && animProperty_IsFalling != NULL)
	{
		bool fallingState = playerCharacter->GetCharacterMovement()->IsFalling();
		animProperty_IsFalling->SetPropertyValue_InContainer(animInstance, fallingState);

		if (fallingState == true)
		{
			isInTheAir = true;
		}
		else
		{
			if (isInTheAir == true)
			{
				animProperty_OnJump->SetPropertyValue_InContainer(animInstance, false);
				isInTheAir = false;
			}
		}
	}
}

void UMyPlayerController::Pressed_Attack()
{
	if (animInstance != NULL)
	{
		if (onAttacking == false)
		{
			playerCharacter->SetAnimRootMotionTranslationScale(2.0f); // Global Set -> Set it before play.
			animInstance->Montage_Play(playerCharacterComp->attackMontage_01);
		}
	}
}

void UMyPlayerController::Released_Attack()
{

}

void UMyPlayerController::Updating_AnimationNotify()
{
	for (int index = 0 ; index < animInstance->NotifyQueue.AnimNotifies.Num() ; index++)
	{
		FString notifyName = animInstance->NotifyQueue.AnimNotifies[index].GetNotify()->NotifyName.ToString();

		if (notifyName == "Attack_Prepare")
		{
			onAttacking = true;
		}

		if (notifyName == "Attack_End")
		{
			End_Attacking();
		}

		if (notifyName == "Open_AttackCollision")
		{
			playerCharacter->MakeNoise();  // Make noise to let AI hear it.
			attackZoneCollision->SetCollisionProfileName("OverlapAll");
		}

		if (notifyName == "Close_AttackCollision")
		{
			attackZoneCollision->SetCollisionProfileName("NoCollision");
		}
	}
}

void UMyPlayerController::End_Attacking()
{
	onAttacking = false;
	attackZoneCollision->SetCollisionProfileName("NoCollision");
}

void UMyPlayerController::OnAttackCollisionBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor
	, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != GetOwner())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, "Attack hit : " + OtherActor->GetName());
		playerCharacterComp->playerScore += 1;
	}
}

void UMyPlayerController::OnAttackCollisionEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor
	, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

