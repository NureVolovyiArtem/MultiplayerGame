// Copyright Artem Volovyi. All Rights Reserved.

#pragma once

#include "GameFramework/GameMode.h"

#include "MGGameMode.generated.h"

struct FGameplayEffectContextHandle;

namespace MatchState
{
	MULTIPLAYERGAME_API extern const FName Warmup;			// Restricted gameplay (short time, no win condition etc.)
	MULTIPLAYERGAME_API extern const FName MatchStarting;	// Match is about to start
}

/**
 * AMGGameMode
 *
 *	The base game mode class used by this project
 */
UCLASS()
class AMGGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	//~AGameModeBase interface
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void RestartPlayer(AController* NewPlayer) override;
	//~End of AGameModeBase interface

	//~AGameMode interface
	virtual void Tick(float DeltaSeconds) override;
	virtual bool HasMatchStarted() const override;
	virtual void OnMatchStateSet() override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual bool PlayerCanRestart_Implementation(APlayerController* Player) override;
	//~End of AGameModeBase interface

	UFUNCTION(BlueprintCallable)
	virtual bool IsWarmup() const;

	UFUNCTION(BlueprintCallable)
	virtual bool ReadyToStartWarmup() const;

	UFUNCTION(BlueprintCallable)
	virtual void StartWarmup();

	UFUNCTION(BlueprintCallable)
	virtual void SwitchToNextMatchState(); 

	UFUNCTION(BlueprintCallable)
	void RequestPlayerRestartNextFrame(AController* Controller, bool bForceReset);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnPlayerKilled"))
	void K2_OnPlayerKilled(AActor* KillerActor, AActor* KilledActor, const FGameplayEffectContextHandle& DamageContext);

protected:
	
	//~AGameMode interface
	virtual void HandleMatchHasStarted() override;
	//~End of AGameModeBase interface

	virtual void HandleWarmupStarted();
	virtual void HandleMatchStarting();

	UFUNCTION()
	virtual void OnPlayerKilled(AActor* KillerActor, AActor* KilledActor, const FGameplayEffectContextHandle& DamageContext);

	int32 GetNextClientId();

protected:

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
    int32 MatchDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
    int32 WarmupDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
    int32 MatchStartingDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
    int32 MinPlayers;

private:

	int32 ClientId = 0;
};
