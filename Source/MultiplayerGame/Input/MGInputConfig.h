// Copyright Artem Volovyi. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "MGInputConfig.generated.h"

class UInputAction;

/**
 * FMGInputAction
 *
 *	Struct used to map a input action to a gameplay input tag.
 */
USTRUCT(BlueprintType)
struct FMGInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag InputTag;
};

/**
 * UMGInputConfig
 *
 *	Data asset that contains input configuration properties.
 */
UCLASS(Blueprintable, Const)
class UMGInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag) const;

	UFUNCTION(BlueprintCallable)
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag) const;

public:
	// List of input actions used by the owner
	// These input actions are mapped to a gameplay tag and must be manually bound
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions", Meta = (TitleProperty = "InputAction"))
	TArray<FMGInputAction> NativeInputActions;

	// List of input actions used by the owner
	// These input actions are mapped to a gameplay tag and are automatically bound to abilities with matching input tags
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
	TArray<FMGInputAction> AbilityInputActions;
};
