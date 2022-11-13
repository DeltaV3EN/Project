#pragma once
#include <String>
#include <vector>



struct PlayerState
{
	int CurrentRoomIndex;
	std::string DesiredExit;
	std::string DesiredPickup;
	bool WantsToExit;
	bool WantsToLook;
	bool hasKey;
	bool HasKey;
	bool Wantstofight;
};

struct RoomExitData
{
	std::string Name;
	int TargetRoomIndex;
	bool locked;
	};


struct RoomData
{	
	std::string Name;
	std::string Description;
	std::vector<RoomExitData> Exits;
	bool HasKey;
	bool CanfightHere;
};
struct WorldState
{
	std::vector<RoomData> Rooms;


};

RoomData CreateRoom(const std::string& inName, const std::string& inDescription);

void InitializeGame(PlayerState& playerState, WorldState& worldState);
void GetInput(PlayerState& playerState, const WorldState& worldState);
void RenderGame(const PlayerState& playerState, const WorldState& worldState);
void UpdateGame(PlayerState& playerState, WorldState& worldState);
void CleanupGame(PlayerState& playerState, WorldState& worldState);