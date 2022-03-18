#include "RZ_CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

URZ_CharacterMovementComponent::URZ_CharacterMovementComponent()
{
}

FNetworkPredictionData_Client* URZ_CharacterMovementComponent::GetPredictionData_Client() const
{
	if (ClientPredictionData == nullptr)
	{
		// Return our custom client prediction class instead
		URZ_CharacterMovementComponent* MutableThis = const_cast<URZ_CharacterMovementComponent*>(this);
		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_My(*this);
	}

	return ClientPredictionData;
}

void URZ_CharacterMovementComponent::SetSprinting(bool sprinting)
{
	SprintKeyDown = sprinting;
}

void URZ_CharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	/* We don't want simulated proxies detecting their own collision
	if (GetPawnOwner()->GetLocalRole() > ROLE_SimulatedProxy)
	{
		// Bind to the OnActorHot component so we're notified when the owning actor hits something (like a wall)
		GetPawnOwner()->OnActorHit.AddDynamic(this, &URZ_CharacterMovementComponent::OnActorHit);
	}*/
}

void URZ_CharacterMovementComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	/*if (GetPawnOwner() != nullptr && GetPawnOwner()->GetLocalRole() > ROLE_SimulatedProxy)
	{
		// Unbind from all events
		GetPawnOwner()->OnActorHit.RemoveDynamic(this, &URZ_CharacterMovementComponent::OnActorHit);
	}

	Super::OnComponentDestroyed(bDestroyingHierarchy);*/
}

void URZ_CharacterMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// Peform local only checks
	if (GetPawnOwner()->IsLocallyControlled())
	{
		if (SprintKeyDown == true)
		{
			/* Only set WantsToSprint to true if the player is moving forward (so that he can't sprint backwards)
			FVector velocity2D = GetPawnOwner()->GetVelocity();
			FVector forward2D = GetPawnOwner()->GetActorForwardVector();
			velocity2D.Z = 0.0f;
			forward2D.Z = 0.0f;
			velocity2D.Normalize();
			forward2D.Normalize();

			WantsToSprint = FVector::DotProduct(velocity2D, forward2D) > 0.5f;*/
			WantsToSprint = true;
		}
		else
		{
			WantsToSprint = false;
		}

		// Update if the required wall run key(s) are being pressed
		//WallRunKeysDown = AreRequiredWallRunKeysDown();
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URZ_CharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	/*  There are 4 custom move flags for us to use. Below is what each is currently being used for:
		FLAG_Custom_0		= 0x10, // Sprinting
		FLAG_Custom_1		= 0x20, // WallRunning
		FLAG_Custom_2		= 0x40, // Unused
		FLAG_Custom_3		= 0x80, // Unused
	*/

	// Read the values from the compressed flags
	WantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
	WallRunKeysDown = (Flags & FSavedMove_Character::FLAG_Custom_1) != 0;
}

float URZ_CharacterMovementComponent::GetMaxSpeed() const
{
	switch (MovementMode)
	{
	case MOVE_Walking:
	case MOVE_NavWalking:
	{
		if (IsCrouching())
		{
			return MaxWalkSpeedCrouched;
		}
		else
		{
			if (WantsToSprint)
			{
				return SprintSpeed;
			}

			return RunSpeed;
		}
	}
	case MOVE_Falling:
		return RunSpeed;
	case MOVE_Swimming:
		return MaxSwimSpeed;
	case MOVE_Flying:
		return MaxFlySpeed;
	case MOVE_Custom:
		return MaxCustomMovementSpeed;
	case MOVE_None:
	default:
		return 0.f;
	}
}

float URZ_CharacterMovementComponent::GetMaxAcceleration() const
{
	if (IsMovingOnGround())
	{
		if (WantsToSprint)
		{
			return SprintAcceleration;
		}

		return RunAcceleration;
	}

	return Super::GetMaxAcceleration();
}

void FSavedMove_My::Clear()
{
	Super::Clear();

	// Clear all values
	SavedWantsToSprint = 0;
	SavedWallRunKeysDown = 0;
}

uint8 FSavedMove_My::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	/* There are 4 custom move flags for us to use. Below is what each is currently being used for:
	FLAG_Custom_0		= 0x10, // Sprinting
	FLAG_Custom_1		= 0x20, // WallRunning
	FLAG_Custom_2		= 0x40, // Unused
	FLAG_Custom_3		= 0x80, // Unused
	*/

	// Write to the compressed flags 
	if (SavedWantsToSprint)
		Result |= FLAG_Custom_0;
	if (SavedWallRunKeysDown)
		Result |= FLAG_Custom_1;

	return Result;
}

bool FSavedMove_My::CanCombineWith(const FSavedMovePtr& NewMovePtr, ACharacter* Character, float MaxDelta) const
{
	const FSavedMove_My* NewMove = static_cast<const FSavedMove_My*>(NewMovePtr.Get());

	// As an optimization, check if the engine can combine saved moves.
	if (SavedWantsToSprint != NewMove->SavedWantsToSprint ||
		SavedWallRunKeysDown != NewMove->SavedWallRunKeysDown)
	{
		return false;
	}

	return Super::CanCombineWith(NewMovePtr, Character, MaxDelta);
}

void FSavedMove_My::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	URZ_CharacterMovementComponent* charMov = static_cast<URZ_CharacterMovementComponent*>(Character->GetCharacterMovement());
	if (charMov)
	{
		// Copy values into the saved move
		SavedWantsToSprint = charMov->WantsToSprint;
		SavedWallRunKeysDown = charMov->WallRunKeysDown;
	}
}

void FSavedMove_My::PrepMoveFor(class ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	URZ_CharacterMovementComponent* charMov = Cast<URZ_CharacterMovementComponent>(Character->GetCharacterMovement());
	if (charMov)
	{
		// Copt values out of the saved move
		charMov->WantsToSprint = SavedWantsToSprint;
		charMov->WallRunKeysDown = SavedWallRunKeysDown;
	}
}

FNetworkPredictionData_Client_My::FNetworkPredictionData_Client_My(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{

}

FSavedMovePtr FNetworkPredictionData_Client_My::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_My());
}