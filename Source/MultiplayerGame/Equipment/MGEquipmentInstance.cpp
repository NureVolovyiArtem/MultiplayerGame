// Copyright Artem Volovyi. All Rights Reserved.


#include "MGEquipmentInstance.h"

#include "MGAbilitySet.h"
#include "MGFirstPersonCharacter.h"
#include "MGEquipmentDefinition.h"
#include "Net/UnrealNetwork.h"

void UMGEquipmentInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Instigator);
	DOREPLIFETIME(ThisClass, SpawnedActorsThirdPerson);
	DOREPLIFETIME(ThisClass, SpawnedActorsFirstPerson);
}

UWorld* UMGEquipmentInstance::GetWorld() const
{
	if (const APawn* OwningPawn = GetPawn())
	{
		return OwningPawn->GetWorld();
	}

	return nullptr;
}

void UMGEquipmentInstance::Tick(float DeltaSeconds)
{
}

APawn* UMGEquipmentInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

void UMGEquipmentInstance::SpawnEquipmentActors(const TArray<FMGEquipmentActorToSpawn>& ActorsToSpawn)
{
	if (APawn* OwningPawn = GetPawn())
	{
		if (const AMGFirstPersonCharacter* Character = Cast<AMGFirstPersonCharacter>(OwningPawn))
		{
			for (const FMGEquipmentActorToSpawn& SpawnInfo : ActorsToSpawn)
			{
				USceneComponent* AttachTarget = SpawnInfo.bFirstPersonView ? Character->GetFirstPersonArms() : Character->GetMesh();
				
				AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnInfo.ActorToSpawn, FTransform::Identity, OwningPawn);
				NewActor->FinishSpawning(FTransform::Identity, /*bIsDefaultTransform=*/ true);
				NewActor->SetActorRelativeTransform(SpawnInfo.AttachTransform);
				NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, SpawnInfo.AttachSocket);

				TArray<TObjectPtr<AActor>>& SpawnedActors = SpawnInfo.bFirstPersonView ? SpawnedActorsFirstPerson : SpawnedActorsThirdPerson;
				SpawnedActors.Add(NewActor);
			}
		}
	}
}

void UMGEquipmentInstance::SetEquipmentActorsVisibility(bool bIsVisible)
{
	for (AActor* Actor : SpawnedActorsThirdPerson)
	{
		if (Actor)
		{
			Actor->SetActorHiddenInGame(!bIsVisible);
		}
	}

	for (AActor* Actor : SpawnedActorsFirstPerson)
	{
		if (Actor)
		{
			Actor->SetActorHiddenInGame(!bIsVisible);
		}
	}
}

void UMGEquipmentInstance::DestroyEquipmentActors()
{
	for (AActor* Actor : SpawnedActorsThirdPerson)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}

	SpawnedActorsThirdPerson.Empty();

	for (AActor* Actor : SpawnedActorsFirstPerson)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}

	SpawnedActorsFirstPerson.Empty();
}

void UMGEquipmentInstance::OnActivated(FMGAbilitySet_GrantedHandles& GrantedHandles)
{
	EquipmentGrantedHandles = GrantedHandles;
	SetEquipmentActorsVisibility(true);
	K2_OnActivated();
}

void UMGEquipmentInstance::OnDeactivated()
{
	SetEquipmentActorsVisibility(false);
	K2_OnDeactivated();
}

void UMGEquipmentInstance::OnEquipped()
{
	// First person arms are hidden before equipping an item, so we start showing them
	if (AMGFirstPersonCharacter* Character = Cast<AMGFirstPersonCharacter>(GetPawn()))
	{
		Character->SetArmsOwnerVisibility(true);
	}

	OnRep_SpawnedActorsFirstPerson();
	OnRep_SpawnedActorsThirdPerson();

	K2_OnEquipped();
}

void UMGEquipmentInstance::OnUnequipped()
{
	K2_OnUnequipped();
}

void UMGEquipmentInstance::OnRep_Instigator()
{
}

void UMGEquipmentInstance::OnRep_SpawnedActorsThirdPerson()
{
	for (const AActor* Actor : SpawnedActorsThirdPerson)
	{
		if (Actor)
		{
			USkeletalMeshComponent* ActorMesh = Cast<USkeletalMeshComponent>(Actor->GetRootComponent());
			ActorMesh->SetOwnerNoSee(true);
		}
	}
}

void UMGEquipmentInstance::OnRep_SpawnedActorsFirstPerson()
{
	for (const AActor* Actor : SpawnedActorsFirstPerson)
	{
		if (Actor)
		{
			USkeletalMeshComponent* ActorMesh = Cast<USkeletalMeshComponent>(Actor->GetRootComponent());
			ActorMesh->SetOnlyOwnerSee(true);
		}
	}
}
