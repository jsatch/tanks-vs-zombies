// Fill out your copyright notice in the Description page of Project Settings.


#include "Missile.h"
#include "PaperFlipbook.h"
#include "TimerManager.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"

// Sets default values
AMissile::AMissile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Speed = 200.f;

	if (!RootComponent)
    {
        RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MissileBase"));
    }

	MissileSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MissileSprite"));
	MissileSprite->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	MissileFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("MissileFlipbook"));
	MissileFlipbook->AttachToComponent(MissileSprite, FAttachmentTransformRules::KeepRelativeTransform);
	
}

// Called when the game starts or when spawned
void AMissile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(ExplodeTimerHandle, this,&AMissile::Explode, 1.f);
}

// Called every frame
void AMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector Loc = GetActorLocation();
	Loc += (DeltaTime * Speed) * GetTransform().GetUnitAxis(EAxis::Z);
	SetActorLocation(Loc);
}

void AMissile::Explode()
{
	UE_LOG(LogTemp, Warning, TEXT("Entra Explode"));
	GetWorldTimerManager().ClearTimer(ExplodeTimerHandle);
	explosionar();
}

/*void AMissile::OnExplode_Implementation()
{
	Destroy();
}*/

void AMissile::explosionar()
{
	// just in case
	if (Flipbook)
	{
		UE_LOG(LogTemp, Warning, TEXT("Entra a explosionar"));
		MissileFlipbook->SetFlipbook(Flipbook);
		MissileFlipbook->Play();
		MissileFlipbook->SetLooping(false);
		//MissileFlipbook->OnFinishedPlaying.Add(&AMissile::FinishExplosion);
		
	}
	UE_LOG(LogTemp, Warning, TEXT("Debe finalizar"));
	Destroy();
}

