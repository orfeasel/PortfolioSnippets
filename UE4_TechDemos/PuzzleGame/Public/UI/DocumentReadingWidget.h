// Orfeas Eleftheriou 2016

#pragma once

#include "Blueprint/UserWidget.h"
#include "DocumentReadingWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNA_API UDocumentReadingWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	/** The texture that the UMG image binds to */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTexture2D* DocumentTexture;

public:

	/** Sets a new texture resource for this widget
	* @param Texture - the new document texture
	*/
	inline void SetImageTexture(UTexture2D* Texture)
	{
		DocumentTexture = Texture;
	}
};
