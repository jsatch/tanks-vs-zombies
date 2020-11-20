// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tanks.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

USTRUCT(BlueprintType)
struct FTankInput
{
    GENERATED_BODY()
public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank Input")
    FVector2D MovementInput;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank Input")
    uint32 bFire1 : 1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank Input")
    uint32 bFire2 : 1;


    void Sanitize();
    void MoveX(float AxisValue);
    void MoveY(float AxisValue);
    void Fire1(bool bPressed);
    void Fire2(bool bPressed);
    
private:
    FVector2D RawMovementInput;
};

UCLASS()
class TANKS_API ATank : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    ATank();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:	
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category = "Tank")
    const FTankInput& GetCurrentInput() {return TankInput;}
    
private:
    void MoveX(float AxisValue);
    void MoveY(float AxisValue);
    void Fire1Pressed();
    void Fire1Released();
    void Fire2Pressed();
    void Fire2Released();

    // Tell which way the tank is facing
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank", meta = (AllowPrivateAccess = "true"))
    class UArrowComponent* TankDirection;

    // Sprite for the tank body.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank", meta = (AllowPrivateAccess = "true"))
    class UPaperSpriteComponent* TankSprite;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank", meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank", meta = (AllowPrivateAccess = "true"))
    UChildActorComponent* ChildTurret;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank Input")
    FTankInput TankInput;

    // Velocidada de giro
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tank", meta = (ClampMin = "0.0"))
    float YawSpeed;

    // Tank Speed
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tank", meta = (ClampMin = "0.0"))
    float MoveSpeed;
};
