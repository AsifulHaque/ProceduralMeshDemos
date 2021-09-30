// Copyright Sigurdur Gunnarsson. All Rights Reserved. 
// Licensed under the MIT License. See LICENSE file in the project root for full license information. 
// Example heightfield generated with noise

#pragma once

#include "CoreMinimal.h"
#include "RuntimeMeshActor.h"
#include "HeightFieldNoiseActor.generated.h"

UCLASS()
class PROCEDURALMESHES_API AHeightFieldNoiseActor : public ARuntimeMeshActor
{
	GENERATED_BODY()

public:
	AHeightFieldNoiseActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
	FVector Size = FVector(1000.0f, 1000.0f, 1000.0f); // Size.X is Radius of the sphere

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
	int32 LengthSections = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
	int32 WidthSections = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters | Noise Layer A")
	float HeightFactorA = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters | Noise Layer A")
	float NoiseInScaleA = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters | Noise Layer A")
	bool MinRadiusA = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters | Noise Layer B")
	float HeightFactorB = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters | Noise Layer B")
	float NoiseInScaleB = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters | Noise Layer B")
	bool MinRadiusB = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
	int32 RandomSeed = 1238;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
	UMaterialInterface* Material;

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostLoad() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient)
	URuntimeMeshProviderStatic* StaticProvider;

private:
	void GenerateMesh();
	void GeneratePoints();
	static void GenerateGrid(const bool MinRadiusA, const bool MinRadiusB,  const float NoiseInScaleA, const float HeightFactorA,const float NoiseInScaleB, const float HeightFactorB, const int32 SectionIndex, TArray<FVector>& InVertices, TArray<int32>& InTriangles, TArray<FVector>& InNormals, TArray<FRuntimeMeshTangent>& InTangents, TArray<FVector2D>& InTexCoords, const FVector InSize, const int32 InLengthSections, const int32 InWidthSections, const TArray<float>& InHeightValues);

	UPROPERTY(Transient)
	FRandomStream RngStream;

	UPROPERTY(Transient)
	TArray<float> HeightValues;

	// Mesh buffers
	void SetupMeshBuffers();
	UPROPERTY(Transient)
	TArray<FVector> Positions;
	UPROPERTY(Transient)
	TArray<int32> Triangles;
	UPROPERTY(Transient)
	TArray<FVector> Normals;
	UPROPERTY(Transient)
	TArray<FRuntimeMeshTangent> Tangents;
	UPROPERTY(Transient)
	TArray<FVector2D> TexCoords;
};
