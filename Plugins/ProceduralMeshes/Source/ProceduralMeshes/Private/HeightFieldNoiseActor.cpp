// Copyright Sigurdur Gunnarsson. All Rights Reserved. 
// Licensed under the MIT License. See LICENSE file in the project root for full license information. 
// Example heightfield generated with noise

#include "HeightFieldNoiseActor.h"
#include "Math/UnrealMathUtility.h"
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
	
	// TODO Remove on Next build: //const int32 NumberOfPoints = (LengthSections + 1) * (WidthSections + 1);
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
	// //TODO Remove Height values entirely.. Now replaced by Perlin3D
	// if (NumberOfPoints != HeightValues.Num())
	// {
	// 	HeightValues.Empty();
	// 	HeightValues.AddUninitialized(NumberOfPoints);
	// }
}

void AHeightFieldNoiseActor::GeneratePoints()
{
	RngStream.Initialize(RandomSeed);

	// TODO Remove on Next build
	// Setup example height data
	// const int32 NumberOfPoints = (LengthSections + 1) * (WidthSections + 1);

	// // Fill height data with random values
	// for (int32 i = 0; i < NumberOfPoints; i++)
	// {
	// 	HeightValues[i] = RngStream.FRandRange(0, HeightFactor);
	// }
}

void AHeightFieldNoiseActor::GenerateMesh()
{
	GetRuntimeMeshComponent()->Initialize(StaticProvider);
	StaticProvider->ClearSection(0, 0);
	
	if (Size.X < 1 || Size.Y < 1 || LengthSections < 1 || WidthSections < 1)
	{
		return;
	}


	// Generate six sides
	for(int32 i = 0; i < 6; i++)
	{
		SetupMeshBuffers();
		GeneratePoints();
		GenerateGrid(MinRadiusA, MinRadiusB, NoiseFrequencyA, AmplitudeA, NoiseFrequencyB, AmplitudeB, i, Positions, Triangles, Normals, Tangents, TexCoords, Size, LengthSections, WidthSections, HeightValues);
		const TArray<FColor> EmptyColors{};
		StaticProvider->CreateSectionFromComponents(0, i, 0, Positions, Triangles, Normals, TexCoords, EmptyColors, Tangents, ERuntimeMeshUpdateFrequency::Infrequent, false);
		StaticProvider->SetupMaterialSlot(0, TEXT("CylinderMaterial"), Material);
	}
	
}

