// Fill out your copyright notice in the Description page of Project Settings.

// --------------------- Lect 63: Entire class is created in lecture 63

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "SProjectileWeapon.generated.h"

/**
 * 
 */
UCLASS()
class COOPGAME_API ASProjectileWeapon : public ASWeapon
{
	GENERATED_BODY()
	
	
protected:
	
	virtual void Fire() override;

	UPROPERTY(EditDefaultsonly, Category = "ProjectileWeapon")
	TSubclassOf<AActor> ProjectileClass;
};
