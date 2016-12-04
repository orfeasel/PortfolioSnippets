// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
#include "MainCharacterHUD.h"




void AMainCharacterHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!bShouldDrawHUD) return;
	
	const FVector2D Center(Canvas->ClipX*0.5f, Canvas->ClipY*0.5f);

	const FVector2D CrosshairDrawPosition(Center.X, Center.Y);

	if (!CrosshairToDraw)
	{
		CrosshairToDraw = CrosshairDefaultTexture;
	}

	FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairToDraw->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;

	TileItem.Size = TileItem.Size / 2;

	Canvas->DrawItem(TileItem);
}

void AMainCharacterHUD::SetCrosshairTexture(UTexture2D* TextureToSet)
{
	CrosshairToDraw = (TextureToSet) ? TextureToSet : CrosshairDefaultTexture;
}

void AMainCharacterHUD::DisableHUDDrawing()
{
	bShouldDrawHUD = false;
}
