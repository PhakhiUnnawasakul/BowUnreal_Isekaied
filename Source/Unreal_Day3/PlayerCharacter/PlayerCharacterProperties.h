// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacterProperties.generated.h"

UCLASS()
class UNREAL_DAY3_API APlayerCharacterProperties : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacterProperties();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	UPROPERTY(EditAnywhere, Category = "Animation Montage")
		UAnimMontage* attackMontage_01;

	int playerScore = 0;
};
