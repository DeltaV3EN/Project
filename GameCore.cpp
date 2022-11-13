#include "GameCore.h"
#include "CommandParser.h"
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <Windows.h>


RoomData CreateRoom(const std::string& inName, const std::string& inDescription, bool hasKey, bool CanFightHere, bool WantstoFight)
{
	RoomData room = {};
	room.Name = inName;
	room.Description = inDescription;
	room.HasKey = hasKey;
	room.CanfightHere = CanFightHere;
	
	
	return room;
}
void AddExittoRoom(RoomData& roomtoEdit, const std::string& exitName, int targetRoomIndex, bool isLocked)
{
	RoomExitData exit = {};
	exit.Name = exitName;
	exit.TargetRoomIndex = targetRoomIndex;
	exit.locked = isLocked;
	roomtoEdit.Exits.push_back(exit);

}




void InitializeGame(PlayerState& playerState, WorldState& worldState)
{
	printf("\nName:Geguri");
	printf("\nTitle:BloodWitch");
	printf("\nObjective:Find an artifact to save your girlfriend...Alexander is waiting for you\n");
	
	
	playerState.CurrentRoomIndex = 0;
	playerState.WantsToLook = true;
	playerState.hasKey = false;
	playerState.Wantstofight = false;
	
// Indenx1
RoomData outside = CreateRoom("Outside", "The entrace to the forest ahead of you, you must get through it to reacht the temple", false,false, false);
AddExittoRoom(outside, "forest", 1, false);
worldState.Rooms.push_back(outside);


//Index2
RoomData forest = CreateRoom("Forest", "The forest outside the holy temple, it has an errie feel", false, false, false);
AddExittoRoom(forest, "outside", 1, true);
AddExittoRoom(forest, "gravelpit", 2, false);
worldState.Rooms.push_back(forest);


//Index 3
RoomData gravelpit = CreateRoom("Gravel pit", "A large pit of gravel, surronded by stone statues, you can see the temple in the distance", false, false, false);
AddExittoRoom(gravelpit, "forest", 2, false);
AddExittoRoom(gravelpit, "entrance", 3, false);
worldState.Rooms.push_back(gravelpit);


//Index 4
RoomData entrance = CreateRoom("Temple Entrance", "You are here, the entrance to the temple", true, true, false);
AddExittoRoom(entrance, "gravelpit",3,true);
AddExittoRoom(entrance, "floor1", 4, false);
worldState.Rooms.push_back(entrance);


//Index 5
RoomData floor1 = CreateRoom("Floor1:Temple of holy light (CANT ESCAPE NOW)", "The first floor of the temple, a medium sized room with statues of holy beings all around", true, true, false);
AddExittoRoom(floor1, "entrance", 4, true);
AddExittoRoom(floor1, "floor2", 5,false);
worldState.Rooms.push_back(floor1);


//Index6 AKA the final room
RoomData floor2 = CreateRoom("Floor 2:Temple of the holy light", "the second floor, much deeper, much darker\n Defeat the boss to optain the artifact.\n You sense someone who wants to fight here", true, false, true);
AddExittoRoom(floor2, "none left", 1,true);

worldState.Rooms.push_back(floor2);
}






void GetInput(PlayerState& playerState, const WorldState& worldState)
{
	playerState.WantsToLook = false;
	playerState.Wantstofight = false;
	playerState.DesiredExit = "";
	playerState.DesiredPickup = "";
	
	printf("What do you do?\n");
	printf("> ");
	TextAdventureCommand command = ParseAdventureCommand();
	if (command.Verb == "quit")
	{
		playerState.WantsToExit = true;
	}
	else if (command.Verb == "look")
	{
		playerState.WantsToLook = true;


	}
	else if (command.Verb == "go")
	{
		playerState.DesiredExit = command.Parameter;
	}
	else if (command.Verb == "get") 
	{
		playerState.DesiredPickup = command.Parameter;
	}
	else if (command.Verb == "fight") 
	{
		playerState.Wantstofight = true;
	}
	else if (command.Verb == "help")
	{
		printf("Command List: look, quit, go [place], get key, fight. \n");
	}
	else
	{
		printf("I don't understand\n");
	}
	printf("\n");

}

void RenderGame(const PlayerState& playerState, const WorldState& worldState)
{


	if (playerState.WantsToLook)
	{
		RoomData currRoom = worldState.Rooms[playerState.CurrentRoomIndex];
		printf("=================================================\n");
		printf("Location: %s\n", currRoom.Name.c_str());
		printf("%s\n\n", currRoom.Description.c_str());
		
		if (currRoom.HasKey)
		{
			printf("Yours senses tell you a key is nearby \n\n");
		}
		printf("EXITS: \n");
		for (unsigned int i = 0; i < currRoom.Exits.size(); ++i)
		{
			printf("%s\n", currRoom.Exits[i].Name.c_str());
		}
		printf("\n");

		if (playerState.hasKey)
		{
			printf("Inventory:\n");
			printf("Key\n");
		
		}

		printf("\n");

	}






}

