// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString &) override;

	virtual FString PickWord() const;
	virtual void PrintInfo() const;
	virtual bool IsGuessValid(const FString &) const;
	virtual void BullsAndCows(const FString &) const;

	virtual void SetupGame();
	virtual void EndGame(bool);

	static bool IsIsogram(const FString &);

	// Your declarations go below!
private:
	bool bGameOver;
	int32 Tries;
	FString HiddenWord;
	TArray<FString> Isograms;
};
