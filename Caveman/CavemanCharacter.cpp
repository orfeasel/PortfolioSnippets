//This is an incomplete source file. It contains limited feautures just for demonstration purposes
void AMyCharacter_FirstTry::PlaceTrap()
{
	if (bIsInTrapMode)
	{
		bIsInTrapMode = false;
		//GLog->Log("Placed Trap");
		//Disable reference to the newly placed trap.
		TrapToBePlaced = nullptr;
	}
	else
	{
		bIsInTrapMode = true;
		//GLog->Log("Spawned trap");
	
		FVector SpawnLocation = GetActorLocation() + (GetActorForwardVector() * SpawnDistance);
		TrapToBePlaced = GetWorld()->SpawnActor<ATrap>(TrapToSpawn, SpawnLocation, FRotator(0));
		SetNewTrapLocation();
	}

}

void AMyCharacter_FirstTry::SetNewTrapLocation()
{
	if (TrapToBePlaced)
	{
		FVector SpawnLocation = GetActorLocation() + (GetActorForwardVector() * SpawnDistance);
		SpawnLocation.Y -= TrapToBePlaced->TrapCollider->GetUnscaledBoxExtent().Y;

		FVector EndLocation = SpawnLocation;
		//We want a vertical vector in order to raycast on landscape
		EndLocation.Z = 0;
		
		FHitResult Hit;
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(TrapToBePlaced);
		//Sweep in order to find the landscape impact point (the trap must always be placed into the ground)
		GetWorld()->SweepSingleByChannel(Hit, SpawnLocation, EndLocation, FQuat::Identity, ECollisionChannel::ECC_WorldStatic, FCollisionShape::FCollisionShape(), CollisionQueryParams);
		
		TrapToBePlaced->SetActorLocation(Hit.ImpactPoint);
	}
}