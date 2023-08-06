// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr)
		return;
	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle == nullptr)
		return;

	// 현재 월드를 부르는 함수 
	//UWorld* World = GetWorld();

	FHitResult HitResult;
	bool HasHit = GetGrabbableInReach(HitResult);

	if(HasHit)
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->WakeAllRigidBodies();
		PhysicsHandle->GrabComponentAtLocationWithRotation(HitComponent,
			NAME_None, 
			HitResult.ImpactPoint, 
			GetComponentRotation());

		// Draw 하는 부분
		/*DrawDebugSphere(World, HitResult.Location, 10, 10, FColor::Yellow, false, 5);
		DrawDebugSphere(World, HitResult.ImpactPoint, 10, 10, FColor::Purple, false, 5);
		UE_LOG(LogTemp, Display, TEXT("HitActor : %s"), *HitResult.GetActor()->GetActorNameOrLabel());*/
	}

	// 발견되지 않을 때
	//else
	//{
	//	DrawDebugSphere(World, End, 10, 10, FColor::Blue, false, 5);
	//	UE_LOG(LogTemp, Display, TEXT("No Hit Actor!!!"));
	//}
}

void UGrabber::Release()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle == nullptr)
		return;

	if(PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		PhysicsHandle->ReleaseComponent();
	}

	UE_LOG(LogTemp, Display, TEXT("Release"));
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PhysicsHandle Nullptr"));
		return nullptr;
	}

	return PhysicsHandle;
}

bool UGrabber::GetGrabbableInReach(FHitResult& HitResult) const
{
	const FVector Start = GetComponentLocation();
	const FVector End = Start + GetForwardVector() * MaxGrabDistance;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);

	return GetWorld()->SweepSingleByChannel(HitResult,
		Start, End, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(5.f));;
}
