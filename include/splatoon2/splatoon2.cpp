#define TESLA_INIT_IMPL // If you have more than one file using the tesla header, only define this in the main one
#include <tesla.hpp>    // The Tesla Header
	

	extern DmntCheatProcessMetadata metadata;



namespace Splatoon2 {
	
	
	enum {RED, GREEN, BLUE};
	enum {ALPHA, BRAVO, CHARLIE, NEUTRAL};
	enum CHEATTYPE {WEAPON, HEAD, CLOTHES, SHOES} ;


	tsl::elm::TrackBar* bar[3] = {0, 0, 0};
	int Team = 0;



	void applyColor(float *color, int team);
	
	tsl::elm::Element* chooseWeapon();
	tsl::elm::Element* chooseHead();
	tsl::elm::Element* chooseClothes();
	tsl::elm::Element* chooseShoes();

	bool debugService_isRunning();


	class GuiEquipCheats2 : public tsl::Gui {
	public:
		CHEATTYPE CheatType;
		
		GuiEquipCheats2(CHEATTYPE cheatType) {
			CheatType = cheatType;
		}

		virtual tsl::elm::Element* createUI() override {
			
			tsl::elm::Element* list;
			
			tsl::elm::OverlayFrame *rootFrame;
			
			switch (CheatType)
			{
				case WEAPON:
					rootFrame = new tsl::elm::OverlayFrame("OVLsplatter", "Equip weapon cheats");
					
					
					list = chooseWeapon();
					
					
					break;
				case HEAD:
					rootFrame = new tsl::elm::OverlayFrame("OVLsplatter", "Equip headgear cheats");
					
					list = chooseHead();
					
					break;
				case CLOTHES:
					rootFrame = new tsl::elm::OverlayFrame("OVLsplatter", "Equip clothes cheats");
					
					list = chooseClothes();
					
					break;
				case SHOES:
					rootFrame = new tsl::elm::OverlayFrame("OVLsplatter", "Equip shoes cheats");
					
					list = chooseShoes();
					
					break;
			}


			
			
			
			rootFrame->setContent(list);


			return rootFrame;
		}
	};


	class GuiEquipCheats : public tsl::Gui {
	public:
		GuiEquipCheats() {}

		virtual tsl::elm::Element* createUI() override {
			
			auto *rootFrame = new tsl::elm::OverlayFrame("OVLsplatter", "Equip cheats");

			auto list = new tsl::elm::List();



			auto *WeaponItem = new tsl::elm::ListItem("Swap weapon", "...");
			WeaponItem->setClickListener([](u64 keys) {
				if (keys & HidNpadButton_A) {
					tsl::changeTo<GuiEquipCheats2>(WEAPON);
					return true;
				}

				return false;
			});


			list->addItem(WeaponItem);


			auto *HeadItem = new tsl::elm::ListItem("Swap head", "...");
			HeadItem->setClickListener([](u64 keys) {
				if (keys & HidNpadButton_A) {
					tsl::changeTo<GuiEquipCheats2>(HEAD);
					return true;
				}

				return false;
			});


			list->addItem(HeadItem);


			auto *ClothesItem = new tsl::elm::ListItem("Swap clothes", "...");
			ClothesItem->setClickListener([](u64 keys) {
				if (keys & HidNpadButton_A) {
					tsl::changeTo<GuiEquipCheats2>(CLOTHES);
					return true;
				}

				return false;
			});


			list->addItem(ClothesItem);


			auto *ShoesItem = new tsl::elm::ListItem("Swap shoes", "...");
			ShoesItem->setClickListener([](u64 keys) {
				if (keys & HidNpadButton_A) {
					tsl::changeTo<GuiEquipCheats2>(SHOES);
					return true;
				}

				return false;
			});


			list->addItem(ShoesItem);

			
			rootFrame->setContent(list);


			return rootFrame;
		}
	};


	class GuiColor : public tsl::Gui {
		public:
			GuiColor() {}
			
