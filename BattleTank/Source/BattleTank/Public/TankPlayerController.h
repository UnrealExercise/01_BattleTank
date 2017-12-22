// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tank.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h" // Must be the last include

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    virtual void BeginPlay() override;

    virtual void Tick(float deltaTime) override;

    ATank*  GetControlledTank() const;

    // return an OUT parameter, true if hit landscape
    bool GetSightRayHitLocation(FVector& OutHitLocation) const;

    bool GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const;

    bool GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const;

    float GetCrossHairX() const { return m_crosshairX; }

    float GetCrossHairY() const { return m_crosshairY; }

    void SetCrossHairX(float crosshairX);

    void SetCrossHairY(float crosshairY);

    float GetLineTraceRange() const { return m_lineTraceRange; }

    void SetLineTraceRange(float lineTraceRange);

    // Moving the barrel towards the crosshair position every frame
    // To make sure the shot hit exactly the position where player aiming at 
    void AimTowardsCrosshair();

private:
    UPROPERTY(EditAnywhere)
    float m_crosshairX = 0.5f;

    UPROPERTY(EditAnywhere)
    float m_crosshairY = 0.3333f;

    UPROPERTY(EditAnywhere)
    float m_lineTraceRange = 1000000.0f;
};
