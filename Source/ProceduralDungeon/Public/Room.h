/*
 * MIT License
 *
 * Copyright (c) 2019-2022 Benoit Pelletier
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include "CoreMinimal.h"
#include "ProceduralLevelStreaming.h"
#include "GameFramework/Actor.h"
#include "ProceduralDungeonTypes.h"
#include "Room.generated.h"

class ADungeonGenerator;
class ARoomLevel;
class URoomData;
class ADoor;

USTRUCT()
struct FRoomConnection
{
	GENERATED_BODY()

	UPROPERTY()
	TWeakObjectPtr<URoom> OtherRoom = nullptr;
	int OtherDoorIndex = -1;
	ADoor* DoorInstance = nullptr;
};

UCLASS()
class PROCEDURALDUNGEON_API URoom : public UObject
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TArray<FRoomConnection> Connections;

	ADungeonGenerator* GeneratorOwner;
	int64 Id;

public:
	UPROPERTY()
	UProceduralLevelStreaming* Instance;
	UPROPERTY()
	FIntVector Position;
	EDoorDirection Direction;

	URoomData* GetRoomData() { return RoomData; }

	ADungeonGenerator* Generator() const { return GeneratorOwner; }

private:
	UPROPERTY()
	URoomData* RoomData;

public:
	void Init(URoomData* RoomData, ADungeonGenerator* Generator, int32 RoomId);

	bool IsConnected(int Index);
	void SetConnection(int Index, URoom* Room, int OtherDoorIndex);
	TWeakObjectPtr<URoom> GetConnection(int Index);
	int GetFirstEmptyConnection();

	void Instantiate(UWorld* World);
	void Destroy(UWorld* World);
	ARoomLevel* GetLevelScript();
	bool IsInstanceLoaded();
	bool IsInstanceUnloaded();

	EDoorDirection GetDoorWorldOrientation(int DoorIndex);
	FIntVector GetDoorWorldPosition(int DoorIndex);
	int GetConnectionCount() { return Connections.Num(); }
	int GetDoorIndexAt(FIntVector WorldPos, EDoorDirection WorldRot);
	bool IsDoorInstanced(int DoorIndex);
	void SetDoorInstance(int DoorIndex, ADoor* Door);
	int GetOtherDoorIndex(int DoorIndex);
	void TryConnectToExistingDoors(TArray<URoom*>& RoomList);

	FIntVector WorldToRoom(FIntVector WorldPos);
	FIntVector RoomToWorld(FIntVector RoomPos);
	EDoorDirection WorldToRoom(EDoorDirection WorldRot);
	EDoorDirection RoomToWorld(EDoorDirection RoomRot);
	void SetRotationFromDoor(int DoorIndex, EDoorDirection WorldRot);
	void SetPositionFromDoor(int DoorIndex, FIntVector WorldPos);
	void SetPositionAndRotationFromDoor(int DoorIndex, FIntVector WorldPos, EDoorDirection WorldRot);
	bool IsOccupied(FIntVector Cell);

	// AABB Overlapping
	static bool Overlap(URoom& A, URoom& B);
	static bool Overlap(URoom& Room, TArray<URoom*>& RoomList);
	static EDoorDirection Add(EDoorDirection A, EDoorDirection B);
	static EDoorDirection Sub(EDoorDirection A, EDoorDirection B);
	static EDoorDirection Opposite(EDoorDirection O);
	static FIntVector GetDirection(EDoorDirection O);
	static FIntVector Rotate(FIntVector Pos, EDoorDirection Rot);

	static FVector GetRealDoorPosition(FIntVector DoorCell, EDoorDirection DoorRot, bool includeOffset = true);

	static void Connect(URoom& RoomA, int DoorA, URoom& RoomB, int DoorB);
	static URoom* GetRoomAt(FIntVector RoomCell, TArray<URoom*>& RoomList);

	// Plugin Settings
	static FVector Unit();
	static FVector DoorSize();
	static float DoorOffset();
	static bool OcclusionCulling();
	static bool DrawDebug();
	static bool CanLoop();
};
