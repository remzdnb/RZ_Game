#include "RZ_CharacterAnimInstance.h"
#include "RZ_CharacterAnimInterfaces.h"
//
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

URZ_CharacterAnimInstance::URZ_CharacterAnimInstance() : 
	Speed(0.0f)
{
}

void URZ_CharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	OwnerCharacterAnimInterface = Cast<IRZ_CharacterAnimInterface>(OwnerCharacter);
}

void URZ_CharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	
	if (OwnerCharacter)
		return;

	if (OwnerCharacterAnimInterface == nullptr)
		return;

	CharacterAnimData = OwnerCharacterAnimInterface->GetCharacterAnimData();

	ForwardSpeed = UKismetMathLibrary::Quat_UnrotateVector(OwnerCharacter->GetActorRotation().Quaternion(), OwnerCharacter->GetVelocity()).X /
		OwnerCharacter->GetMovementComponent()->GetMaxSpeed();

	RightSpeed = UKismetMathLibrary::Quat_UnrotateVector(OwnerCharacter->GetActorRotation().Quaternion(), OwnerCharacter->GetVelocity()).Y /
		OwnerCharacter->GetMovementComponent()->GetMaxSpeed();
	
}
