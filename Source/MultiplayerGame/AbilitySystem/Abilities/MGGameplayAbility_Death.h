// Copyright Artem Volovyi. All Rights Reserved.

#pragma once

#include "MGGameplayAbility.h"

#include "MGGameplayAbility_Death.generated.h"

/**
 * UMGGameplayAbility_Death
 *
 *	Gameplay ability used for handling death
 */
UCLASS(Abstract)
class UMGGameplayAbility_Death : public UMGGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void StartDeath();

	UFUNCTION(BlueprintCallable)
	void FinishDeath();

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