		virtual tsl::elm::Element* createUI() override {
			
			auto *rootFrame = new tsl::elm::OverlayFrame("OVLsplatter", "Change colors");

			auto list = new tsl::elm::List();


			list->addItem(new tsl::elm::CategoryHeader("Choose the color"));
			
			
			bar[RED] = new tsl::elm::TrackBar("R:");
			bar[GREEN] = new tsl::elm::TrackBar("G:");
			bar[BLUE] =	new tsl::elm::TrackBar("B:");
			
			
			
			
			
			for (int barNum = 0; barNum < 3; barNum++)
			{
				bar[barNum]->setClickListener([](u64 keys) {
					if (keys & HidNpadButton_A) {
						
						applyColor(new float[3]{bar[RED]->getProgress() / 100.0, bar[GREEN]->getProgress() / 100.0, bar[BLUE]->getProgress() / 100.0}, Team);
						
						return true;
					}

					return false;
				});
			}
			

			list->addItem(bar[RED]);
			list->addItem(bar[GREEN]);
			list->addItem(bar[BLUE]);

			list->addItem(new tsl::elm::CategoryHeader("Color:", true));



			list->addItem(new tsl::elm::CustomDrawer([](tsl::gfx::Renderer *renderer, s32 x, s32 y, s32 w, s32 h) {
				renderer->drawRect(x, y, 600, 400, renderer->a(0xF000 + 0x0100 * (int) (bar[BLUE]->getProgress() * 15 / 100.0) + 0x0010 * (int) (bar[GREEN]->getProgress() * 15 / 100.0) + 0x0001 * (int) (bar[RED]->getProgress() * 15 / 100.0)));
			}), 100);

			
			
			rootFrame->setContent(list);


			return rootFrame;
		}
	};


	class GuiTeams : public tsl::Gui {
	public:
		GuiTeams() {}

		virtual tsl::elm::Element* createUI() override {
			
			auto *rootFrame = new tsl::elm::OverlayFrame("OVLsplatter", "Change colors");

			auto list = new tsl::elm::List();


			list->addItem(new tsl::elm::CategoryHeader("Choose the team"));



			auto *AlphaTeam = new tsl::elm::ListItem("Alpha team", "...");
			AlphaTeam->setClickListener([](u64 keys) {
				if (keys & HidNpadButton_A) {
					Team = ALPHA;
					tsl::changeTo<GuiColor>();
					return true;
				}

				return false;
			});
			
			
			auto *BravoTeam = new tsl::elm::ListItem("Bravo team", "...");
			BravoTeam->setClickListener([](u64 keys) {
				if (keys & HidNpadButton_A) {
					Team = BRAVO;
					tsl::changeTo<GuiColor>();
					return true;
				}

				return false;
			});
			
			

			auto *NeutralTeam = new tsl::elm::ListItem("Neutral team", "...");
			NeutralTeam->setClickListener([](u64 keys) {
				if (keys & HidNpadButton_A) {
					Team = NEUTRAL;
					tsl::changeTo<GuiColor>();
					return true;
				}

				return false;
			});
			
			
			
			list->addItem(AlphaTeam);
			list->addItem(BravoTeam);
			list->addItem(NeutralTeam);
			
			
			rootFrame->setContent(list);


			return rootFrame;
		}
	};

	class GuiTest2 : public tsl::Gui {
	public:
		GuiTest2() {	}	

		// Called when this Gui gets loaded to create the UI
		// Allocate all elements on the heap. libtesla will make sure to clean them up when not needed anymore
		virtual tsl::elm::Element* createUI() override {
			// A OverlayFrame is the base element every overlay consists of. This will draw the default Title and Subtitle.
			// If you need more information in the header or want to change it's look, use a HeaderOverlayFrame.
			auto frame = new tsl::elm::OverlayFrame("OVLsplatter", "v2.0.1");

			// A list that can contain sub elements and handles scrolling
			auto list = new tsl::elm::List();

			// List Items
			list->addItem(new tsl::elm::CategoryHeader("Choose what to do"));
			
			
			//Change colors
			auto *changeColorsItem = new tsl::elm::ListItem("Change colors", "...");
			changeColorsItem->setClickListener([](u64 keys) {
				if (keys & HidNpadButton_A) {
					tsl::changeTo<GuiTeams>();
					return true;
				}

				return false;
			});
			
			
			//Change colors
			auto *equipCheatsItem = new tsl::elm::ListItem("Cheats to apply when you are in the shoal waiting for a match to start and you can open the equip menu with +", "...");
			equipCheatsItem->setClickListener([](u64 keys) {
				if (keys & HidNpadButton_A) {
					tsl::changeTo<GuiEquipCheats>();
					return true;
				}

				return false;
			});

			list->addItem(changeColorsItem);
			list->addItem(equipCheatsItem);

			// Add the list to the frame for it to be drawn
			frame->setContent(list);

			// Return the frame to have it become the top level element of this Gui
			return frame;
		}

