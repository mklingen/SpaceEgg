// Fill out your copyright notice in the Description page of Project Settings.


#include "BinaryPuzzle.h"

UBinaryPuzzle::UBinaryPuzzle() :
	UActorComponent()
{
	this->PrimaryComponentTick.bCanEverTick = false;
	state1.SetNum(Size);
	state2.SetNum(Size);
}

void UBinaryPuzzle::BeginPlay()
{
	UActorComponent::BeginPlay();
}

bool UBinaryPuzzle::IsPuzzleWon_Implementation() {
	for (int i = 0; i < Size; i++) {
		if (!CheckSolution(i)) {
			return false;
		}
	}
	return true;
}

void UBinaryPuzzle::WinPuzzle_Implementation() {
	//TODO
}

void UBinaryPuzzle::LosePuzzle_Implementation() {
	//TODO
}

void UBinaryPuzzle::ResetPuzzle_Implementation() {
	//TODO
}

bool UBinaryPuzzle::Set(int col, int idx, bool value) {
	if (!IsIndexValid(idx) || col < 0 || col > 1) {
		return false;
	}
	if (col == 0) {
		this->state1[idx] = value;
	}
	else {
		this->state2[idx] = value;
	}
	return true;
}

bool UBinaryPuzzle::CheckSolution(int idx) const {
	if (!IsIndexValid(idx)) {
		return false;
	}
	const bool& a = this->state1[idx];
	const bool& b = this->state2[idx];
	switch (op) {
	case And: {
		return (a && b);
	}
	case Or: {
		return (a || b);
	}
	case NotEqual: {
		return (a != b);
	}
	case Equal: {
		return (a == b);
	}
	case Neither: {
		return (!a && !b);
	}
	}
	return false;
}

bool UBinaryPuzzle::GetState(int column, int idx) const {
	if (column == 0) 
	{
		return GetState1(idx);
	}
	else if (column == 1) 
	{
		return GetState2(idx);
	}
	return false;
}

bool UBinaryPuzzle::GetState1(int idx) const
{
	if (!IsIndexValid(idx))
	{
		return false;
	}
	return state1[idx];
}

bool UBinaryPuzzle::GetState2(int idx) const
{
	if (!IsIndexValid(idx))
	{
		return false;
	}
	return state2[idx];
}