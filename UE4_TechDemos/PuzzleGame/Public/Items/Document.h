// Orfeas Eleftheriou 2016

#pragma once

#include "Items/BaseItem.h"
#include "Document.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNA_API ADocument : public ABaseItem
{
	GENERATED_BODY()
		
protected:

	/** The document's texture that is shown in the player's screen
	when the player is reading the current document*/
	UPROPERTY(EditAnywhere, Category = Document)
	UTexture2D* DocumentTexture;

public:

	virtual bool Interact(UBackpackComponent& BackpackComponent) override;

	/** Gets the document's texture
	* @return the document's texture
	*/
	FORCEINLINE UTexture2D* GetDocumentTexture() { return DocumentTexture; }

	
};
