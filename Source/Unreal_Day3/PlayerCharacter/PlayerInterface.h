// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"
#include "PlayerWidget.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Blueprint/WidgetTree.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "Runtime/UMG/Public/Components/Button.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/ProgressBar.h"
#include "Runtime/UMG/Public/Components/CanvasPanelSlot.h"

#include "PlayerCharacterProperties.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInterface.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_DAY3_API UPlayerInterface : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerInterface();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TSubclassOf<class UUserWidget> playerWidget;

	UPlayerWidget* mainWidget;
	APlayerCharacterProperties* playerCharacter;

	UTextBlock* scoreText;
};