		// Called once every frame to update values
		virtual void update() override {
		}

		// Called once every frame to handle inputs not handled by other UI elements
		virtual bool handleInput(u64 keysDown, u64 keysHeld, const HidTouchState &touchPos, HidAnalogStickState joyStickPosLeft, HidAnalogStickState joyStickPosRight) override {
			return false;   // Return true here to signal the inputs have been consumed
		}
	};


	void applyColor(float *color, int team)
	{
		
		//Oct
		for (unsigned int offset = 0xACCD6A4; offset <= 0xACCE124; offset += 0x1C0)
			dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + offset + 0x40 * team, color, sizeof(float) * 3);
		
		//Vss gachi
		for (unsigned int offset = 0xACB1604; offset <= 0xACB2244; offset += 0x1C0)
			dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + offset + 0x40 * team, color, sizeof(float) * 3);
		
		//Vss fest
		for (unsigned int offset = 0xACB87EC; offset <= 0xACB87EC; offset += 0x1C0)
			dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + offset + 0x40 * team, color, sizeof(float) * 3);
		for (unsigned int offset = 0xACBF654; offset <= 0xACC4DD4; offset += 0x1C0)
			dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + offset + 0x40 * team, color, sizeof(float) * 3);
		
		//Coop
		for (unsigned int offset = 0xACC667C; offset <= 0xACC6D7C; offset += 0x1C0)
			dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + offset + 0x40 * team, color, sizeof(float) * 3);
		
		//Regular
		for (unsigned int offset = 0xACAA5DC; offset <= 0xACAB59C; offset += 0x1C0)
			dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + offset + 0x40 * team, color, sizeof(float) * 3);
		
		//Mission
		for (unsigned int offset = 0xACA35B4; offset <= 0xACA4C74; offset += 0x1C0)
			dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + offset + 0x40 * team, color, sizeof(float) * 3);
		
		
		
		tsl::goBack();
	}
	
	
	
	
	tsl::elm::Element* chooseWeapon(){
		
		
		auto list = new tsl::elm::List();
		
		
		auto *Weapon = new tsl::elm::ListItem("Hero shot Lv3 (doesn't shot!)");
		Weapon->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 10020;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA20644, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Weapon);
		
		
		Weapon = new tsl::elm::ListItem("Hero roller Lv2");
		Weapon->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 10510;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA20644, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Weapon);
		
		
		Weapon = new tsl::elm::ListItem("Hero brush Lv2");
		Weapon->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 10610;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA20644, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Weapon);
		
		
		Weapon = new tsl::elm::ListItem("Hero charger Lv2");
		Weapon->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 11010;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA20644, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Weapon);
		
		
		Weapon = new tsl::elm::ListItem("Hero slosher Lv2");
		Weapon->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 11510;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA20644, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Weapon);
		
		
		Weapon = new tsl::elm::ListItem("Hero splatling Lv2");
		Weapon->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 12010;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA20644, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Weapon);
		
		
		Weapon = new tsl::elm::ListItem("Hero dualies Lv2");
		Weapon->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 12510;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA20644, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Weapon);
		
		
		Weapon = new tsl::elm::ListItem("Hero brella Lv2");
		Weapon->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 13010;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA20644, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Weapon);
		
		
		Weapon = new tsl::elm::ListItem("Octo roller Lv2");
		Weapon->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 15021;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA20644, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Weapon);
		
		
		Weapon = new tsl::elm::ListItem("Octo slosher Lv2");
		Weapon->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 15031;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA20644, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Weapon);
		
		
		Weapon = new tsl::elm::ListItem("Sanitized octo dualies Lv2");
		Weapon->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 16041;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA20644, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Weapon);
		
		
		Weapon = new tsl::elm::ListItem("Sanitized octo brella Lv2");
		Weapon->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 16051;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA20644, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Weapon);
		
		
		Weapon = new tsl::elm::ListItem("Sanitized octobrush Lv2");
		Weapon->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 16061;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA20644, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Weapon);
		
		
		Weapon = new tsl::elm::ListItem("Grizzco blaster");
		Weapon->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 20000;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA20644, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Weapon);
		
		
		Weapon = new tsl::elm::ListItem("Grizzco brella");
		Weapon->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 20010;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA20644, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Weapon);
		
		
		Weapon = new tsl::elm::ListItem("Grizzco charger");
		Weapon->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 20020;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA20644, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Weapon);
		
		
		Weapon = new tsl::elm::ListItem("Grizzco slosher");
		Weapon->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 20030;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA20644, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Weapon);
		
		
		Weapon = new tsl::elm::ListItem("Really weird octoshot");
		Weapon->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 25000;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA20644, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Weapon);
		
		
		Weapon = new tsl::elm::ListItem("Really weird splatling");
		Weapon->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 25500;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA20644, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Weapon);
		
		
		Weapon = new tsl::elm::ListItem("Hero charger (without pointer!)");
		Weapon->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 26000;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA20644, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Weapon);
		
		
		Weapon = new tsl::elm::ListItem("Silent golden dynamo roller");
		Weapon->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 26500;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA20644, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Weapon);
		
		
		
		
		
		
		
		return list;
	}




	tsl::elm::Element* chooseHead(){
		
		auto list = new tsl::elm::List();
		
		
		
		
		
		auto *Head = new tsl::elm::ListItem("No headgear");
		Head->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 0;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA2069C, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Head);
		
		
		Head = new tsl::elm::ListItem("Coop cap");
		Head->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 20000;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA2069C, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Head);
		
		
		Head = new tsl::elm::ListItem("Hero helmet");
		Head->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 27102;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA2069C, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Head);
		
		
		Head = new tsl::elm::ListItem("Octo hero helmet");
		Head->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 27201;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA2069C, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Head);
		
		
		Head = new tsl::elm::ListItem("Sanitized 3 goo");
		Head->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 27300;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA2069C, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Head);
		
		
		return list;
	}
	


	tsl::elm::Element* chooseClothes(){
		
		
		auto list = new tsl::elm::List();
		
		
		
		
		
		auto *Clothes = new tsl::elm::ListItem("No clothes");
		Clothes->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 0;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA2067C, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Clothes);
		
		
		Clothes = new tsl::elm::ListItem("Coop slopsuit");
		Clothes->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 20000;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA2067C, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Clothes);
		
		
		Clothes = new tsl::elm::ListItem("Agent 8 suit");
		Clothes->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 27200;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA2067C, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Clothes);
		
		
		Clothes = new tsl::elm::ListItem("Hero jacket");
		Clothes->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 27102;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA2067C, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Clothes);
		
		
		Clothes = new tsl::elm::ListItem("Octo hero jacket");
		Clothes->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 27201;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA2067C, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Clothes);
		
		return list;		
	}


	tsl::elm::Element* chooseShoes(){
		
		auto list = new tsl::elm::List();
		
		
		
		
		
		auto *Shoes = new tsl::elm::ListItem("No shoes");
		Shoes->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 0;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA2065C, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Shoes);
		
		
		Shoes = new tsl::elm::ListItem("Agent 8 boots");
		Shoes->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 27200;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA2065C, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Shoes);
		
		
		Shoes = new tsl::elm::ListItem("Hero boots");
		Shoes->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 27102;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA2065C, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Shoes);
		
		
		Shoes = new tsl::elm::ListItem("Octo hero boots");
		Shoes->setClickListener([](u64 keys) {
			if (keys & HidNpadButton_A) {
				int ID = 27201;
				dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xAA2065C, &ID, sizeof(int));
				return true;
			}

			return false;
		});
		
		list->addItem(Shoes);
		
		
		return list;
	}



	bool debugService_isRunning()
	{
		Handle handle;
		SmServiceName service=smEncodeName("dmnt:cht");
		bool isRunning=R_FAILED(smRegisterService(&handle, service, false, 1));
		svcCloseHandle(handle);
		if(!isRunning) smUnregisterService(service);
		return isRunning;
	}
}
