// Copyright Artem Volovyi. All Rights Reserved.


#include "MGFirstPersonCharacter.h"

#include "Camera/CameraComponent.h"

// Sets default values
AMGFirstPersonCharacter::AMGFirstPersonCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetRootComponent());
    FirstPersonCamera->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));
    FirstPersonCamera->bUsePawnControlRotation = true;

	FirstPersonArms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonArms"));
	FirstPersonArms->AttachToComponent(FirstPersonCamera, FAttachmentTransformRules::KeepRelativeTransform);
	FirstPersonArms->SetOwnerNoSee(true);
	FirstPersonArms->SetOnlyOwnerSee(true);

	FirstPersonBodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonBodyMesh"));
	FirstPersonBodyMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	FirstPersonBodyMesh->SetOwnerNoSee(true);
	FirstPersonBodyMesh->SetOnlyOwnerSee(true);
	FirstPersonBodyMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
}

void AMGFirstPersonCharacter::RecalculateBaseEyeHeight()
{
	Super::RecalculateBaseEyeHeight();

	FirstPersonCamera->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));
}

void AMGFirstPersonCharacter::SetMeshOwnerVisibility(bool bVisible)
{
	GetMesh()->SetOwnerNoSee(!bVisible);
}

void AMGFirstPersonCharacter::SetLowerBodyOwnerVisibility(bool bVisible)
{
	FirstPersonBodyMesh->SetOwnerNoSee(!bVisible);
}

void AMGFirstPersonCharacter::SetArmsOwnerVisibility(bool bVisible)
{
	FirstPersonArms->SetOwnerNoSee(!bVisible);
}
