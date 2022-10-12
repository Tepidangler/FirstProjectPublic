

#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMainPlayerController::DisplayHUDOverlay()
{
	if (HUDOverlay)
	{
		bHUDOverlayVisible = true;
		HUDOverlay->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::RemoveHUDOverlay()
{
}

void AMainPlayerController::DisplayEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = true;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
	
}

void AMainPlayerController::RemoveEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
	
}

void AMainPlayerController::DisplayLoadingScreen_Implementation()
{
	if (LoadingScreen)
	{
		bLoadingScreenVisible = true;
		LoadingScreen->SetVisibility(ESlateVisibility::Visible);

		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);
		bShowMouseCursor = false;

	}
}

void AMainPlayerController::RemoveLoadingScreen_Implementation()
{
	if (LoadingScreen)
	{
		bLoadingScreenVisible = false;
		LoadingScreen->SetVisibility(ESlateVisibility::Hidden);

		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);
		bShowMouseCursor = false;

	}
}

void AMainPlayerController::DisplayMainMenu_Implementation()
{
	if (MainMenu)
	{
		bMainMenuVisible = true;
		MainMenu->SetVisibility(ESlateVisibility::Visible);

		FInputModeUIOnly InputModeUIOnly;
		SetInputMode(InputModeUIOnly);
		bShowMouseCursor = true;
	}

}

void AMainPlayerController::RemoveMainMenu_Implementation()
{
	if (MainMenu)
	{
		bMainMenuVisible = false;
		MainMenu->SetVisibility(ESlateVisibility::Hidden);

		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);
		bShowMouseCursor = false;
	}
}

void AMainPlayerController::DisplayDeathOverlay_Implementation()
{
	if (DeathOverlay)
	{
		bDeathOverlayVisible = true;
		DeathOverlay->SetVisibility(ESlateVisibility::Visible);

		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);
		bShowMouseCursor = false;
	}
}

void AMainPlayerController::RemoveDeathOverlay_Implementation()
{
	if (DeathOverlay)
	{
		bDeathOverlayVisible = false;
		DeathOverlay->SetVisibility(ESlateVisibility::Hidden);

		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);
		bShowMouseCursor = false;
	}
}

void AMainPlayerController::DisplayOptionsMenu_Implementation()
{
	if (OptionsMenu)
	{
		bOptionsMenuVisible = true;
		OptionsMenu->SetVisibility(ESlateVisibility::Visible);

		FInputModeUIOnly InputModeUIOnly;
		SetInputMode(InputModeUIOnly);
		bShowMouseCursor = true;
	}
}

void AMainPlayerController::RemoveOptionsMenu_Implementation()
{
	if (OptionsMenu)
	{
		bOptionsMenuVisible = false;
		OptionsMenu->SetVisibility(ESlateVisibility::Hidden);

		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);
		bShowMouseCursor = false;
	}
}

void AMainPlayerController::DisplayLoadGameScreen_Implementation()
{
	if (LoadGameScreen)
	{
		bLoadGameScreenVisible = true;
		LoadGameScreen->SetVisibility(ESlateVisibility::Visible);

		FInputModeUIOnly InputModeUIOnly;
		SetInputMode(InputModeUIOnly);
		bShowMouseCursor = true;
	}
}

void AMainPlayerController::RemoveLoadGameScreen_Implementation()
{
	if (LoadGameScreen)
	{
		bLoadGameScreenVisible = false;
		LoadGameScreen->SetVisibility(ESlateVisibility::Hidden);

		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);
		bShowMouseCursor = false;
	}
}

void AMainPlayerController::DisplayPauseMenu_Implementation()
{
	if (PauseMenu)
	{
		bPauseMenuVisible = true;
		PauseMenu->SetVisibility(ESlateVisibility::Visible);

		FInputModeUIOnly InputModeUIOnly;
		SetInputMode(InputModeUIOnly);
		bShowMouseCursor = true;
	}
}

void AMainPlayerController::RemovePauseMenu_Implementation()
{
	if (PauseMenu)
	{
		bPauseMenuVisible = false;
		PauseMenu->SetVisibility(ESlateVisibility::Hidden);

		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);
		bShowMouseCursor = false;
	}

}

void AMainPlayerController::TogglePauseMenu()
{
	if (bPauseMenuVisible)
	{
		RemovePauseMenu();
	}
	else
	{
		DisplayPauseMenu();
	}
}

void AMainPlayerController::ToggleMainMenu()
{
	if (bMainMenuVisible)
	{
		RemoveMainMenu();
	}
	else
	{
		DisplayMainMenu();
	}
}

void AMainPlayerController::ToggleLoadGameScreen()
{
	if (bLoadGameScreenVisible)
	{
		RemoveLoadGameScreen();
	}
	else
	{
		DisplayLoadGameScreen();
	}
}

void AMainPlayerController::ToggleOptionsMenu()
{
	if (bOptionsMenuVisible)
	{
		RemoveOptionsMenu();
	}
	else
	{
		DisplayOptionsMenu();
	}
}

void AMainPlayerController::ToggleDeathOverlay()
{
	if (bDeathOverlayVisible)
	{
		RemoveDeathOverlay();
	}
	else
	{
		DisplayDeathOverlay();
	}
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}

	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Hidden);


	if (WEnemyHealthBar)
	{
		FVector2D Alignment(0.f, 0.f);
		EnemyHealthBar = CreateWidget<UUserWidget>(this, WEnemyHealthBar);
		if (EnemyHealthBar)
		{
			EnemyHealthBar->AddToViewport();
			EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
			
			EnemyHealthBar->SetAlignmentInViewport(Alignment);
		}

	}

	if (WPauseMenu)
	{
		FVector2D Alignment(0.f, 0.f);
		PauseMenu = CreateWidget<UUserWidget>(this, WPauseMenu);
		if (PauseMenu)
		{
			PauseMenu->AddToViewport();
			PauseMenu->SetVisibility(ESlateVisibility::Hidden);

			PauseMenu->SetAlignmentInViewport(Alignment);
		}

	}

	if (WMainMenu)
	{
		FString MapName = GetWorld()->GetMapName();
		MainMenu = CreateWidget<UUserWidget>(this, WMainMenu);
		if (MainMenu)
		{
			MainMenu->AddToViewport();
			MainMenu->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (WLoadGameScreen)
	{
		LoadGameScreen = CreateWidget<UUserWidget>(this, WLoadGameScreen);
		if (LoadGameScreen)
		{
			LoadGameScreen->AddToViewport();
			LoadGameScreen->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	if (WLoadingScreen)
	{
		LoadingScreen = CreateWidget<UUserWidget>(this, WLoadingScreen);
		if (LoadingScreen)
		{
			LoadingScreen->AddToViewport();
			LoadingScreen->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (WOptionsMenu)
	{
		OptionsMenu = CreateWidget<UUserWidget>(this, WOptionsMenu);
		if (OptionsMenu)
		{
			OptionsMenu->AddToViewport();
			OptionsMenu->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyHealthBar)
	{
		FVector2D PositionInViewport;
		ProjectWorldLocationToScreen(EnemyLocation, PositionInViewport);
		PositionInViewport.Y -= 50.f;
		PositionInViewport.X -= 35.f;

		FVector2D SizeInViewport = FVector2D(200.f, 25.f);

		EnemyHealthBar->SetPositionInViewport(PositionInViewport);
		EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewport);
		
	}
}
