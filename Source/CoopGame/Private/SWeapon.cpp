// Fill out your copyright notice in the Description page of Project Settings.

#include "SWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"				// Lect 60
#include "Components/SkeletalMeshComponent.h"		// Lect 55
#include "Particles/ParticleSystemComponent.h"		// Lect 61

// ----------- Lect 66: Creating a console variable to handle debugging -----------
static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(
	TEXT("COOP.DebugWeapons"), 
	DebugWeaponDrawing, 
	TEXT("Draw Debug Lines for Weapons."), 
	ECVF_Cheat);

// Sets default values
ASWeapon::ASWeapon()
{
	// ------------------- Lect 55: Create the skeletal mesh for weapon -------------------

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
		// set skeletal mesh as root component in heirarchy
	RootComponent = MeshComp;


	// --------------------- Lect 60 --------------------

		// creating the FName for our muzzle flash
	MuzzleSocketName = "MuzzleSocket";

	// --------------------- Lect 61 --------------------
	TracerTargetName = "Target";	// this is already in the editor, but we do it by code anyways
}


// -------------- Lect 56: Implementation of the fire function -------------------


void ASWeapon::Fire()
{
	// Trace the world from pawn eyes to crosshair location

		// determine who is holding the weapon and get reference to them
	AActor* MyOwner = GetOwner();
		// validate there is an owner
	if (MyOwner)
	{
		// craete FVector for EyeLoc and FRotator for EyeRot to pass in to GetActorEyesViewPoint() function
		FVector EyeLocation;
		FRotator EyeRotation;

			// ------------- Lect 58: Vector to set shot direction of line cast -----------------
		FVector ShotDirection = EyeRotation.Vector();

		// get the MyOwner's eye position for starting position of line trace
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);	// this returns the Loc and Rot of the Actors eyes to our Loc and Rot Variables

		// hit result that holds data (is a struct filled with data containing what was hit, how far it was, and the hit normal)
			// ****** very important variable as it holds all the data of what was hit by the line cast *******
		FHitResult Hit;

		// End location is set by taking the initial trace point and adding a distance vec to it, this will determine the tracing distance of the line
		// This is also determined by the direction the player is looking, i.e., the rotation which we cast into a vector
		float DistanceMultiplier = 10000;
		FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * DistanceMultiplier);

		// FCollisionQueryParams: struct
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);	// ignore player
		QueryParams.AddIgnoredActor(this);		// ignore weapon
		QueryParams.bTraceComplex = true;		// traces against each individual polygon of the mesh we are hitting for more accurate hit scan 
												// and accurately sets hit points on target. Expensive but necessary for accurate hits (headshots)

		// --------------------------- Lect 61: Particle: Target parameter -------------------------


		// This vector is the direction the particle smoke trail will follow. If our line trace does not hit an object, the trail will follow
		// the TraceEnd direction of the line trace. If it does hit an object. we will grab its direction for the Hit variable
		FVector TracerEndPoint = TraceEnd;

		// Start line trace	--- trace function returnse a boolean, true if hit object, false otherwise
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, /*collision channel*/ ECC_Visibility, QueryParams))// anything visible blocks trace
		{
			// Blocking hit made. Processes damage.


			// ----------------------- Lect 58: Damaging an Actor --------------------

				// make reference to actor 
			AActor* HitActor = Hit.GetActor();

				// apply damange to actor	// there are different types of damage appliers avaiable to us from this class but PointDamage 
											// makes sense since we are doing a hit scan.
			UGameplayStatics::ApplyPointDamage(HitActor, 20.0f, ShotDirection, Hit, MyOwner->GetInstigatorController(), this, DamageType);

			// ---------------------- Lect 60: Play Impact effect only if line trace hit something -------------------------
				// check if ImpactEffect assigned and play it only if it was
			if (ImpactEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation(), true);
			}

			// ----------------------------- Lect 61 ----------------------------------

				// if tracer hit something just overwrite the default TraceEnd value of the TracerEndPoint with the normal of the point hit
			TracerEndPoint = Hit.ImpactPoint;
		}

		// Draw visible line cast line	and show only if console varialbe set to show
		if (DebugWeaponDrawing > 0)
		{
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);
		}
		
		// ----------------- Lect 60: check to see MuzzleEffect was assigned and then play muzzle flas only if it was


		if (MuzzleEffect)
		{
			// ---------------------- Lect 60: Setting up muzzle flash particle effect -------------------------
			UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, /*Comp to attach to*/MeshComp, /*attach point name*/ MuzzleSocketName); // attached bc the muzzle will move and we want the particle effect to follow its movement
		}


		// ------------------ Lect 61: Smoke tracer particle effect -------------------
			// check to make sure the TracerEffect was assigned from the blueprint.


		if (TracerEffect)
		{
			FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);	// gets the location of the socket we set on the gun from the skeleton,
			// we want this smoke trail effect to play even if our trace didnt hit anything
				
			// Create TracerComp to recieve the return value of the SpawnEmitterAtLocation function (happens at runtime)
			UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, /*muzzle loc*/MuzzleLocation);

			// specify the target for the trail
			if (TracerComp)
			{
				TracerComp->SetVectorParameter(TracerTargetName, /*set loc*/ TracerEndPoint);	// param is TargetName found in smoke trail particle system in editor
			}

		}
	}
}

void ASWeapon::PlayFireEffects()
{

}

