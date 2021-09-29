// Copyright Sigurdur Gunnarsson. All Rights Reserved. 
// Licensed under the MIT License. See LICENSE file in the project root for full license information. 
// Example heightfield generated with noise

#include "HeightFieldNoiseActor.h"
#include "Providers/RuntimeMeshProviderStatic.h"

AHeightFieldNoiseActor::AHeightFieldNoiseActor()
{
	PrimaryActorTick.bCanEverTick = false;
	StaticProvider = CreateDefaultSubobject<URuntimeMeshProviderStatic>(TEXT("RuntimeMeshProvider-Static"));
	StaticProvider->SetShouldSerializeMeshData(false);
}

void AHeightFieldNoiseActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	GenerateMesh();
}

// This is called when actor is already in level and map is opened
void AHeightFieldNoiseActor::PostLoad()
{
	Super::PostLoad();
	GenerateMesh();
}

void AHeightFieldNoiseActor::SetupMeshBuffers()
{
	const int32 NumberOfSides = 4; // create 4 mesh section TODO remove if not needed
	const int32 NumberOfPoints = (LengthSections + 1) * (WidthSections + 1);
	const int32 VertexCount = LengthSections * WidthSections * 4; // 4x vertices per quad/section
	const int32 TriangleCount = LengthSections * WidthSections * 2 * 3; // 2x3 vertex indexes per quad

	if (VertexCount != Positions.Num())
	{
		Positions.Empty();
		Positions.AddUninitialized(VertexCount);
		Normals.Empty();
		Normals.AddUninitialized(VertexCount);
		Tangents.Empty();
		Tangents.AddUninitialized(VertexCount);
		TexCoords.Empty();
		TexCoords.AddUninitialized(VertexCount);
	}

	if (TriangleCount != Triangles.Num())
	{
		Triangles.Empty();
		Triangles.AddUninitialized(TriangleCount);
	}

	if (NumberOfPoints != HeightValues.Num())
	{
		HeightValues.Empty();
		HeightValues.AddUninitialized(NumberOfPoints);
	}
}

void AHeightFieldNoiseActor::GeneratePoints()
{
	RngStream.Initialize(RandomSeed);

	// Setup example height data
	const int32 NumberOfPoints = (LengthSections + 1) * (WidthSections + 1);

	// Fill height data with random values
	for (int32 i = 0; i < NumberOfPoints; i++)
	{
		HeightValues[i] = RngStream.FRandRange(0, Size.Z);
	}
}

void AHeightFieldNoiseActor::GenerateMesh()
{
	GetRuntimeMeshComponent()->Initialize(StaticProvider);
	StaticProvider->ClearSection(0, 0);
	
	if (Size.X < 1 || Size.Y < 1 || LengthSections < 1 || WidthSections < 1)
	{
		return;
	}

	// SetupMeshBuffers();
	// GeneratePoints();
	// Generate six sides
	for(int32 i = 0; i < 6; i++)
	{
		SetupMeshBuffers();
		GeneratePoints();
		GenerateGrid(i, Positions, Triangles, Normals, Tangents, TexCoords, FVector2D(Size.X, Size.Y), LengthSections, WidthSections, HeightValues);
		const TArray<FColor> EmptyColors{};
		StaticProvider->CreateSectionFromComponents(0, i, 0, Positions, Triangles, Normals, TexCoords, EmptyColors, Tangents, ERuntimeMeshUpdateFrequency::Infrequent, false);
		StaticProvider->SetupMaterialSlot(0, TEXT("CylinderMaterial"), Material);
	}
	//GenerateGrid(Positions, Triangles, Normals, Tangents, TexCoords, FVector2D(Size.X, Size.Y), LengthSections, WidthSections, HeightValues);

	// const TArray<FColor> EmptyColors{};
	// StaticProvider->CreateSectionFromComponents(0, 0, 0, Positions, Triangles, Normals, TexCoords, EmptyColors, Tangents, ERuntimeMeshUpdateFrequency::Infrequent, false);
	// StaticProvider->SetupMaterialSlot(0, TEXT("CylinderMaterial"), Material);
}