void UpdateGame(PlayerState& playerState, WorldState& worldState)
{

	int Enter;
	std::string input;
	const char* Playername = "Geguri";
	const char* Bossname = "Alexander";
	int PHP = 100;
	int PMP = 50;
	int HealspellPower = 55;
	int HealspellCost = 5;
	int BloodburnPower = 18;
	int BloodCost = 9;
	int AttackP = 10;
	int BossHP = 100;
	int BossDPS = 30;
	int PlayerLevel = 10;


	RoomData currRoom = worldState.Rooms[playerState.CurrentRoomIndex];
	if (playerState.DesiredExit != "")
	{
		bool foundExit = false;

		for (unsigned int i = 0; i < currRoom.Exits.size(); ++i)
		{


			if (playerState.DesiredExit == currRoom.Exits[i].Name)
			{
				foundExit = true;
				if (!currRoom.Exits[i].locked)
				{
					playerState.CurrentRoomIndex = currRoom.Exits[i].TargetRoomIndex;
					playerState.WantsToLook = true;
				}
				else
				{
					if (playerState.hasKey)
					{
						playerState.CurrentRoomIndex = currRoom.Exits[i].TargetRoomIndex;
						playerState.WantsToLook = true;
					}
					else
					{
						printf("\n Nope, cant go forward, check before to try again");
					}
				}
			}

		}

		if (!foundExit)
		{
			printf("I could not find an exit called '%s'\n", playerState.DesiredExit.c_str());

		}


	}
	else if (playerState.DesiredPickup != "")
	{
		if (playerState.DesiredPickup == "key")
		{
			if (currRoom.HasKey)
			{
				printf("Found a key \n");
				playerState.HasKey = true;
				currRoom.HasKey = false;
			}
			else
			{
				printf("You dont sense a key here......\n");
			}
		}
		else
		{
			printf("No valid command, try somthing other then '%s'\n", playerState.DesiredPickup.c_str());
		}

		{


		}
		if (playerState.Wantstofight)
		{
			if (currRoom.CanfightHere)
			{
				; printf("As the gravel beneath your feet crunches");
				printf("\n you can't help be feel....");
				printf("\n Afraid..");
				Sleep(1000);
				printf("\n of the Dangers the lie ahead.");
				printf("\n As if on Queue you hear a powerful voice speak to you, no where near but not sounding far.");
				Sleep(1000);
				printf("\n Foolish Mortal, If you dare continue to step forward, to stain these holy grounds with your worthless life.");
				printf("\n Then continue forward, but be warned vermin!!!");
				Sleep(1000);
				printf("\n I shall show you the wraith of the Holy one.");
				Sleep(1000);

				printf("\n..\n...........\n ..... \n...\n\n...\n.......");
				Sleep(1000);
				printf("\nYOU HAVE MADE YOUR CHOICE VERMIN, NOW PREPARE TO DIE BY MY HANDS!!!!");
				Sleep(500);
				printf("\n\n......................................");
				printf("\n.Alexander the Holy Paladin of Hatred.");
				printf("\n.                                    .");
				printf("\n.                                    .");
				printf("\n.                                    .");
				printf("\n.                                    .");
				printf("\n.          Health:100                .");
				printf("\n.           ATTACK:30                .");
				printf("\n.                                    .");
				printf("\n.                                    .");
				printf("\n......................................");
				printf("\n\n Stats-");

				while (BossHP > 0 && PHP > 0) {
					// Player code
					std::cout << "Boss HP: " << BossHP;
					std::cout << "\nHP:" << PHP;
					std::cout << "\nATK:" << AttackP;
					std::cout << "\nMP:" << PMP;
					printf("\n Press 1 for physical attack, 2 for healing spell (cost 5), or 3 to Cast an attack spell");

					int choice = 0;
					scanf_s("%d", &choice);
					printf("\n");
					if (choice == 1) {
						printf("%s attacks!\n", Playername);
						printf("%s's Hit points have been reduced by %d \n", Bossname, AttackP);
						printf("%s has %d HP left", Bossname, BossHP);
						printf("\n");
						BossHP = BossHP - AttackP;
						Sleep(1000);
					}
					else if (choice == 2) {
						printf("%s casts Heal spell\n", Playername);

						if (PMP >= HealspellCost) {
							printf("heal'd for %d\n", HealspellPower);
							printf("\n");
							PHP = PHP + HealspellPower;
							PMP = PMP - HealspellCost;
							Sleep(1000);
						}
						else {
							printf("Sorry pal, not enough MP\n");
							Sleep(1000);
						}

					}
					else if (choice == 3) {
						if (PMP >= BloodCost) {
							printf("\nYou use your nails to cut a nick into your finger, using your blood and a spell to create of crimson wall of mana and flames");
							PHP = PHP - 3;
							printf("\n%s's Hit points have been reduced by %d \n", Bossname, BloodburnPower);
							BossHP = BossHP - BloodburnPower;
						}
						else {
							printf("Sorry pal, not enough MP\n");
							Sleep(1000);
						}
					}






					else {
						printf("hey look, that isnt a valid input, its 1,2,3 time to be punished");
					}


					//Alexander
					printf("\n%s raises his sword, and smites you down", Bossname);
					printf("\n the attack deals %d to %s,\n", BossDPS, Playername);
					printf("\n");
					PHP = PHP - BossDPS;
					Sleep(1000);
				}

				if (PHP > 0 && BossHP <= 0) {
					printf(".............\n");
					printf("how!?");
					Sleep(1000);
					printf("\nHOW CAN I BE BEATEN BY THE LIKES OF SCUM LIKE YOU");
					Sleep(1000);
					printf("\nTHIS IS NOT OVER!!!!!!");
					printf("\nthe soul of alexander leaves his body, preparing to go find a new host");
					Sleep(1000);
					printf("\n");
					system("pause");
					return;

				}
				else if (BossHP > 0); {
					printf("\nHAHAHAHA GOODBYE YOU VERMIN");
					Sleep(1000);
					printf("\nYou have been defeated, get back up strong hero");
					printf("\n");
					system("pause");
					return;
				}





				system("pause");
				return;
			}
			else
			{
				printf("there is not one to challenge...\n");
			}
		}

	}


}
	void CleanupGame(PlayerState & playerState, WorldState & worldState)
{
	printf("Quitting...\n\n");
}