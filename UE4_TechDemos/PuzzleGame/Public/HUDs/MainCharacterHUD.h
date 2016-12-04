// Orfeas Eleftheriou 2016

#pragma once

#include "GameFramework/HUD.h"
#include "MainCharacterHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNA_API AMainCharacterHUD : public AHUD
{
	GENERATED_BODY()

private:

	/** Holds a reference to the texture that the HUD is currently drawing */
	UTexture2D* CrosshairToDraw = CrosshairDefaultTexture;
	
	bool bShouldDrawHUD = true;

protected:

	/** The default crosshair texture */
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* CrosshairDefaultTexture;


public:

	virtual void DrawHUD() override;

	/** Sets a new crosshair texture for the HUD to display in the next Draw call */
	void SetCrosshairTexture(UTexture2D* TextureToSet);
	
	void DisableHUDDrawing();
};
