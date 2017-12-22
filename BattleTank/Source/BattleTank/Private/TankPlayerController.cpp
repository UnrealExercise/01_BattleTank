// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"

void ATankPlayerController::BeginPlay()
{
    Super::BeginPlay();

    auto ControlledTank = GetControlledTank();

    if (!ControlledTank)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerController not possessing a tank"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerController possessing: %s"), *(ControlledTank->GetName()));
    }
}

void ATankPlayerController::Tick(float deltaTime)
{
    Super::Tick(deltaTime);

    AimTowardsCrosshair();
}

ATank*  ATankPlayerController::GetControlledTank() const
{
    return Cast<ATank>(GetPawn());
}

// Get world location of linetrace through crosshair, true if hits landscape
bool ATankPlayerController::GetSightRayHitLocation(FVector & OutHitLocation) const
{
    // Find the crosshair position in pixel coordinates
    int32 ViewportSizeX, ViewportSizeY;
    GetViewportSize(ViewportSizeX, ViewportSizeY);

    auto ScreenLocation = FVector2D(ViewportSizeX * GetCrossHairX(), ViewportSizeY * GetCrossHairY());

    // "De-project" the screen position of the crosshair to a world direction 
    FVector LookDirection;
    if (GetLookDirection(ScreenLocation, LookDirection))
    {
        // Line-trace along that look direction, and see what we hit (up to max range)
        GetLookVectorHitLocation(LookDirection, OutHitLocation);
    }
   
    return true;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector & LookDirection) const
{
    FVector CameraWorldLocation;    // To be discarded  
    return DeprojectScreenPositionToWorld(ScreenLocation.X,
                                          ScreenLocation.Y, 
                                          CameraWorldLocation, 
                                          LookDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector & HitLocation) const
{
    FHitResult HitResult;
    auto StartLocation = PlayerCameraManager->GetCameraLocation();
    auto EndLocation = StartLocation + (LookDirection * GetLineTraceRange());
    
    if (GetWorld()->LineTraceSingleByChannel(HitResult,
                                             StartLocation,
                                             EndLocation,
                                             ECollisionChannel::ECC_Visibility))
    {
        HitLocation = HitResult.Location;

        return true;
    }
    HitLocation = FVector(0);

    return false;   // Line-trace didn't succeed
}

void ATankPlayerController::SetCrossHairX(float crosshairX)
{
    m_crosshairX = crosshairX;
}

void ATankPlayerController::SetCrossHairY(float crosshairY)
{
    m_crosshairY = crosshairY;
}

void ATankPlayerController::SetLineTraceRange(float lineTraceRange)
{
    m_lineTraceRange = lineTraceRange;
}

void ATankPlayerController::AimTowardsCrosshair()
{
    if (!GetControlledTank()) { return; }

    FVector HitLocation;     // Out parameter

    if (GetSightRayHitLocation(HitLocation))    // Has "side-effect", is going to linetrace
    {
        UE_LOG(LogTemp, Warning, TEXT("Hit Location: %s"), *HitLocation.ToString());
        // TODO: Tell controlled tank to aim at this point.
    }
}