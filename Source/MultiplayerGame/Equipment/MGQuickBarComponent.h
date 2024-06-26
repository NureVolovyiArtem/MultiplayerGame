// Copyright Artem Volovyi. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "MGQuickBarComponent.generated.h"

class UMGEquipmentInstance;
class UMGEquipmentDefinition;
class UMGEquipmentManagerComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMGQuickBar_SlotsChanged, const TArray<UMGEquipmentInstance*>&, Slots);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMGQuickBar_ActiveSlotIndexChanged, int32, ActiveSlotIndex);

UENUM(BlueprintType)
enum EMGQuickSlots
{
	QuickSlot1,
	QuickSlot2,
	QuickSlot3,
	QuickSlot4,
	QuickSlot5,
	QuickSlot6
};

/**
 * FMGQuickSlotEquipmentDefinition
 *
 *	Associates quick slots with equipment definitions
 */
USTRUCT()
struct FMGQuickSlotEquipmentDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EMGQuickSlots> QuickSlot;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMGEquipmentDefinition> EquipmentDefinition;
};

/**
 * UMGQuickBarComponent
 *
 *	Component that adds equipment quick slots to the pawn
 */
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UMGQuickBarComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UMGQuickBarComponent();

	//~UActorComponent interface
	virtual void BeginPlay() override;
	//~End of UActorComponent interface

	UFUNCTION(BlueprintCallable)
	void CycleActiveSlotForward();

	UFUNCTION(BlueprintCallable)
	void CycleActiveSlotBackward();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SetActiveSlotIndex(int32 NewIndex);

	UFUNCTION(BlueprintCallable)
	TArray<UMGEquipmentInstance*> GetSlots() const { return Slots; }

	UFUNCTION(BlueprintCallable)
	int32 GetActiveSlotIndex() const { return ActiveSlotIndex; }

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UMGEquipmentInstance* AddItemToSlot(TSubclassOf<UMGEquipmentDefinition> EquipmentClass);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void RemoveItemFromSlot(int32 SlotIndex);

protected:

	UFUNCTION()
	void OnRep_Slots();

	UFUNCTION()
	void OnRep_ActiveSlotIndex();

private:

	UMGEquipmentManagerComponent* GetEquipmentManager() const;

	// Finds quick slot designated for this equipment definition
	int32 FindQuickSlotByEquipmentDefinition(TSubclassOf<UMGEquipmentDefinition> EquipmentDefinition);

	void EquipItemInActiveSlot();
	void UnequipItemInActiveSlot();

public:

	UPROPERTY(EditDefaultsOnly)
	TArray<FMGQuickSlotEquipmentDefinition> QuickSlotEquipmentDefinitions;

public:

	// Delegate fired when slots are changed
	UPROPERTY(BlueprintAssignable)
	FMGQuickBar_SlotsChanged OnSlotsChanged;

	// Delegate fired when active slot index is changed
	UPROPERTY(BlueprintAssignable)
	FMGQuickBar_ActiveSlotIndexChanged OnActiveSlotIndexChanged;

private:
	
	UPROPERTY(ReplicatedUsing=OnRep_Slots)
	TArray<TObjectPtr<UMGEquipmentInstance>> Slots;

	UPROPERTY(ReplicatedUsing=OnRep_ActiveSlotIndex)
	int32 ActiveSlotIndex = -1;
};
