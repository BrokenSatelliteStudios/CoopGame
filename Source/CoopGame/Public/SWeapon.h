// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;

UCLASS()
class COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();

protected:

	// ------------------ Lect 54: Setting up the weapon -------------------
		// create weapon skeletal mesh component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComp;


	// ---------------- Lect 67: Cleaning up code adding modularity
	void PlayFireEffects(FVector TraceEnd);


	// ----------------- Lect 58: ------------------------
		// create a TSubclassOf for the ApplyPointDamage function in the cpp file
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")	// defaultsonly because we dont need to change at runtime
	TSubclassOf<UDamageType> DamageType;

	// ------------------- Lect 60: -------------------
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;	

	// Lect 60
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* MuzzleEffect;

	// Lect 60
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* ImpactEffect;

	// ------------------ Lect 61: Creating a tracer effect particle system, simulates smoke trail of bullet -------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* TracerEffect;

	// ------------------- Lect 61: -------------------
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName TracerTargetName;

	// -------------------- Lect 68: Adding Camera Shake -----------------
	// This subclass is needed for the camera shake function
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UCameraShake> FireCamShake;


public:	
	// moved the Fire function here during lecture 67
	// ---------------- Lect 56: Line Tracing for shooting ------------------
	// Bound function so we can make it callable from blueprints.
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Fire();		// ----------- Lect 63 we made this function virtual so that we could override it from the derived class for projectiles

	
};
