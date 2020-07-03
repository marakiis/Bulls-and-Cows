// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    // LOAD WORD LIST
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("Collections/HiddenWords.txt");
    FFileHelper::LoadFileToStringArray(this->Isograms, *WordListPath);
    /* Requires UE4.25
    FFileHelper::LoadFileToStringArrayWithPredicate(this->Isograms, *WordListPath, [](const FString& Word)
        {
            IsIsogram(Word);
        });
    */
    // SETUP
    this->SetupGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (this->bGameOver) {
        ClearScreen();
        this->SetupGame();
        return;
    }
    
    ClearScreen();

    if (!this->IsGuessValid(Input)) {
        return;
    }

    if (Input == this->HiddenWord) {
        // WIN
        PrintLine(TEXT("You guessed right"));
        this->EndGame(true);
        return;
    } 

    PrintLine(TEXT("Sorry it isn't the correct Hidden Word"));
    this->Tries--;
    
    if (this->Tries < 1) {
        // LOOSE
        this->EndGame(false);
        return;
    }
    this->BullsAndCows(Input);
    this->PrintInfo();
}

bool UBullCowCartridge::IsGuessValid(const FString& Guess) const {
    if (Guess == "") {
        PrintLine(TEXT("The Hidden Word is an Isogram"));
        return false;
    }
    // CHECK IF ISOGRAM
    if (!IsIsogram(Guess)) {
        PrintLine(TEXT("The Hidden Word is an Isogram"));
        return false;
    }
    // CHECK IF CORRECT NUMBER OF LETTERS
    if (Guess.Len() != this->HiddenWord.Len()) {
        PrintLine(FString::Printf(TEXT("The Hidden Word is %i letters long.\n"), this->HiddenWord.Len()));
        return false;
    }
    return true;
}

bool UBullCowCartridge::IsIsogram(const FString& Word) {
    for (int32 i = 0; i < Word.Len() -1; i++) {
        for (int32 j = i + 1; j < Word.Len(); j++) {
            if (Word[i] == Word[j]) {
                return false;
            }
        }
    }
    return true;
}

void UBullCowCartridge::BullsAndCows(const FString& Guess) const {
    if (Guess.Len() != this->HiddenWord.Len()) { // SHOULD NOT HAPPEN
        PrintLine(FString::Printf(TEXT("The Hidden Word is %i letters long.\n"), this->HiddenWord.Len()));
        return;
    }

    int32 Bulls = 0;
    int32 Cows = 0;

    for (int32 i = 0; i < Guess.Len(); i++) {
        for (int32 j = 0; j < Guess.Len(); j++) {
            if (Guess[i] == this->HiddenWord[j]) {
                if (i == j) {
                    Bulls++;
                } else {
                    Cows++;
                }
                break;
            }
        }
    }
    PrintLine(FString::Printf(TEXT("You have %i Bulls and %i Cows"), Bulls, Cows));
}

void UBullCowCartridge::SetupGame() {
    // PRINT WELCOME MESSAGE
    PrintLine(TEXT("Welcome to Bulls & Cows !"));
    PrintLine(TEXT("The goal is to guess the Hidden Word!"));
    
    // SETUP VARIABLES
    this->bGameOver = false;
    this->HiddenWord = this->PickWord();
    this->Tries = this->HiddenWord.Len() * 2;
    
     // PRINT GUESSES INFO
    PrintLine(FString::Printf(TEXT("The Hidden Word is %i letters long.\n"), 
        this->HiddenWord.Len()));
    this->PrintInfo();
    // DEBUG
    PrintLine(FString::Printf(TEXT("The Hidden Word is: %s"), *HiddenWord));
}

FString UBullCowCartridge::PickWord() const { 
    FString out;
    bool valid = false;
    do {
        int32 picked = FMath::RandRange(0, this->Isograms.Num() - 1);
        out = this->Isograms[picked];
        valid = this->IsIsogram(out);
    } while (!valid);
    return out;
}

void UBullCowCartridge::PrintInfo() const {
    PrintLine(FString::Printf(TEXT("You have %i tries.\n"), this->Tries));
    PrintLine(TEXT("Please enter your guess..."));
}

void UBullCowCartridge::EndGame(bool bWin) {
    // PRINT END MESSAGE
    if (bWin) {
       PrintLine(TEXT("You win !"));
    } else {
        PrintLine(TEXT("You loose the game !"));
    }

    this->bGameOver = true;
    PrintLine(TEXT("Game Over press enter play again..."));
}
