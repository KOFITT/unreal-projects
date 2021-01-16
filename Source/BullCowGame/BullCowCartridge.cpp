// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
	Super::BeginPlay();
	Isograms = GetValidWords(Words);
	SetupGame();
	FBullCowCount Count;
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
	if (bGameOver)
	{
		ClearScreen();
		SetupGame();
		return;
	}
	else
	{
		ProcessGuess(Input);
		// PrintLine(Input);
		return;
	}
}

void UBullCowCartridge::SetupGame()
{
	HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
	Lives = (HiddenWord.Len() * 2) + 5;

	PrintLine(TEXT("Welcome to the bull cow game!"));
	PrintLine(FString::Printf(TEXT("You have %i lives."), Lives));
	PrintLine(FString::Printf(TEXT("The hidden word is %i letter long."), HiddenWord.Len()));
	PrintLine(TEXT("Start the game with typing your guess..."));
	PrintLine(TEXT("\nBull number means letter is correct and \nin the correct spot."));
	PrintLine(TEXT("Cow number means letter is correct but \nin the wrong spot."));

	bGameOver = false;
}

void UBullCowCartridge::EndGame()
{
	bGameOver = true;
	ClearScreen();
	PrintLine(TEXT("Out of lives."));
	PrintLine(FString::Printf(TEXT("Hidden word was \"%s\".\n"), *HiddenWord));
	PrintLine(TEXT("Press enter to continue."));
}

bool UBullCowCartridge::ProcessGuess(FString Guess)
{
	if (Guess == HiddenWord)
	{
		PrintLine(TEXT("You pass this level. Congratulations!"));
		return bGameOver = true;
	}


	if (Guess.Len() != HiddenWord.Len())
	{
		PrintLine(TEXT("The hidden word is %i character long. \nMake another guess."), HiddenWord.Len());
		return bGameOver = false;
	}

	if (!bIsIsogram(Guess))
	{
		PrintLine("Your guess is not isogram, make sure your guess is isogram!");
		return bGameOver = false;
	}

	FBullCowCount Score = GetBullCows(Guess);
	PrintLine(TEXT("You have %i Bulls and %i Cows."), Score.Bulls, Score.Cows);

	if (Lives > 1)
	{
		PrintLine(TEXT("You lost one life!"));
		PrintLine(FString::Printf(TEXT("You have %i lives now."), --Lives));
		return bGameOver = false;
	}
	EndGame();
	return 0;
}

bool UBullCowCartridge::bIsIsogram(const FString& Words) const
{
	for (int32 Index = 0; Index < Words.Len(); Index++)
	{
		for (int32 Comparison = Index + 1; Comparison < Words.Len(); Comparison++)
		{
			if (Words[Index] == Words[Comparison])
			{
				return false;
			}
		}
	}
	return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& Words) const
{
	TArray<FString> ValidWords;

	for (FString Word : Words)
	{
		if (Word.Len() >= 4 && Word.Len() <=8 && bIsIsogram(Word))
		{
			ValidWords.Emplace(Word);
		}
	}
	return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
	FBullCowCount Count;
	for (int32 GuessIndex = 0; GuessIndex <= Guess.Len() - 1; GuessIndex++)
	{
		if (Guess[GuessIndex] == HiddenWord[GuessIndex])
		{
			Count.Bulls++;
			continue;
		}

		for (int32 HiddenIndex = 0; HiddenIndex < Guess.Len(); HiddenIndex++)
		{
			if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
			{
				Count.Cows++;
				break;
			}
		}
	}
	return Count;
}