// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Tank.h"
#include "Missile.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "PaperSpriteComponent.h"

// Sets default values
ATurret::ATurret()
{
     // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    
    TurretDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("TurretDirection"));
    RootComponent = (USceneComponent*) TurretDirection;
    
    TurretSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("TurretSprite"));
    TurretSprite->AttachToComponent(TurretDirection, FAttachmentTransformRules::KeepRelativeTransform);

    YawSpeed = 180.f;
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
    Super::BeginPlay();
    AddTickPrerequisiteActor(GetParentComponent()->GetOwner());

    Tank = Cast<ATank>(GetParentComponent()->GetOwner());
    //check(Tank);
    
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    check(TurretDirection);

    if (Tank != nullptr)
    {
        if (APlayerController* PC = Cast<APlayerController>(Tank->GetController()))
        {
            FVector2D AimLocation;
            if (PC->GetMousePosition(AimLocation.X, AimLocation.Y))
            {
                FVector2D TurretLocation = FVector2D::ZeroVector;
                UGameplayStatics::ProjectWorldToScreen(PC, TurretDirection->GetComponentLocation(), TurretLocation);

                float DesiredYaw;
                

                if (UTankStatics::FindLookAtAngle(TurretLocation, AimLocation, DesiredYaw))
                {
                    FRotator CurrentRotation = TurretDirection->GetComponentRotation();
                    float DeltaYaw = UTankStatics::FindDeltaAngleDegrees(CurrentRotation.Yaw, DesiredYaw);
                    float MaxDeltaYawThisFrame = YawSpeed*DeltaTime;
                    if (MaxDeltaYawThisFrame > FMath::Abs(DesiredYaw))
                    {
                        CurrentRotation.Yaw = DesiredYaw;
                    }else
                    {
                        CurrentRotation.Yaw += FMath::Sign(DeltaYaw) * MaxDeltaYawThisFrame;
                    }
                    
                    TurretDirection->SetWorldRotation(CurrentRotation);
                }

            }

            const FTankInput& CurrentInput = Tank->GetCurrentInput();
            if (CurrentInput.bFire1 && Projectile)
            {
                if (UWorld* World = GetWorld())
                {
                    if (AMissile* NewProjectile = Cast<AMissile>(World->SpawnActor(Projectile)))
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Fire!!"));
                        FVector Loc = TurretSprite->GetSocketLocation("Muzzle");
                        FRotator Rot = TurretSprite->GetComponentRotation();

                        NewProjectile->SetActorLocation(Loc);
                        NewProjectile->SetActorRotation(Rot);
                    }
                }
            }
        }
    }

}

