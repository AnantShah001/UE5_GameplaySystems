// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerBoxZone.h"

//Construction
UTriggerBoxZone::UTriggerBoxZone()
{
	PrimaryComponentTick.bCanEverTick = true;
	UE_LOG(LogTemp, Warning, TEXT("UTriggerBoxZone : Construction"));
}

void UTriggerBoxZone::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("UTriggerBoxZone : BeginPlay"));
}

void UTriggerBoxZone::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	UE_LOG(LogTemp, Warning, TEXT("UTriggerBoxZone : Tick"));
}


