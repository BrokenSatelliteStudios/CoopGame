// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"


// forward declare cameracomp class
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class COOPGAME_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	void MoveForward(float value);
	void MoveSideways(float value);

	// ------------------ Lect 47: Adding camera component -----------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	// ------------- Lect 48: Adding spring arm component to cam create the camera component -----------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	// ------------ Lect 50: Adding animations to player. ---------------------
		// creating functions for crouching
	void BeginCrouch();

	void EndCrouch();

	// ----------- Lect 51: Adding a jump mechanic ----------------------
	void BeginJump();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	// ------------ Lect 57: Setting line trace from camera as eyePosition ----------------
		// this function is provided by urneal engine in the pawn class. Our camera is a pawn so we use this
	virtual FVector GetPawnViewLocation() const override;
};
