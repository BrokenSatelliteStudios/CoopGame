// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SWeapon.h" // Lecture 67

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


	// ----------- Lect 65: Adding ADS ------------- binding action for ADS using right click
	ZoomedFOV = 65.0f;
	ZoomInterpSpeed = 20;

	// ------------------ Lect 67: Cleaning up code ----------------
	WeaponAttachSocketName = "WeaponSocket";
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// ----------- Lect 65: Adding ADS -------------
	DefaultFOV = CameraComp->FieldOfView;	// get field of view from start

	// ----------- Lect 67: Cleaning up code -- Adding gun spawning at player hands functionality from non blueprint -------------

	// Spawns the StarterWeaponClass at a location
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CurrentWeapon = GetWorld()->SpawnActor<ASWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (CurrentWeapon)
	{
		//  *** set owner to player character. This is important otherwise the player cant actually use the gun
		CurrentWeapon->SetOwner(this);
		// attach current weapon to player mesh at the WeaponSocket we specified in the editor
			// Must use GetMesh() to get the mesh of the player because even if we are in derived class from ACharacter we cant access it
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponAttachSocketName);
	}
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

									// ----------- Lect 65: Adding ADS -------------

		// sets CurrentFov to ZoomedFOV if bWantsToZoom is true, otherwise set to DefaultFOV
	// we put the FOV update here so that we can interpolate it and smoothen it out with delta time
	float TargetFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV;
	// interpolate
	float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);

	CameraComp->SetFieldOfView(NewFOV);
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
// Lec 51
void ASCharacter::BeginJump()
{
	Jump();
}

// ----------- Lect 65: Adding ADS ------------- binding action for ADS using right click
void ASCharacter::BeginZoom()
{
	bWantsToZoom = true;
}
// ----------- Lect 65: Adding ADS ------------- binding action for ADS using right click
void ASCharacter::EndZoom()
{
	bWantsToZoom = false;
}


// ------------- Lect 67: Code cleanup - adding code based fire mechanics -------------
void ASCharacter::Fire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Fire();
	}
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

	// Lec 51: adding a jump mechanism by binding an event action to the player input on keyward Jump using the ASCharacter class Jump() function found here http://api.unrealengine.com/INT/API/Runtime/Engine/GameFramework/ACharacter/index.html
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASCharacter::BeginJump);


	// ----------- Lect 65: Adding ADS ------------- binding action for ADS using right click
	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ASCharacter::BeginZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ASCharacter::EndZoom);


	// ----------- Lect 67: Cleaning code -- adding fire mechanics -- here we bind the event of fire to our pressing of fire
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASCharacter::Fire);
}

FVector ASCharacter::GetPawnViewLocation() const
{
	// dont need base implementation of this function so we dont call super
	
	// check if there is a camera and if so return it
	if (CameraComp)
	{
		return CameraComp->GetComponentLocation();	// return the location of the camera component
	}

	// else call super and shoot from original location (actor head location) which is the base default implementation
	return Super::GetPawnViewLocation();
}

