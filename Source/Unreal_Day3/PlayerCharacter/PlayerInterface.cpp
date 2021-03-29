// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInterface.h"

// Sets default values for this component's properties
UPlayerInterface::UPlayerInterface()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	playerWidget = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/Blueprints/HUD/HUD_PlayerInterface.HUD_PlayerInterface_C"));
}


// Called when the game starts
void UPlayerInterface::BeginPlay()
{
	Super::BeginPlay();

	playerCharacter = Cast<APlayerCharacterProperties>(GetOwner());

	if (playerWidget != NULL)
	{
		mainWidget = CreateWidget<UPlayerWidget>(GetWorld(), playerWidget);

		if (mainWidget != NULL)
		{
			mainWidget->AddToViewport(1);

			UWidgetTree* widgetTree = mainWidget->WidgetTree;
			
			FString widgetString = "Score_Text";
			FName widgetName = FName(*widgetString);
			UWidget* targetWidget = widgetTree->FindWidget(widgetName);

			if (targetWidget != NULL)
			{
				scoreText = Cast<UTextBlock>(targetWidget);
			}
		}
	}
}


// Called every frame
void UPlayerInterface::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString stringToShow = "Score : " + FString::FromInt(playerCharacter->playerScore);
	scoreText->SetText(FText::FromString(stringToShow));
}

