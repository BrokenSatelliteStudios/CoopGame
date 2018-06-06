// Fill out your copyright notice in the Description page of Project Settings.

#include "SProjectileWeapon.h"


void ASProjectileWeapon::Fire()
{
	// Get owner (player) reference so that we can get their eye location
	AActor* MyOwner = GetOwner();

	if (MyOwner && ProjectileClass)	// make sure projectile class was acually set otherwise dont execute
	{
		// craete FVector for EyeLoc and FRotator for EyeRot to pass in to GetActorEyesViewPoint() function
		FVector EyeLocation;
		FRotator EyeRotation;

		// get the MyOwner's eye position and rotayion for starting position of line trace
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);	// we really just need to get the Eye Rotation of the actor player


		// we already have a mesh component in the base class that we use for the muzzle so we just create a vector position for it locally here
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;	// set actor to always spawn

		// Spawn the projectile actor class
		GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, EyeRotation, SpawnParams);
	}
}

