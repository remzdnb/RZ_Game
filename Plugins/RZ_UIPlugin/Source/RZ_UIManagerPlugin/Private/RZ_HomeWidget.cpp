#include "RZ_HomeWidget.h"
//
#include "Kismet/GameplayStatics.h"

void URZ_HomeWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	/*GInstance = Cast<USB_GameInstance>(GetGameInstance());
	if (GInstance->GetSaveGame())
	{
		PlayerNameEText->SetText(FText::FromString(GInstance->GetSaveGame()->PlayerName));
		MapNameEText->SetText(FText::FromString(GInstance->GetSaveGame()->MapName));
		JoinIPEText->SetText(FText::FromString(GInstance->GetSaveGame()->JoinIP));
	}
	
	PlayerNameEText->OnTextValidated.AddDynamic(this, &URZ_HomeWidget::OnPlayerNameChanged);
	MapNameEText->OnTextValidated.AddUniqueDynamic(this, &URZ_HomeWidget::OnMapNameChanged);
	JoinIPEText->OnTextValidated.AddUniqueDynamic(this, &URZ_HomeWidget::OnMapNameChanged);
	LoadMapButton->OnButtonPressed.AddDynamic(this, &URZ_HomeWidget::OnLoadMapButtonPressed);
	JoinServerButton->OnButtonPressed.AddDynamic(this, &URZ_HomeWidget::OnJoinServerButtonPressed);
	CreateServerButton->OnButtonPressed.AddDynamic(this, &URZ_HomeWidget::OnCreateServerButtonPressed);*/
}

void URZ_HomeWidget::OnPlayerNameChanged(const FText& PlayerNameText)
{
	//GInstance->SavePlayerName(PlayerNameText.ToString());
}

void URZ_HomeWidget::OnMapNameChanged(const FText& MapNameText)
{
	//GInstance->GetSaveGame()->MapName = MapNameText.ToString();
	//GInstance->ApplySaveGame();
}

void URZ_HomeWidget::OnJoinIPChanged(const FText& JoinIPText)
{
	//GInstance->GetSaveGame()->JoinIP = JoinIPText.ToString();
	//GInstance->ApplySaveGame();
}

void URZ_HomeWidget::OnLoadMapButtonPressed(uint8 ButtonID)
{
	//const FString Command = "open " + MapNameEText->GetText().ToString();
	
	//GetOwningPlayer()->ConsoleCommand(Command, true);
}

void URZ_HomeWidget::OnJoinServerButtonPressed(uint8 ButtonID)
{
}

void URZ_HomeWidget::OnCreateServerButtonPressed(uint8 ButtonID)
{
}
