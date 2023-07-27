// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/PrimitiveComponent.h"
#include "Gameframework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Math/Vector.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

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

	ExplosionRadius = 500.0f;

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
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(10, 15.0f, FColor::Red, FString::Printf(TEXT("Barrel Hit!")));

	ACharacter* otherChar = Cast<ACharacter>(OtherActor);
	if (!otherChar)
		return;

	FVector barrelLocation = GetActorLocation();

	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AActor* OtherActor = *ActorItr;

		if (OtherActor != this)
		{
			FVector otherActorLoc = OtherActor->GetActorLocation();

			if (FVector::Dist(barrelLocation, otherActorLoc) < ExplosionRadius)
			{

				ACharacter* character = Cast<ACharacter>(OtherActor);
				if (character)
				{

					if (UPawnMovementComponent* moveComp = character->GetMovementComponent())
					{
						// alternivly you can use  ERadialImpulseFalloff::RIF_Linear for the impulse to get linearly weaker as it gets further from origin.
						// set the float radius to 500 and the float strength to 2000.
						moveComp->AddRadialImpulse(GetActorLocation(), 500.f, 100.f, ERadialImpulseFalloff::RIF_Constant, true);
					}
				}
				else
				{
					UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>((OtherActor)->GetRootComponent());
					if (MeshComp)
					{
						MeshComp->AddRadialImpulse(GetActorLocation(), 500.f, 100.f, ERadialImpulseFalloff::RIF_Constant, true);
					}
				}

			}

		}
	}


}
