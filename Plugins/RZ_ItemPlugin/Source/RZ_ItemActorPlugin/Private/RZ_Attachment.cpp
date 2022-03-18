#include "RZ_Attachment.h"
// Engine
#include "Components/StaticMeshComponent.h"

ARZ_Attachment::ARZ_Attachment()
{
	/*BaseStaticMeshCT = CreateDefaultSubobject<UStaticMeshComponent>(FName("BaseStaticMeshCT"));
	BaseStaticMeshCT->SetCollisionProfileName("BlockAll");
	BaseStaticMeshCT->SetCustomDepthStencilValue(1);
	BaseStaticMeshCT->SetupAttachment(SceneCT);*/
}

void ARZ_Attachment::BeginPlay()
{
	Super::BeginPlay();

	/*if (DataManager)
	{
		const FRZ_AttachmentData* NewAttachmentData = DataManager->GetAttachmentDataFromRow(DataRowName);
		if (NewAttachmentData)
		{
			AttachmentData = *NewAttachmentData;
		}
		else
		{
			return;
		}
	}*/
}

/*void ARZ_Attachment::UpdateItemState(ERZ_ItemState NewItemState)
{
	Super::UpdateItemState(NewItemState);


}*/