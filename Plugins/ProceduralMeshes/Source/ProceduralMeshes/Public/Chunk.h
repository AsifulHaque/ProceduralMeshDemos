// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RuntimeMeshActor.h"
#include "Chunk.generated.h"

/**
 * 
 */
UCLASS()
class PROCEDURALMESHES_API AChunk : public ARuntimeMeshActor
{
	GENERATED_BODY()

public:
	AChunk();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
	FVector Size = FVector(1000.0f, 1000.0f, 1000.0f); // Size.X is Radius of the sphere

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
	int32 LengthSections = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
	int32 WidthSections = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
	int32 HeightSections = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
	float SurfaceValue = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
	bool InterpolateSurface = true;
	
	// Offset to the noise sample
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters | Noise Octaves")
	FVector NoiseOffset = FVector(0.0f, 0.0f, 0.0f);
	// Number of Noise Octaves to apply
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters | Noise Octaves")
	int32 NumberOfOctaves = 2;
	// Frequency for the first Octave
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters | Noise Octaves")
	float OctaveInitialFrequency = 1.f;
	// Exponential Rate of increase of Frequency; Controls the Frequency exponentially, Frequency == Intial Frequency, when Lacunariy = 1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters | Noise Octaves")
	float Lacunarity = 1.f; 
	// Height for the first Octave
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters | Noise Octaves")
	float NoiseIntialHeight = 100.f; 
	// Decreases the Amplitude gradually with each octave
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters | Noise Octaves")
	float Persistance = 1.f; 
	// Clamp everything to a minimum Radius
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters | Noise Octaves")
	bool MinRadius = true; 

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
	// int32 RandomSeed = 1238;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
	UMaterialInterface* Material;

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostLoad() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient)
	URuntimeMeshProviderStatic* StaticProvider;
	//Calculate Surface Vertex Location from Triangulation Table
	static FVector	GetLocationOfTrigoPoint(const int PointIndex,const TArray<FVector> Corners,const TArray<float> VertexValue, const float SurfaceValue, const bool InterpolateSurface);

private:
	void GenerateMesh();
	static void GenerateGrid(const bool InterpolateSurface, const float SurfaceValue, const FVector NoiseOffset, const float OctaveInitialFrequency, const float NoiseIntialHeight, const int32 NumberOfOctaves, const float Lacunarity, const float Persistance, const bool MinRadius, const int32 SectionIndex, TArray<FVector>& InVertices, TArray<int32>& InTriangles, TArray<FVector>& InNormals, TArray<FRuntimeMeshTangent>& InTangents, TArray<FVector2D>& InTexCoords, const FVector InSize, const int32 InLengthSections, const int32 InWidthSections, const int32 InHeightSections);

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

	const static int	TrigoTable[256][16];
};
