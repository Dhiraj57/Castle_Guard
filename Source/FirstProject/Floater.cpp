// Fill out your copyright notice in the Description page of Project Settings.


#include "Floater.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFloater::AFloater()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CustomStaticMesh"));

	InitialLocation = FVector(0.0f);
	PlacedLocation = FVector(0.0f);
	InitialDirection = FVector(0.0f);

	bInitializeFloaterLocations = false;
	bShouldFloat = false;

	InitialForce = FVector(2000000.f, 0.f, 0.0f);
	InitialTorque = FVector(2000000.f, 0.f, 0.0f);

}

// Called when the game starts or when spawned
void AFloater::BeginPlay()
{
	Super::BeginPlay();  

	InitialLocation.X = FMath::FRand() * 500.f ;
	InitialLocation.Y = FMath::FRand() * 500.f;
	InitialLocation.Z = FMath::FRand() * 500.f;

	PlacedLocation = GetActorLocation();

	if(bInitializeFloaterLocations)
	{
		SetActorLocation(InitialLocation);
	}

	//StaticMesh->AddForce(InitialForce);
	//StaticMesh->AddTorque(InitialTorque);
	
}

// Called every frame
void AFloater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bShouldFloat)
	{

		FVector NewLocation = GetActorLocation();
		NewLocation.Z += FMath::Sin(RunningTime);
		SetActorLocation(NewLocation);

		RunningTime += DeltaTime;

		//FHitResult HitResult;
		//AddActorLocalOffset(InitialDirection, true, &HitResult);

		//FVector HitLocation = HitResult.Location;
	}

}

