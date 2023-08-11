// Copyright Artem Volovyi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "MGFirstPersonCharacter.h"
#include "MGInputConfig.h"
#include "MGAbilitySystemComponent.h"
#include "MGEquipmentManagerComponent.h"
#include "MGHealthComponent.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "MGCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMGCharacter_PlayerStateReplicated);

/**
 * AMGCharacter
 *
 *	The base character pawn class used by this project
 */
UCLASS()
class AMGCharacter : public AMGFirstPersonCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	
	AMGCharacter();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	//~IAbilitySystemInterface interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~End of IAbilitySystemInterface interface

	UAbilitySystemComponent* GetMGAbilitySystemComponent() const;

	//~AActor interface
	virtual void Reset() override;
	//~End of AActor interface

	//~APawn interface
	virtual void PossessedBy(AController* NewController) override;
	//~End of APawn interface

	void InitPlayer();

	virtual void OnRep_PlayerState() override;

	void InputAbilityInputTagPressed(FGameplayTag InputTag);
	void InputAbilityInputTagReleased(FGameplayTag InputTag);

	void InputMove(const FInputActionValue& Value);
	void InputLook(const FInputActionValue& Value);

	// Begins the death sequence for the character (disables collision, disables movement, etc...)
	UFUNCTION()
	virtual void OnDeathStarted(AActor* OwningActor);

	// Ends the death sequence for the character (detaches controller, destroys pawn, etc...)
	UFUNCTION()
	virtual void OnDeathFinished(AActor* OwningActor);

	// Called when the death sequence for the character has completed
	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "OnDeathFinished"))
	void K2_OnDeathFinished();

protected:

	void DisableMovementAndCollision();
	void UninitializeAndDestroy();
	void UninitializeAbilitySystem();

public:

	UPROPERTY(BlueprintAssignable)
	FMGCharacter_PlayerStateReplicated OnPlayerStateReplicated;

protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMGAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMGEquipmentManagerComponent> EquipmentManagerComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMGHealthComponent> HealthComponent;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UMGInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputMappingContext> InputMapping;
};
