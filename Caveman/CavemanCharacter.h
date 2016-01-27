//This is an incomplete header file. It contains limited feautures just for demonstration purposes
UCLASS()
class CAVEMANTESTFIELD_API AMyCharacter_FirstTry : public ACharacter
{
	GENERATED_BODY()
private:

	//If caveman is on trap mode movement should be disabled in order to place trap first
	bool bIsInTrapMode = false;
	//Move the trap on the ground (used only while "InTrapMode")
	void SetNewTrapLocation();
	//Reference for the trap we are trying to place
	ATrap* TrapToBePlaced;

public:
	/*Used in order to Place Traps*/
	UFUNCTION(BlueprintCallable, Category = "Trap")
	void PlaceTrap();

	/*Blueprint of trap to spawn*/
	UPROPERTY(EditDefaultsOnly, Category = "Trap")
	TSubclassOf<ATrap> TrapToSpawn;

	/*Max Spawn distance from caveman*/
	UPROPERTY(EditDefaultsOnly, Category = "Trap")
	float SpawnDistance;
}