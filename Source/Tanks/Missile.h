// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Missile.generated.h"

UCLASS()
class TANKS_API AMissile : public AActor
{
    GENERATED_BODY()
    
public:	
    // Sets default values for this actor's properties
    AMissile();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    float Speed;

    FTimerHandle ExplodeTimerHandle;

    void Explode();

    // En caso de no existir OnExplode, llamara al OnExplode_Implementation
    UFUNCTION(BlueprintNativeEvent, Category = "Projectile")
    void OnExplode();
    virtual void OnExplode_Implementation();

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
    class UPaperSpriteComponent* MissileSprite;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
    class UPaperFlipbookComponent* MissileFlipbook;
    
};
