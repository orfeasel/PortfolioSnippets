//This is an incomplete header file. It contains limited feautures just for demonstration purposes
UCLASS()
class CAVEMANTESTFIELD_API ACavemanController : public APlayerController
{
	GENERATED_BODY()

private:
	void DetermineNewTemperatureLimits(float& UpperLimit, float& FloorLimit, float CurrentTemperature, float MaxTemp, float MinTemp);

public:

	/*Min temperature between 06:00-18:00*/
	UPROPERTY(EditAnywhere, Category = "Temperature")
	float MinDayTemperature = 20;

	/*Max temperature between 06:00-18:00*/
	UPROPERTY(EditAnywhere, Category = "Temperature")
	float MaxDayTemperature = 30;

	/*Min temperature between 18:00-05:00*/
	UPROPERTY(EditAnywhere, Category = "Temperature")
	float MinNightTemperature = 5;

	/*Max temperature between 18:00-05:00*/
	UPROPERTY(EditAnywhere, Category = "Temperature")
	float MaxNightTemperature = 15;

	/*Clamp value for temperature (ie a value of 2 means that the temp will rise or fall max 2 degrees*/
	UPROPERTY(EditDefaultsOnly, Category = "Temperature")
	float ClampValue=2;

	/*Decides the temperature based on max and mins temperatures given*/
	UFUNCTION(BlueprintCallable, Category = "Temperature")
	void SetNewTemperature();
}