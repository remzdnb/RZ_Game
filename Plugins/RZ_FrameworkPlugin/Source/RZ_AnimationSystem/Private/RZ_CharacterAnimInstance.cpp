#include "RZ_CharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

URZ_CharacterAnimInstance::URZ_CharacterAnimInstance() : 
	Speed(0.0f),
	ForwardSpeed(0.0f),
	RightSpeed(0.0f)
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

	Speed = 0.0f;
	ForwardSpeed = 0.0f;
	RightSpeed = 0.0f;
	
	if (OwnerCharacter == nullptr)
		return;

	if (OwnerCharacterAnimInterface == nullptr)
		return;

	//

	Speed = OwnerCharacter->GetVelocity().Size();
	bIsMoving = Speed > 0.0f ? true : false; 

	ForwardSpeed = UKismetMathLibrary::Quat_UnrotateVector(OwnerCharacter->GetActorRotation().Quaternion(), OwnerCharacter->GetVelocity()).X /
		1;

	RightSpeed = UKismetMathLibrary::Quat_UnrotateVector(OwnerCharacter->GetActorRotation().Quaternion(), OwnerCharacter->GetVelocity()).Y /
		1;
	//

	CharacterAnimData = OwnerCharacterAnimInterface->GetCharacterAnimData();
}

void URZ_CharacterAnimInstance::SetHeadVisibility(bool bNewIsVisible)
{
	bIsHeadVisible = bNewIsVisible;
}