void AHeightFieldNoiseActor::GenerateGrid(const int32 SectionIndex, TArray<FVector>& InVertices, TArray<int32>& InTriangles, TArray<FVector>& InNormals, TArray<FRuntimeMeshTangent>& InTangents, TArray<FVector2D>& InTexCoords, const FVector2D InSize, const int32 InLengthSections, const int32 InWidthSections, const TArray<float>& InHeightValues)
{
	// Note the coordinates are a bit weird here since I aligned it to the transform (X is forwards or "up", which Y is to the right)
	// Should really fix this up and use standard X, Y coords then transform into object space?
	const FVector2D SectionSize = FVector2D(InSize.X / InLengthSections, InSize.Y / InWidthSections);
	int32 VertexIndex = 0;
	int32 TriangleIndex = 0;

	for (int32 X = 0; X < InLengthSections; X++)
	{
		for (int32 Y = 0; Y < InWidthSections; Y++)
		{
			// Setup a quad
			const int32 BottomLeftIndex = VertexIndex++;
			const int32 BottomRightIndex = VertexIndex++;
			const int32 TopRightIndex = VertexIndex++;
			const int32 TopLeftIndex = VertexIndex++;

			const int32 NoiseIndex_BottomLeft = (X * InWidthSections) + Y;
			const int32 NoiseIndex_BottomRight = NoiseIndex_BottomLeft + 1;
			const int32 NoiseIndex_TopLeft = ((X+1) * InWidthSections) + Y;
			const int32 NoiseIndex_TopRight = NoiseIndex_TopLeft + 1;

			//Determine the section offset for each of the 6 sides
			FVector SideOffset = FVector(0.f, 0.f, 0.f);
			FVector SideRotationAxis = FVector(1.f, 0.f, 0.f); // X axis
			float SideRotationAngle = SectionIndex * 90.f;
			switch (SectionIndex)
			{
			case 0:
				SideOffset = FVector(-(InSize.X/2.f), -(InSize.X/2.f), (InSize.X/2.f));
				break;
			case 1:
				SideOffset = FVector(-(InSize.X/2.f), -(InSize.X/2.f), -(InSize.X/2.f));
				break;
			case 2:
				SideOffset = FVector(-(InSize.X/2.f), InSize.X/2.f, -(InSize.X/2.f));
				break;
			case 3:
				SideOffset = FVector(-(InSize.X/2.f), InSize.X/2.f, InSize.X/2.f);
				break;
			case 4:
				SideOffset = FVector((InSize.X/2.f), -(InSize.X/2.f), (InSize.X/2.f));
				SideRotationAxis = FVector(0.f, 1.f, 0.f); // y axis
				SideRotationAngle = 90.f;
				break;
			case 5:
				SideOffset = FVector(-(InSize.X/2.f), -(InSize.X/2.f), -(InSize.X/2.f));
				SideRotationAxis = FVector(0.f, 1.f, 0.f); // y axis
				SideRotationAngle = -90.f;
				break;
			default:
				
				break;
			}

			const FVector PBottomLeft = FVector(X * SectionSize.X , Y * SectionSize.Y , InHeightValues[NoiseIndex_BottomLeft]).RotateAngleAxis(SideRotationAngle, SideRotationAxis);
			const FVector PBottomRight = FVector(X * SectionSize.X , (Y+1) * SectionSize.Y , InHeightValues[NoiseIndex_BottomRight]).RotateAngleAxis(SideRotationAngle, SideRotationAxis);
			const FVector PTopRight = FVector((X + 1) * SectionSize.X , (Y + 1) * SectionSize.Y , InHeightValues[NoiseIndex_TopRight]).RotateAngleAxis(SideRotationAngle, SideRotationAxis);
			const FVector PTopLeft = FVector((X+1) * SectionSize.X , Y * SectionSize.Y , InHeightValues[NoiseIndex_TopLeft]).RotateAngleAxis(SideRotationAngle, SideRotationAxis);
			
			InVertices[BottomLeftIndex] = FVector(PBottomLeft.X + SideOffset.X, PBottomLeft.Y + SideOffset.Y, PBottomLeft.Z + SideOffset.Z);
			InVertices[BottomRightIndex] = FVector(PBottomRight.X + SideOffset.X, PBottomRight.Y + SideOffset.Y, PBottomRight.Z + SideOffset.Z);
			InVertices[TopRightIndex] =FVector(PTopRight.X + SideOffset.X, PTopRight.Y + SideOffset.Y, PTopRight.Z + SideOffset.Z); 
			InVertices[TopLeftIndex] = FVector(PTopLeft.X + SideOffset.X, PTopLeft.Y + SideOffset.Y, PTopLeft.Z + SideOffset.Z);

			// Note that Unreal UV origin (0,0) is top left
			InTexCoords[BottomLeftIndex] = FVector2D(static_cast<float>(X) / static_cast<float>(InLengthSections), static_cast<float>(Y) / static_cast<float>(InWidthSections));
			InTexCoords[BottomRightIndex] = FVector2D(static_cast<float>(X) / static_cast<float>(InLengthSections), static_cast<float>(Y + 1) / static_cast<float>(InWidthSections));
			InTexCoords[TopRightIndex] = FVector2D(static_cast<float>(X + 1) / static_cast<float>(InLengthSections), static_cast<float>(Y + 1) / static_cast<float>(InWidthSections));
			InTexCoords[TopLeftIndex] = FVector2D(static_cast<float>(X + 1) / static_cast<float>(InLengthSections), static_cast<float>(Y) / static_cast<float>(InWidthSections));

			// Now create two triangles from those four vertices
			// The order of these (clockwise/counter-clockwise) dictates which way the normal will face. 
			InTriangles[TriangleIndex++] = BottomLeftIndex;
			InTriangles[TriangleIndex++] = TopRightIndex;
			InTriangles[TriangleIndex++] = TopLeftIndex;

			InTriangles[TriangleIndex++] = BottomLeftIndex;
			InTriangles[TriangleIndex++] = BottomRightIndex;
			InTriangles[TriangleIndex++] = TopRightIndex;

			// Normals
			const FVector NormalCurrent = FVector::CrossProduct(InVertices[BottomLeftIndex] - InVertices[TopLeftIndex], InVertices[TopLeftIndex] - InVertices[TopRightIndex]).GetSafeNormal();

			// If not smoothing we just set the vertex normal to the same normal as the polygon they belong to
			InNormals[BottomLeftIndex] = InNormals[BottomRightIndex] = InNormals[TopRightIndex] = InNormals[TopLeftIndex] = NormalCurrent;

			// Tangents (perpendicular to the surface)
			const FVector SurfaceTangent = (PBottomLeft - PBottomRight).GetSafeNormal();
			InTangents[BottomLeftIndex] = InTangents[BottomRightIndex] = InTangents[TopRightIndex] = InTangents[TopLeftIndex] = SurfaceTangent;
		}
	}
}
