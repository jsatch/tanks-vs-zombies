// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Components/ArrowComponent.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"

void FTankInput::Sanitize()
{
    MovementInput = RawMovementInput.ClampAxes(-1.f, 1.f);
    MovementInput = MovementInput.GetSafeNormal();
    RawMovementInput.Set(0.f,0.f);
}
void FTankInput::MoveX(float AxisValue)
{
    RawMovementInput.X  += AxisValue;
}
void FTankInput::MoveY(float AxisValue)
{
    RawMovementInput.Y += AxisValue;
}

void FTankInput::Fire1(bool bPressed)
{
    bFire1 = bPressed;
}
void FTankInput::Fire2(bool bPressed)
{
    bFire2 = bPressed;
}

// Sets default values
ATank::ATank()
{
     // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    if (!RootComponent)
    {
        RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TankBase"));
    }

    TankDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("TankDirection"));
    TankDirection->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

    TankSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("TankSprite"));
    TankSprite->AttachToComponent(TankDirection, FAttachmentTransformRules::KeepRelativeTransform);

    USpringArmComponent* SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->TargetArmLength = 500.f;
    SpringArm->bEnableCameraLag = true;
    SpringArm->bEnableCameraRotationLag = false;
    SpringArm->bUsePawnControlRotation = false;
    SpringArm->CameraLagSpeed = 2.f;
    SpringArm->bDoCollisionTest = false;
    SpringArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    SpringArm->SetWorldRotation(FRotator(0.f, 0.f, 0.f));

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->bUsePawnControlRotation = false;
    CameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
    CameraComponent->OrthoWidth = 1024.f;
    CameraComponent->AspectRatio = 3.0f / 4.f;
    CameraComponent->SetWorldRotation(FRotator(0.f, 0.f, 0.f));
    CameraComponent->AttachToComponent(SpringArm, 
        FAttachmentTransformRules::KeepRelativeTransform,USpringArmComponent::SocketName);

    ChildTurret = CreateDefaultSubobject<UChildActorComponent>(TEXT("Turret"));
    ChildTurret->AttachToComponent(TankDirection, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
    Super::BeginPlay();
    
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    TankInput.Sanitize();
    //UE_LOG(LogTemp, Warning, TEXT("Movement: (%f %f)"), TankInput.MovementInput.X, TankInput.MovementInput.Y);

    {
        FVector DesiredMovementDirection = FVector(TankInput.MovementInput.X, TankInput.MovementInput.Y, 0.f);

        if (!DesiredMovementDirection.IsNearlyZero())
        {
            FRotator MovementAngle = DesiredMovementDirection.Rotation();
            float DeltaYaw = UTankStatics::FindDeltaAngleDegrees(
                TankDirection->GetComponentRotation().Yaw,
                MovementAngle.Yaw);
            bool bReverse = false;
            if (DeltaYaw != 0.f)
            {
                float AdjustedDeltaYaw = DeltaYaw;

                if (AdjustedDeltaYaw < -90.f)
                {
                    bReverse = true;
                    AdjustedDeltaYaw += 180;
                }else if(AdjustedDeltaYaw > 90.f)
                {
                    bReverse = true;
                    AdjustedDeltaYaw -= 180;
                }

                float MaxYawThisFrame = YawSpeed * DeltaTime;
                if (MaxYawThisFrame >= FMath::Abs(AdjustedDeltaYaw))
                {
                    if (bReverse)
                    {
                        FRotator FacingAngle = MovementAngle;
                        FacingAngle.Yaw = MovementAngle.Yaw + 180.f;
                        TankDirection->SetWorldRotation(FacingAngle);
                    }else
                    {
                        TankDirection->SetWorldRotation(MovementAngle);
                    }
                    
                }else
                {
                    TankDirection->AddLocalRotation(
                        FRotator(0.f, FMath::Sign(AdjustedDeltaYaw)* MaxYawThisFrame, 0.f)
                    );
                }
            }

            // Tank movement
            {
                FVector MovementDirection = TankDirection->GetForwardVector() * (bReverse ? -1.f : 1.f);
                FVector Pos = GetActorLocation();

                Pos.X += MovementDirection.X * MoveSpeed * DeltaTime;
                Pos.Y += MovementDirection.Y * MoveSpeed * DeltaTime; 

                SetActorLocation(Pos);
            }
        }

    }

}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveX", this, &ATank::MoveX);
    PlayerInputComponent->BindAxis("MoveY", this, &ATank::MoveY);
    PlayerInputComponent->BindAction("Fire1", EInputEvent::IE_Pressed, this, &ATank::Fire1Pressed);
    PlayerInputComponent->BindAction("Fire1", EInputEvent::IE_Released, this, &ATank::Fire1Released);
    PlayerInputComponent->BindAction("Fire2", EInputEvent::IE_Pressed, this, &ATank::Fire2Pressed);
    PlayerInputComponent->BindAction("Fire2", EInputEvent::IE_Released, this, &ATank::Fire2Released);
}

void ATank::MoveX(float AxisValue)
{
    TankInput.MoveX(AxisValue);
}
void ATank::MoveY(float AxisValue)
{
    TankInput.MoveY(AxisValue);
}

void ATank::Fire1Pressed()
{
    TankInput.Fire1(true);
}
void ATank::Fire1Released()
{
    TankInput.Fire1(false);
}
void ATank::Fire2Pressed()
{
    TankInput.Fire2(true);
}
void ATank::Fire2Released()
{
    TankInput.Fire2(false);
}