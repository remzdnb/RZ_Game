/// RemzDNB
///
///	RZ_CharacterAnimInstance.h
///	Basic animation system for humanoid characters.
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_AnimationSystem.h"
#include "Animation/AnimInstance.h"
#include "RZ_CharacterAnimInstance.generated.h"

UCLASS()
class RZ_ANIMATIONSYSTEM_API URZ_CharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	
	URZ_CharacterAnimInstance();
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:

	///
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartUseAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void StartReloadAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void StopAllAnimations();

	UFUNCTION()
	void SetHeadVisibility(bool bNewIsVisible);

private:

	//TWeakObjectPtr<ACharacter> OwnerCharacter;
	class IRZ_CharacterAnimInterface* OwnerCharacterAnimInterface;

	//
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Character", meta = (AllowPrivateAccess = "true"))
	ACharacter* OwnerCharacter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Character", meta = (AllowPrivateAccess = "true"))
	float Speed;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Character", meta = (AllowPrivateAccess = "true"))
	float ForwardSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Character", meta = (AllowPrivateAccess = "true"))
	float RightSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Character", meta = (AllowPrivateAccess = "true"))
	bool bIsMoving;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Character", meta = (AllowPrivateAccess = "true"))
	bool bIsHeadVisible;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Character", meta = (AllowPrivateAccess = "true"))
	FRZ_CharacterAnimData CharacterAnimData;

	///

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Anim Sequences", meta = (AllowPrivateAccess = "true"))
	class UAnimSequence* HitSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Anim Sequences", meta = (AllowPrivateAccess = "true"))
	class UAnimSequence* RifleAttackSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Anim Sequences", meta = (AllowPrivateAccess = "true"))
	class UAnimSequence* PistolAttackSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Anim Sequences", meta = (AllowPrivateAccess = "true"))
	class UAnimSequence* SwordAttackSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Anim Sequences", meta = (AllowPrivateAccess = "true"))
	class UAnimSequence* RifleReloadSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Anim Sequences", meta = (AllowPrivateAccess = "true"))
	class UAnimSequence* PistolReloadSequence;
};