void AHeightFieldNoiseActor::GenerateGrid(const bool MinRadiusA, const bool MinRadiusB, const float NoiseFrequencyA, const float AmplitudeA,const float NoiseFrequencyB, const float AmplitudeB, const int32 SectionIndex, TArray<FVector>& InVertices, TArray<int32>& InTriangles, TArray<FVector>& InNormals, TArray<FRuntimeMeshTangent>& InTangents, TArray<FVector2D>& InTexCoords, const FVector InSize, const int32 InLengthSections, const int32 InWidthSections, const TArray<float>& InHeightValues)
{
	// Note the coordinates are a bit weird here since I aligned it to the transform (X is forwards or "up", which Y is to the right)
	// Should really fix this up and use standard X, Y coords then transform into object space?
	

	 		//################################################################//
		//######################//VERTEX MACHINE--START//##########################//
	 //################################################################//
	//Size calc 
	FVector2D SectionSZ;
	switch (SectionIndex)
			{
			case 0:
			SectionSZ = FVector2D(InSize.X / InLengthSections, InSize.Y / InWidthSections);
			break;
			case 1:
			SectionSZ = FVector2D(InSize.X / InLengthSections, InSize.Z / InWidthSections);
			break;
			case 2:
			SectionSZ = FVector2D(InSize.X / InLengthSections, InSize.Y / InWidthSections);
			break;
			case 3:
			SectionSZ = FVector2D(InSize.X / InLengthSections, InSize.Z / InWidthSections);
			break;
			case 4:
			SectionSZ = FVector2D(InSize.Z / InLengthSections, InSize.Y / InWidthSections);
			break;
			case 5:
			SectionSZ = FVector2D(InSize.Z / InLengthSections, InSize.Y / InWidthSections);
			break;

			default:
			SectionSZ = FVector2D(InSize.X / InLengthSections, InSize.Y / InWidthSections);
			break;
			}
	const FVector2D SectionSize = SectionSZ;
	int32 VertexIndex = 0;
	int32 TriangleIndex = 0;
	float SRadius = InSize.X;

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
				SideOffset = FVector(-(InSize.X/2.f), -(InSize.Y/2.f), (InSize.Z/2.f));
				break;
			case 1:
				SideOffset = FVector(-(InSize.X/2.f), -(InSize.Y/2.f), -(InSize.Z/2.f));
				break;
			case 2:
				SideOffset = FVector(-(InSize.X/2.f), InSize.Y/2.f, -(InSize.Z/2.f));
				break;
			case 3:
				SideOffset = FVector(-(InSize.X/2.f), InSize.Y/2.f, InSize.Z/2.f);
				break;
			case 4:
				SideOffset = FVector((InSize.X/2.f), -(InSize.Y/2.f), (InSize.Z/2.f));
				SideRotationAxis = FVector(0.f, 1.f, 0.f); // y axis
				SideRotationAngle = 90.f;
				break;
			case 5:
				SideOffset = FVector(-(InSize.X/2.f), -(InSize.Y/2.f), -(InSize.Z/2.f));
				SideRotationAxis = FVector(0.f, 1.f, 0.f); // y axis
				SideRotationAngle = -90.f;
				break;
			default:
				
				break;
			}

			//Calculate relative vertex position from SectionSize & then Rotate
			FVector PBottomLeft = FVector(X * SectionSize.X , Y * SectionSize.Y , 0.f).RotateAngleAxis(SideRotationAngle, SideRotationAxis);
			FVector PBottomRight = FVector(X * SectionSize.X , (Y+1) * SectionSize.Y , 0.f).RotateAngleAxis(SideRotationAngle, SideRotationAxis);
			FVector PTopRight = FVector((X + 1) * SectionSize.X , (Y + 1) * SectionSize.Y , 0.f).RotateAngleAxis(SideRotationAngle, SideRotationAxis);
			FVector PTopLeft = FVector((X+1) * SectionSize.X , Y * SectionSize.Y , 0.f).RotateAngleAxis(SideRotationAngle, SideRotationAxis);
			//Normalize 
			FVector NBottomLeft = FVector(PBottomLeft.X + SideOffset.X, PBottomLeft.Y + SideOffset.Y, PBottomLeft.Z + SideOffset.Z).GetSafeNormal();
			FVector NBottomRight = FVector(PBottomRight.X + SideOffset.X, PBottomRight.Y + SideOffset.Y, PBottomRight.Z + SideOffset.Z).GetSafeNormal();
			FVector NTopRight = FVector(PTopRight.X + SideOffset.X, PTopRight.Y + SideOffset.Y, PTopRight.Z + SideOffset.Z).GetSafeNormal();
			FVector NTopLeft = FVector(PTopLeft.X + SideOffset.X, PTopLeft.Y + SideOffset.Y, PTopLeft.Z + SideOffset.Z).GetSafeNormal();

			//##################################################################################################Noise Buffer Here#####################
			//.............Position...=..Position..X..Radius.. + .................................NOISE LAYER1 ...............................................Minimum Radius Clamp Check.....+......... NOISE LAYER2 .........................
			const FVector FBottomLeft = NBottomLeft * SRadius + NBottomLeft * AmplitudeA * FMath::Clamp(FMath::PerlinNoise3D(NBottomLeft * NoiseFrequencyA), (MinRadiusA)? 0.f : -1.f, 1.f) + NBottomLeft * AmplitudeB * FMath::Clamp(FMath::PerlinNoise3D(NBottomLeft * NoiseFrequencyB), (MinRadiusB)? 0.f : -1.f, 1.f) * FMath::Clamp(FMath::PerlinNoise3D(NBottomLeft * NoiseFrequencyA), (MinRadiusA)? 0.f : -1.f, 1.f);
			const FVector FBottomRight = NBottomRight * SRadius + NBottomRight * AmplitudeA * FMath::Clamp(FMath::PerlinNoise3D(NBottomRight * NoiseFrequencyA), (MinRadiusA)? 0.f : -1.f, 1.f) + NBottomRight * AmplitudeB * FMath::Clamp(FMath::PerlinNoise3D(NBottomRight * NoiseFrequencyB), (MinRadiusB)? 0.f : -1.f, 1.f) * FMath::Clamp(FMath::PerlinNoise3D(NBottomRight * NoiseFrequencyA), (MinRadiusA)? 0.f : -1.f, 1.f);
			const FVector FTopRight = NTopRight * SRadius + NTopRight * AmplitudeA * FMath::Clamp(FMath::PerlinNoise3D(NTopRight * NoiseFrequencyA), (MinRadiusA)? 0.f : -1.f, 1.f) + NTopRight * AmplitudeB * FMath::Clamp(FMath::PerlinNoise3D(NTopRight * NoiseFrequencyB), (MinRadiusB)? 0.f : -1.f, 1.f) * FMath::Clamp(FMath::PerlinNoise3D(NTopRight * NoiseFrequencyA), (MinRadiusA)? 0.f : -1.f, 1.f);
			const FVector FTopLeft = NTopLeft * SRadius + NTopLeft * AmplitudeA * FMath::Clamp(FMath::PerlinNoise3D(NTopLeft * NoiseFrequencyA), (MinRadiusA)? 0.f : -1.f, 1.f) + NTopLeft * AmplitudeB * FMath::Clamp(FMath::PerlinNoise3D(NTopLeft * NoiseFrequencyB), (MinRadiusB)? 0.f : -1.f, 1.f) * FMath::Clamp(FMath::PerlinNoise3D(NTopLeft * NoiseFrequencyA), (MinRadiusA)? 0.f : -1.f, 1.f);
			
			//Pass the vertex to vertex buffer
			InVertices[BottomLeftIndex] = FBottomLeft;
			InVertices[BottomRightIndex] = FBottomRight;
			InVertices[TopRightIndex] = FTopRight;
			InVertices[TopLeftIndex] = FTopLeft;

 					//################################################################//
				//##################//VERTEX MACHINE--END//##########################//
			//################################################################//
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
