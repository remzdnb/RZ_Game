/// RemzDNB
///
///	RZ_CharacterAnimInstance.h
///	Basic animation system for humanoid characters.
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_CharacterAnimPluginTypes.h"
//
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RZ_BaseCharacterAnimInstance.generated.h"

UCLASS()
class RZ_CHARACTERANIMPLUGIN_API URZ_CharacterAnimInstance : public UAnimInstance
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
	void StartAttackAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void StartReloadAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void StopAllAnimations();

private:

	TWeakObjectPtr<ACharacter> OwnerCharacter;
	class IRZ_CharacterAnimInterface* OwnerCharacterAnimInterface;

	//

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Speed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float ForwardSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RightSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
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
