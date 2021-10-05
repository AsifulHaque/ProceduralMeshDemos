// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"
#include "Math/UnrealMathUtility.h"
#include "Providers/RuntimeMeshProviderStatic.h"

const int AChunk::TrigoTable[256][16] = {
		{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
		{3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
		{3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
		{3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
		{9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
		{1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
		{9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
		{2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
		{8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
		{9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
		{4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
		{3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
		{1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
		{4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
		{4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
		{9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
		{1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
		{5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
		{2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
		{9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
		{0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
		{2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
		{10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
		{4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
		{5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
		{5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
		{9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
		{0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
		{1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
		{10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
		{8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
		{2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
		{7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
		{9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
		{2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
		{11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
		{9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
		{5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
		{11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
		{11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
		{1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
		{9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
		{5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
		{2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
		{0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
		{5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
		{6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
		{0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
		{3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
		{6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
		{5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
		{1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
		{10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
		{6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
		{1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
		{8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
		{7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
		{3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
		{5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
		{0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
		{9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
		{8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
		{5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
		{0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
		{6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
		{10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
		{10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
		{8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
		{1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
		{3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
		{0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
		{10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
		{0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
		{3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
		{6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
		{9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
		{8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
		{3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
		{6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
		{0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
		{10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
		{10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
		{1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
		{2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
		{7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
		{7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
		{2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
		{1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
		{11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
		{8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
		{0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
		{7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
		{10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
		{2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
		{6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
		{7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
		{2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
		{1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
		{10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
		{10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
		{0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
		{7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
		{6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
		{8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
		{9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
		{6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
		{1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
		{4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
		{10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
		{8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
		{0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
		{1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
		{8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
		{10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
		{4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
		{10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
		{5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
		{11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
		{9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
		{6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
		{7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
		{3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
		{7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
		{9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
		{3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
		{6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
		{9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
		{1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
		{4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
		{7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
		{6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
		{3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
		{0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
		{6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
		{1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
		{0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
		{11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
		{6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
		{5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
		{9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
		{1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
		{1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
		{10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
		{0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
		{5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
		{10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
		{11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
		{0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
		{9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
		{7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
		{2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
		{8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
		{9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
		{9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
		{1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
		{9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
		{9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
		{5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
		{0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
		{10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
		{2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
		{0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
		{0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
		{9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
		{5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
		{3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
		{5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
		{8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
		{0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
		{9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
		{0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
		{1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
		{3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
		{4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
		{9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
		{11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
		{11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
		{2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
		{9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
		{3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
		{1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
		{4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
		{4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
		{0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
		{3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
		{3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
		{0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
		{9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
		{1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
		{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
};

AChunk::AChunk()
{
	PrimaryActorTick.bCanEverTick = false;
	StaticProvider = CreateDefaultSubobject<URuntimeMeshProviderStatic>(TEXT("RuntimeMeshProvider-Static"));
	StaticProvider->SetShouldSerializeMeshData(false);
}

void AChunk::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	GenerateMesh();
}

// This is called when actor is already in level and map is opened
void AChunk::PostLoad()
{
	Super::PostLoad();
	GenerateMesh();
}

void AChunk::SetupMeshBuffers()
{
	
	// TODO Remove on Next build: //const int32 NumberOfPoints = (LengthSections + 1) * (WidthSections + 1);
	const int32 VertexCount = LengthSections * WidthSections * 4; // 4x vertices per quad/section
	const int32 TriangleCount = LengthSections * WidthSections * 2 * 3; // 2x3 vertex indexes per quad

	// if (VertexCount != Positions.Num())
	// {
		Positions.Empty();
		//Positions.AddUninitialized(VertexCount);
		Normals.Empty();
		//Normals.AddUninitialized(VertexCount);
		Tangents.Empty();
		//Tangents.AddUninitialized(VertexCount);
		TexCoords.Empty();
		//TexCoords.AddUninitialized(VertexCount);
	//}

	// if (TriangleCount != Triangles.Num())
	// {
		Triangles.Empty();
		//Triangles.AddUninitialized(TriangleCount);
	// }
	
}


void AChunk::GenerateMesh()
{
	GetRuntimeMeshComponent()->Initialize(StaticProvider);
	StaticProvider->ClearSection(0, 0);
	
	if (Size.X < 1 || Size.Y < 1 || LengthSections < 1 || WidthSections < 1)
	{
		return;
	}

	SetupMeshBuffers();
	GenerateGrid(InterpolateSurface, SurfaceValue, NoiseOffset, OctaveInitialFrequency, NoiseIntialHeight, NumberOfOctaves, Lacunarity, Persistance, MinRadius, 0, Positions, Triangles, Normals, Tangents, TexCoords, Size, LengthSections, WidthSections, HeightSections);
	const TArray<FColor> EmptyColors{};
	StaticProvider->CreateSectionFromComponents(0, 0, 0, Positions, Triangles, Normals, TexCoords, EmptyColors, Tangents, ERuntimeMeshUpdateFrequency::Infrequent, false);
	StaticProvider->SetupMaterialSlot(0, TEXT("CylinderMaterial"), Material);	
}

void AChunk::GenerateGrid(const bool InterpolateSurface, const float SurfaceValue, const FVector NoiseOffset, const float OctaveInitialFrequency, const float NoiseIntialHeight, const int32 NumberOfOctaves, const float Lacunarity, const float Persistance, const bool MinRadius, const int32 SectionIndex, TArray<FVector>& InVertices, TArray<int32>& InTriangles, TArray<FVector>& InNormals, TArray<FRuntimeMeshTangent>& InTangents, TArray<FVector2D>& InTexCoords, const FVector InSize, const int32 InLengthSections, const int32 InWidthSections, const int32 InHeightSections)
{
	// Note the coordinates are a bit weird here since I aligned it to the transform (X is forwards or "up", which Y is to the right)
	// Should really fix this up and use standard X, Y coords then transform into object space?
	

	 		//################################################################//
		//######################//VERTEX MACHINE--START//##########################//
	 //################################################################//
	//Size calc 
	const FVector SectionSize = FVector(InSize.X / InLengthSections, InSize.Y / InWidthSections, InSize.Z / InHeightSections);
	int32 VertexIndex = 0;
	int32 TriangleIndex = 0;
	float SRadius = InSize.X;

	//march through cubes~~
	for (int32 X = 0; X < InLengthSections; X++)
	{
		for (int32 Y = 0; Y < InWidthSections; Y++)
		{
			for (int32 Z = 0; Z < InHeightSections; Z++)
			{
				// Setup a cube
				TArray<FVector> Corners;
				Corners.Empty();
				Corners.AddUninitialized(8);
				// const int32 BottomLeftIndex = VertexIndex++;
				// const int32 BottomRightIndex = VertexIndex++;
				// const int32 TopRightIndex = VertexIndex++;
				// const int32 TopLeftIndex = VertexIndex++;
				// const int32 BottomLeftIndex_b = VertexIndex++;
				// const int32 BottomRightIndex_b = VertexIndex++;
				// const int32 TopRightIndex_b = VertexIndex++;
				// const int32 TopLeftIndex_b = VertexIndex++;

				//Calculate relative vertex position from SectionSize
				Corners[0] = FVector(X * SectionSize.X , Y * SectionSize.Y , Z * SectionSize.Z); 
				Corners[1] = FVector((X + 1) * SectionSize.X , Y * SectionSize.Y , Z * SectionSize.Z);
				Corners[2] = FVector((X + 1) * SectionSize.X , (Y + 1) * SectionSize.Y , Z * SectionSize.Z);
				Corners[3] = FVector(X * SectionSize.X , (Y + 1) * SectionSize.Y , Z * SectionSize.Z);
				Corners[4] = FVector(X * SectionSize.X , Y * SectionSize.Y , (Z + 1) * SectionSize.Z);
				Corners[5] = FVector((X + 1) * SectionSize.X , Y * SectionSize.Y , (Z + 1) * SectionSize.Z);
				Corners[6] = FVector((X + 1) * SectionSize.X , (Y + 1) * SectionSize.Y , (Z + 1) * SectionSize.Z);
				Corners[7] = FVector(X * SectionSize.X , (Y + 1) * SectionSize.Y , (Z + 1) * SectionSize.Z);
				
				//Calculate MATERIAL-AIR state from NOISE BUFFER FACTORY
				//Get Edge from Triangulation Table
				//Pass the midpoints of the vertex to Vertex Buffer

				//###########################-- NOISE BUFFER FACTORY --#####################
				TArray<float> VertexValue;
				VertexValue.Empty();
				for(int i=0; i<8; i++)
				{
					VertexValue.Add(Corners[i].Z);
				}
				
				//for number of NoiseOctaves=i
				//EV = NV * Noise(i) * if i > 0 => Mask(i-1)
				for (int32 i=0; i < NumberOfOctaves; i++)
				{
					float frequency = OctaveInitialFrequency * FMath::Pow(Lacunarity, i);
					//float prevFrequency = (i==0) ? frequency : (OctaveInitialFrequency * FMath::Pow(Lacunarity, (i-1)));
					float amplitude = NoiseIntialHeight * FMath::Pow(Persistance, i );
					//Sphere Surface Position
					VertexValue[0] +=  amplitude * FMath::PerlinNoise3D((Corners[0] + NoiseOffset)  * frequency);
					VertexValue[1] +=  amplitude * FMath::PerlinNoise3D((Corners[1] + NoiseOffset)  * frequency); 
					VertexValue[2] +=  amplitude * FMath::PerlinNoise3D((Corners[2] + NoiseOffset)  * frequency);
					VertexValue[3] +=  amplitude * FMath::PerlinNoise3D((Corners[3] + NoiseOffset)  * frequency);
					VertexValue[4] +=  amplitude * FMath::PerlinNoise3D((Corners[4] + NoiseOffset)  * frequency); 
					VertexValue[5] +=  amplitude * FMath::PerlinNoise3D((Corners[5] + NoiseOffset)  * frequency); 
					VertexValue[6] +=  amplitude * FMath::PerlinNoise3D((Corners[6] + NoiseOffset)  * frequency);
					VertexValue[7] +=  amplitude * FMath::PerlinNoise3D((Corners[7] + NoiseOffset)  * frequency);
			
				}

				//################--Get Edge Index from TRIANGULATION TABLE--##################################
				int Index = 0;
				int Index2 = 0;
				int TrigoIndex = 0;
				//Convert point to Trigo Index
				while ( Index < 8 )
				{
					if ( VertexValue[Index] < SurfaceValue )
					{
						TrigoIndex |= 1 << Index;
					}
					Index++;
				}
				//###################~~ VERTEX MACHINE CORE ~~###################################
				while( Index2 < 16 && AChunk::TrigoTable[TrigoIndex][Index2] != -1)
				{
					int TriVertexA = VertexIndex++;
					int TriVertexB = VertexIndex++;
					int TriVertexC = VertexIndex++;

					InVertices.Add(GetLocationOfTrigoPoint(AChunk::TrigoTable[TrigoIndex][Index2],Corners,VertexValue, SurfaceValue, InterpolateSurface));
					InVertices.Add(GetLocationOfTrigoPoint(AChunk::TrigoTable[TrigoIndex][Index2 + 1],Corners,VertexValue, SurfaceValue, InterpolateSurface));
					InVertices.Add(GetLocationOfTrigoPoint(AChunk::TrigoTable[TrigoIndex][Index2 + 2],Corners,VertexValue, SurfaceValue, InterpolateSurface));
				
					// The order of these (clockwise/counter-clockwise) dictates which way the normal will face. 
					InTriangles.Add(TriVertexA);
					InTriangles.Add(TriVertexB);
					InTriangles.Add(TriVertexC);
					
					// UVs
					InTexCoords.Add(FVector2D(static_cast<float>(X) / static_cast<float>(InLengthSections), static_cast<float>(Y) / static_cast<float>(InWidthSections)));
					InTexCoords.Add(FVector2D(static_cast<float>(X) / static_cast<float>(InLengthSections), static_cast<float>(Y + 1) / static_cast<float>(InWidthSections)));
					InTexCoords.Add(FVector2D(static_cast<float>(X + 1) / static_cast<float>(InLengthSections), static_cast<float>(Y + 1) / static_cast<float>(InWidthSections)));
					
					// Normals
					const FVector NormalCurrent = FVector::CrossProduct(InVertices[TriVertexA] - InVertices[TriVertexC], InVertices[TriVertexC] - InVertices[TriVertexB]).GetSafeNormal();
					// If not smoothing we just set the vertex normal to the same normal as the polygon they belong to
					//InNormals[TriVertexA] = InNormals[TriVertexB] = InNormals[TriVertexC] = NormalCurrent;
					InNormals.Add(NormalCurrent);
					InNormals.Add(NormalCurrent);
					InNormals.Add(NormalCurrent);
					
					// Tangents (perpendicular to the surface)
					const FVector SurfaceTangent = (InVertices[TriVertexA] - InVertices[TriVertexB]).GetSafeNormal();
					//InTangents[TriVertexA] = InTangents[TriVertexB] = InTangents[TriVertexC] = SurfaceTangent;
					InTangents.Add(SurfaceTangent);
					InTangents.Add(SurfaceTangent);
					InTangents.Add(SurfaceTangent);

					Index2 += 3;
				}
				// //Finalize vertex for sending to vertex buffer
				// const FVector FBottomLeft = NBottomLeft *  SRadius + EBottomLeft;
				// const FVector FBottomRight = NBottomRight *  SRadius + EBottomRight;
				// const FVector FTopRight = NTopRight *  SRadius + ETopRight;
				// const FVector FTopLeft = NTopLeft *  SRadius + ETopLeft;
			
				// //Pass the vertex to vertex buffer
				// InVertices[BottomLeftIndex] = FBottomLeft;
				// InVertices[BottomRightIndex] = FBottomRight;
				// InVertices[TopRightIndex] = FTopRight;
				// InVertices[TopLeftIndex] = FTopLeft;

 						//################################################################//
					//##################//VERTEX MACHINE--END//##########################//
				//################################################################//
				// Note that Unreal UV origin (0,0) is top left
				// InTexCoords[BottomLeftIndex] = FVector2D(static_cast<float>(X) / static_cast<float>(InLengthSections), static_cast<float>(Y) / static_cast<float>(InWidthSections));
				// InTexCoords[BottomRightIndex] = FVector2D(static_cast<float>(X) / static_cast<float>(InLengthSections), static_cast<float>(Y + 1) / static_cast<float>(InWidthSections));
				// InTexCoords[TopRightIndex] = FVector2D(static_cast<float>(X + 1) / static_cast<float>(InLengthSections), static_cast<float>(Y + 1) / static_cast<float>(InWidthSections));
				// InTexCoords[TopLeftIndex] = FVector2D(static_cast<float>(X + 1) / static_cast<float>(InLengthSections), static_cast<float>(Y) / static_cast<float>(InWidthSections));

				// Now create two triangles from those four vertices
				// The order of these (clockwise/counter-clockwise) dictates which way the normal will face. 
				// InTriangles[TriangleIndex++] = BottomLeftIndex;
				// InTriangles[TriangleIndex++] = TopRightIndex;
				// InTriangles[TriangleIndex++] = TopLeftIndex;

				// InTriangles[TriangleIndex++] = BottomLeftIndex;
				// InTriangles[TriangleIndex++] = BottomRightIndex;
				// InTriangles[TriangleIndex++] = TopRightIndex;

				// Normals
				// const FVector NormalCurrent = FVector::CrossProduct(InVertices[BottomLeftIndex] - InVertices[TopLeftIndex], InVertices[TopLeftIndex] - InVertices[TopRightIndex]).GetSafeNormal();

				// // If not smoothing we just set the vertex normal to the same normal as the polygon they belong to
				// InNormals[BottomLeftIndex] = InNormals[BottomRightIndex] = InNormals[TopRightIndex] = InNormals[TopLeftIndex] = NormalCurrent;

				// // Tangents (perpendicular to the surface)
				// const FVector SurfaceTangent = (PBottomLeft - PBottomRight).GetSafeNormal();
				// InTangents[BottomLeftIndex] = InTangents[BottomRightIndex] = InTangents[TopRightIndex] = InTangents[TopLeftIndex] = SurfaceTangent;
			}
		}
	}
}


FVector	AChunk::GetLocationOfTrigoPoint(const int PointIndex,const TArray<FVector> Corners,const TArray<float> VertexValue, const float SurfaceValue, const bool InterpolateSurface) 
{
	FVector	Result;
	float	Mult;
	float	Divider;

	if ( PointIndex == 0 )
	{
		Divider = VertexValue[1] - VertexValue[0];
		Mult = SurfaceValue - VertexValue[0];
		if ( InterpolateSurface )
			Result.X = (Corners[1].X - Corners[0].X) / Divider * Mult + Corners[0].X;
		else 
			Result.X = (Corners[1].X - Corners[0].X) / 2 + Corners[0].X;
		Result.Y = Corners[1].Y;
		Result.Z = Corners[1].Z;
	}
	else if ( PointIndex == 1 )
	{
		Divider = VertexValue[2] - VertexValue[1];
		Mult = SurfaceValue - VertexValue[1];
		Result.X = Corners[2].X;
		if ( InterpolateSurface )
			Result.Y = (Corners[2].Y - Corners[1].Y) / Divider * Mult + Corners[1].Y;
		else 
			Result.Y = (Corners[2].Y - Corners[1].Y) / 2 + Corners[1].Y;
		Result.Z = Corners[2].Z;
	}
	else if ( PointIndex == 2 )
	{
		Divider = VertexValue[2] - VertexValue[3];
		Mult = SurfaceValue - VertexValue[3];
		if ( InterpolateSurface )
			Result.X = (Corners[2].X - Corners[3].X) / Divider * Mult + Corners[3].X;
		else 
			Result.X = (Corners[2].X - Corners[3].X) / 2 + Corners[3].X;
		Result.Y = Corners[3].Y;
		Result.Z = Corners[3].Z;
	}
	else if ( PointIndex == 3 )
	{
		Divider = VertexValue[3] - VertexValue[0];
		Mult = SurfaceValue - VertexValue[0];
		Result.X = Corners[3].X;
		if ( InterpolateSurface )
			Result.Y = (Corners[3].Y - Corners[0].Y) / Divider * Mult + Corners[0].Y;
		else 
			Result.Y = (Corners[3].Y - Corners[0].Y) / 2 + Corners[0].Y;
		Result.Z = Corners[3].Z;
	}
	else if ( PointIndex == 4 )
	{
		Divider = VertexValue[5] - VertexValue[4];
		Mult = SurfaceValue - VertexValue[4];
		if ( InterpolateSurface )
			Result.X = (Corners[5].X - Corners[4].X) / Divider * Mult + Corners[4].X;
		else 
			Result.X = (Corners[5].X - Corners[4].X) / 2 + Corners[4].X;
		Result.Y = Corners[5].Y;
		Result.Z = Corners[5].Z;
	}
	else if ( PointIndex == 5 )
	{
		Divider = VertexValue[6] - VertexValue[5];
		Mult = SurfaceValue - VertexValue[5];
		Result.X = Corners[6].X;
		if ( InterpolateSurface )
			Result.Y =(Corners[6].Y - Corners[5].Y) / Divider * Mult + Corners[5].Y;
		else 
			Result.Y =(Corners[6].Y - Corners[5].Y) / 2 + Corners[5].Y;
		Result.Z = Corners[6].Z;
	}
	else if ( PointIndex == 6 )
	{
		Divider = VertexValue[6] - VertexValue[7];
		Mult = SurfaceValue - VertexValue[7];
		if ( InterpolateSurface )
			Result.X = (Corners[6].X - Corners[7].X) / Divider * Mult + Corners[7].X;
		else 
			Result.X = (Corners[6].X - Corners[7].X) / 2 + Corners[7].X;
		Result.Y = Corners[7].Y;
		Result.Z = Corners[7].Z;
	}
	else if ( PointIndex == 7 )
	{
		Divider = VertexValue[7] - VertexValue[4];
		Mult = SurfaceValue - VertexValue[4];
		Result.X = Corners[7].X;
		if ( InterpolateSurface )
			Result.Y = (Corners[7].Y - Corners[4].Y) / Divider * Mult + Corners[4].Y;
		else 
			Result.Y = (Corners[7].Y - Corners[4].Y) / 2 + Corners[4].Y;
		Result.Z = Corners[7].Z;
	}
	else if ( PointIndex == 8 )
	{
		Divider = VertexValue[4] - VertexValue[0];
		Mult = SurfaceValue - VertexValue[0];
		Result.X = Corners[4].X;
		Result.Y = Corners[4].Y;
		if ( InterpolateSurface )
			Result.Z = (Corners[4].Z - Corners[0].Z) / Divider * Mult + Corners[0].Z;
		else 
			Result.Z = (Corners[4].Z - Corners[0].Z) / 2 + Corners[0].Z;
	}
	else if ( PointIndex == 9 )
	{
		Divider = VertexValue[5] - VertexValue[1];
		Mult = SurfaceValue - VertexValue[1];
		Result.X = Corners[5].X;
		Result.Y = Corners[5].Y;
		if ( InterpolateSurface )
			Result.Z = (Corners[5].Z - Corners[1].Z) / Divider * Mult + Corners[1].Z;
		else 
			Result.Z = (Corners[5].Z - Corners[1].Z) / 2 + Corners[1].Z;
	}
	else if ( PointIndex == 10 )
	{
		Divider = VertexValue[6] - VertexValue[2];
		Mult = SurfaceValue - VertexValue[2];
		Result.X = Corners[6].X;
		Result.Y = Corners[6].Y;
		if ( InterpolateSurface )
			Result.Z = (Corners[6].Z - Corners[2].Z) / Divider * Mult + Corners[2].Z;
		else 
			Result.Z = (Corners[6].Z - Corners[2].Z) / 2 + Corners[2].Z;
	}
	else if ( PointIndex == 11 )
	{
		Divider = VertexValue[7] - VertexValue[3];
		Mult = SurfaceValue - VertexValue[3];
		Result.X = Corners[7].X;
		Result.Y = Corners[7].Y;
		if ( InterpolateSurface )
			Result.Z = (Corners[7].Z - Corners[3].Z) / Divider * Mult + Corners[3].Z;
		else 
			Result.Z = (Corners[7].Z - Corners[3].Z) / 2 + Corners[3].Z;
	}
	return ( Result );
}