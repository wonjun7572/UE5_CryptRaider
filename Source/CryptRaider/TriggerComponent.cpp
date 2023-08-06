// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Display, TEXT("Trigger Component Alive"));
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Actor = GetAcceptableActor();

	if(Actor != nullptr)
	{
		UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());

		if(Component != nullptr)
			Component->SetSimulatePhysics(false);

		Actor->AttachToComponent(this,FAttachmentTransformRules::KeepWorldTransform);
		m_Mover->SetSholudMove(true);
	}
	else
	{
		m_Mover->SetSholudMove(false);
	}
}

AActor* UTriggerComponent::GetAcceptableActor() const
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	// int32 index = Actors.Num();
	// for(int32 i = 0; i < index; ++i)
	// {
	// 	UE_LOG(LogTemp, Display, TEXT("Overlapping : %s"), *Actors[i]->GetActorNameOrLabel());
	// }

	// 범위 기반 for 문
	for(AActor* Actor : Actors)
	{
		if(Actor->ActorHasTag(AcceptableActorTag) && !Actor->ActorHasTag("Grabbed"))
		{
			return Actor;
		}
	}

	return nullptr;
}

void UTriggerComponent::SetMover(UMover* mover)
{
	m_Mover = mover;
}