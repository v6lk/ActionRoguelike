// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	barrel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Barrel Comp"));
	barrel->SetSimulatePhysics(true);
	barrel->SetNotifyRigidBodyCollision(true);

	barrel->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
	barrel->OnComponentHit.AddDynamic(this, &AExplosiveBarrel::OnHit);

	// Set as root component
	RootComponent = barrel;

}
// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExplosiveBarrel::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//Apply force to all objects in vicinity
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(10, 15.0f, FColor::Red, FString::Printf(TEXT("Barrel Hit!")));
}
