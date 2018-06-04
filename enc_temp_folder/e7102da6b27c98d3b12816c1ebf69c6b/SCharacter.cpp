// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//-------------- Lect 48: Adding spring arm component to cam create the camera component---------- -
	SpringArmComp = CreateAbstractDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true; // in order to rotate camera based on view rotation
	SpringArmComp->SetupAttachment(RootComponent);

	// -------------- Lect 50: Enable support for coruching -------------------
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;


	// -------- Lec 47 create the camera component -- use text macro to set name -----------
	CameraComp = CreateAbstractDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Lect 45 adding movement
void ASCharacter::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector() * value);
}

// Lect 45 adding movement
void ASCharacter::MoveSideways(float value)
{
	AddMovementInput(GetActorRightVector() * value);
}

// Lec 50
void ASCharacter::BeginCrouch()
{
	// already implemented in the engine
	Crouch();
}
// Lec 50
void ASCharacter::EndCrouch()
{
	// already implemented in the engine
	UnCrouch();
}


// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Lect 45 adding movement
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveSideways", this, &ASCharacter::MoveSideways);


	// Lect 46 adding mouse look rotations --> AddControllerYawInput is a built in function from the pawn class that handles
		// the looking up aspects of the rotation, i.e. rotation around the Y-axis .
	PlayerInputComponent->BindAxis("LookUp", this, &ASCharacter::AddControllerPitchInput);
		// handles the looking sideways aspects of player to mouse movement i.e., rotation around the Z-axis.
	PlayerInputComponent->BindAxis("Turn", this, &ASCharacter::AddControllerYawInput);

	// Lect 50: Create binding actions for the crouching mechanics of the game based on player input
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASCharacter::BeginCrouch);	// when crouch key pressed
	PlayerInputComponent->BindAction("Crouch", IE_Released , this, &ASCharacter::EndCrouch);	// when crouch released
}

