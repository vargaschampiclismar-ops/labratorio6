// Copyright Epic Games, Inc. All Rights Reserved.

#include "AventuraUSFX022026L1GameMode.h"
#include "AventuraUSFX022026L1Pawn.h"
#include "Plataforma.h"
#include "PlataformaAerea.h"
#include "PlataformaTerrestre.h"
#include "PlataformaSubterranea.h"
#include "PlataformaAcuatica.h"
#include "Inventory.h"
#include "FabricaPlataformas.h"

AAventuraUSFX022026L1GameMode::AAventuraUSFX022026L1GameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = AAventuraUSFX022026L1Pawn::StaticClass();

	tipoPlataformaActual = 0;
}

void AAventuraUSFX022026L1GameMode::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	if (World == nullptr)
	{
		return;
	}

	AFabricaPlataformas* FabricaPlataformas = World->SpawnActor<AFabricaPlataformas>(AFabricaPlataformas::StaticClass());
	if (FabricaPlataformas)
	{
		FRotator Rotacion = FRotator::ZeroRotator;
		FVector SpawnLocation = FVector::ZeroVector;

		for (int i = 0; i < 10; i++)
		{
			// Generar una posición aleatoria dentro del nivel
			SpawnLocation = FVector(FMath::RandRange(-1500.0f, 1500.0f), FMath::RandRange(-1500.0f, 1500.0f), FMath::RandRange(50.0f, 500.0f));

			// Definir una clave única numérica para el TMap (ej. 100 en adelante)
			int32 ClaveUnica = 100 + i;

			// Llamar al Factory Method de tu fábrica (Nota: usa "Area" o "Acuatica" o "Terrestre" según tu configuración)[cite: 37]
			APlataforma* NuevaPlataforma = FabricaPlataformas->CrearPlataforma("Area", SpawnLocation, Rotacion);

				if (NuevaPlataforma)
				{
					NuevaPlataforma->NumeroComponente = ClaveUnica;

					// Almacenar en el TMap con clave única
					MapaPlataformas.Add(ClaveUnica, NuevaPlataforma);
					aPlataformas.Add(NuevaPlataforma);
				}
		}
	}

	// Temporizador para eliminar plataformas (si lo tienes activo)
	GetWorldTimerManager().SetTimer(TimerEliminarPlataforma, this, &AAventuraUSFX022026L1GameMode::EliminarPlataforma, 1.0f, true);

}

void AAventuraUSFX022026L1GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAventuraUSFX022026L1GameMode::EliminarPlataforma()
{

	for (int32 i = 0; i < aPlataformas.Num(); i++)
	{
		if (aPlataformas[i] != nullptr)
		{
			// Verifica si la plataforma es de tipo ATerrestre
			APlataforma * plataformaActual = aPlataformas[i];

			if (plataformaActual != nullptr)
			{
				if (plataformaActual->tipoPlataforma == static_cast<ETipoPlataforma>(tipoPlataformaActual))
				{
					// Eliminamos el objeto del escenario
					plataformaActual->Destroy();

					// Lo eliminamos también del TArray
					aPlataformas.RemoveAt(i);

					// Salimos del bucle después de eliminar el primero
					break;
				}
			}
		}
	}
	if (tipoPlataformaActual >= 3)
	{
		tipoPlataformaActual = 0;
	}
	else
		tipoPlataformaActual++;

}

void AAventuraUSFX022026L1GameMode::ControlarMovimientoGrupo(bool bActivar)
{
	for (TPair<int32, APlataforma*>& Elemento : MapaPlataformas)
	{
		APlataforma* PlataformaActual = Elemento.Value;
		if (PlataformaActual)
		{
			PlataformaActual->bMovimientoActivo = bActivar;
		}
	}
}

