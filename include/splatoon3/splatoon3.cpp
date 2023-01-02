#define TESLA_INIT_IMPL // If you have more than one file using the tesla header, only define this in the main one
#include <tesla.hpp>    // The Tesla Header

extern DmntCheatProcessMetadata metadata;


namespace Splatoon3 {
	
	enum {RED, GREEN, BLUE};
	enum {ALPHA, BRAVO, CHARLIE, NEUTRAL};



	tsl::elm::TrackBar* bar[3] = {0, 0, 0};
	int Team = 0;


	void applyColor(float *color, int team);

	bool debugService_isRunning();

	
	
	
	

	class GuiMisc : public tsl::Gui {
	public:
		GuiMisc() {}

		virtual tsl::elm::Element* createUI() override {
			
			auto *rootFrame = new tsl::elm::OverlayFrame("OVLsplatter", "Miscellaneous cheats");

			auto list = new tsl::elm::List();
			
			u32 flag;

			//First read the memory address to know if displaying on or off on the button
			dmntchtReadCheatProcessMemory(metadata.main_nso_extents.base + 0x05CE4428, &flag, sizeof(flag));
			//Then create the button
			auto *offlineShops = new tsl::elm::ToggleListItem("Offline shops and murch", flag);
			//If the button is clicked...
			offlineShops->setClickListener([offlineShops](u64 keys) {
				if (keys & HidNpadButton_A) {
					
					
					//Get the state of the button, place it in the address and freeze it
					bool offlineShopsCheat = offlineShops->getState();
					dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + 0x05CE4428, &offlineShopsCheat, sizeof(offlineShopsCheat));
					dmntchtEnableFrozenAddress(metadata.main_nso_extents.base + 0x05CE4428, sizeof(int), 0);

					return true;
				}

				return false;
			});
			
			list->addItem(offlineShops);


			
			
			auto *abilityChunks = new tsl::elm::ListItem("99 of all the ability chunks");
			//If the button is clicked...
			abilityChunks->setClickListener([](u64 keys) {
				if (keys & HidNpadButton_A) {
					
					int value = 99;
					
					u64 p;
					
					dmntchtReadCheatProcessMemory(metadata.main_nso_extents.base + 0x5DBBD88, &p, sizeof(p));
					dmntchtReadCheatProcessMemory(p + 0x510, &p, sizeof(p));
					p += 0x478;
					
					for (u64 offset = p; offset < p + 14 * 4; offset += 4){
						dmntchtWriteCheatProcessMemory(offset, &value, sizeof(value));
						dmntchtEnableFrozenAddress(offset, sizeof(value), 0);
					}
					return true;
				}

				return false;
			});
			
			list->addItem(abilityChunks);
			
			
			
			//Create the button
			auto *superSeaSnails = new tsl::elm::ListItem("Refill super sea snails");
			//If the button is clicked...
			superSeaSnails->setClickListener([](u64 keys) {
				if (keys & HidNpadButton_A) {
					
					int value = 999;
					
					dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xBE2331A0, &value, sizeof(value));

					return true;
				}

				return false;
			});
			
			list->addItem(superSeaSnails);
			
			
			
			//Create the button
			auto *money = new tsl::elm::ListItem("Refill money");
			//If the button is clicked...
			money->setClickListener([](u64 keys) {
				if (keys & HidNpadButton_A) {
					
					int value = 9999999;
					
					dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xBE23319C, &value, sizeof(value));

					return true;
				}

				return false;
			});
			
			list->addItem(money);
			
			
			
			//Create the button
			auto *licenses = new tsl::elm::ListItem("Refill Sheldon licences");
			//If the button is clicked...
			licenses->setClickListener([](u64 keys) {
				if (keys & HidNpadButton_A) {
					
					int value = 999;
					
					dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + 0xBE2331BC, &value, sizeof(value));

					return true;
				}

				return false;
			});
			
			list->addItem(licenses);
			
			
			
			//Create the button
			auto *die = new tsl::elm::ListItem("Don't click me!!");
			//If the button is clicked...
			die->setClickListener([](u64 keys) {
				if (keys & HidNpadButton_A) {
					
					int value = 0;
					
					u64 p;
					
					dmntchtReadCheatProcessMemory(metadata.main_nso_extents.base + 0x5bf4990, &p, sizeof(p));
					dmntchtReadCheatProcessMemory(p + 0x50, &p, sizeof(p));
					dmntchtReadCheatProcessMemory(p + 0x88, &p, sizeof(p));
					dmntchtWriteCheatProcessMemory(p +0x6C, &value, sizeof(value));

					return true;
				}

				return false;
			});
			
			list->addItem(die);
			

			// Add the list to the frame for it to be drawn
			rootFrame->setContent(list);

			
			// Return the frame to have it become the top level element of this Gui
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
			
			

			auto *CharlieTeam = new tsl::elm::ListItem("Charlie team", "...");
			CharlieTeam->setClickListener([](u64 keys) {
				if (keys & HidNpadButton_A) {
					Team = CHARLIE;
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
			list->addItem(CharlieTeam);
			list->addItem(NeutralTeam);
			
			
			rootFrame->setContent(list);


			return rootFrame;
		}
	};

	


	class GuiIPS : public tsl::Gui {
	public:
		GuiIPS() {}

		virtual tsl::elm::Element* createUI() override {
			
			auto *rootFrame = new tsl::elm::OverlayFrame("OVLsplatter", "public IPS patches");

			auto list = new tsl::elm::List();
			
			bool flag;
			u32 instruction;
			
			
			dmntchtReadCheatProcessMemory(metadata.main_nso_extents.base + 0x0364EB98, &instruction, sizeof(instruction)); 
			flag = instruction == 0x52803208;
			
			auto *allowHazard = new tsl::elm::ToggleListItem("Allow Hazard Level 200+ (Shoal) [Coxxs]", flag);
			allowHazard->setClickListener([flag](u64 keys) {
				if (keys & HidNpadButton_A) {
					
					if (flag)
					{
						u32 instruction = 0xb9403f48;
						dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + 0x364eb98, &instruction, sizeof(instruction));
					}
					else
					{
						u32 instruction = 0x52803208;
						dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + 0x0364EB98, &instruction, sizeof(instruction)); 
					}
					
					return true;
				}

				return false;
			});
			
			list->addItem(allowHazard);
			
			
			
			
			
			
			dmntchtReadCheatProcessMemory(metadata.main_nso_extents.base + 0x02CB4AA4, &instruction, sizeof(instruction)); 
			flag = instruction == 0xD503201F;
			
			auto *offlineSave = new tsl::elm::ToggleListItem("Offline save [Coxxs]", flag);
			offlineSave->setClickListener([flag](u64 keys) {
				if (keys & HidNpadButton_A) {
					
					if (flag)
					{
						u32 instruction = 0x36000440;
						dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + 0x2cb4aa4, &instruction, sizeof(instruction));
						instruction = 0x36000340;
						dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + 0x2cb4ac4, &instruction, sizeof(instruction));
						instruction = 0x540002e1;
						dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + 0x2cb4ad0, &instruction, sizeof(instruction));
						instruction = 0x885f7d09;
						dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + 0x2cb4adc, &instruction, sizeof(instruction));
						instruction = 0x32160129;
						dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + 0x2cb4ae0, &instruction, sizeof(instruction));
						instruction = 0x880a7d09;
						dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + 0x2cb4ae4, &instruction, sizeof(instruction));
						instruction = 0x35ffffaa;
						dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + 0x2cb4ae8, &instruction, sizeof(instruction));
					}
					else
					{
						u32 instruction = 0xD503201F;
						dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + 0x02CB4AA4, &instruction, sizeof(instruction)); 
						dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + 0x02CB4AC4, &instruction, sizeof(instruction));
						dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + 0x02CB4AD0, &instruction, sizeof(instruction));
						dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + 0x02CB4ADC, &instruction, sizeof(instruction));
						dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + 0x02CB4AE0, &instruction, sizeof(instruction));
						dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + 0x02CB4AE4, &instruction, sizeof(instruction));
						dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + 0x02CB4AE8, &instruction, sizeof(instruction));
					}
					
					return true;
				}

				return false;
			});
			
			list->addItem(offlineSave);
			
			
			
			
			
			
			dmntchtReadCheatProcessMemory(metadata.main_nso_extents.base + 0x010E6CA0, &instruction, sizeof(instruction)); 
			flag = instruction == 0xD2800020;
			
			auto *sunset = new tsl::elm::ToggleListItem("Sunset [Coxxs]", flag);
			sunset->setClickListener([flag](u64 keys) {
				if (keys & HidNpadButton_A) {
					
					if (flag)
					{
						u32 instruction = 0xa9bb7bfd;
						dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + 0x10e6ca0, &instruction, sizeof(instruction));
						instruction = 0xf9000bf9;
						dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + 0x10e6ca4, &instruction, sizeof(instruction));
					}
					else
					{
						u32 instruction = 0xD2800020;
						dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + 0x010E6CA0, &instruction, sizeof(instruction)); 
						instruction = 0xD65F03C0;
						dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + 0x010E6CA4, &instruction, sizeof(instruction));
					}
					
					return true;
				}

				return false;
			});
			
			list->addItem(sunset);
			
			
			
			
			
			
			dmntchtReadCheatProcessMemory(metadata.main_nso_extents.base + 0x010E6CA0, &instruction, sizeof(instruction)); 
			flag = instruction == 0xD2800040;
			
			auto *night = new tsl::elm::ToggleListItem("Night [Coxxs]", flag);
			night->setClickListener([flag](u64 keys) {
				if (keys & HidNpadButton_A) {
					
					if (flag)
					{
						u32 instruction = 0xa9bb7bfd;
						dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + 0x10e6ca0, &instruction, sizeof(instruction));
						instruction = 0xf9000bf9;
						dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + 0x10e6ca4, &instruction, sizeof(instruction));
					}
					else
					{
						u32 instruction = 0xD2800040;
						dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + 0x010E6CA0, &instruction, sizeof(instruction)); 
						instruction = 0xD65F03C0;
						dmntchtWriteCheatProcessMemory(metadata.main_nso_extents.base + 0x010E6CA4, &instruction, sizeof(instruction));
					}
					
					return true;
				}

				return false;
			});
			
			list->addItem(night);
			
			rootFrame->setContent(list);


			return rootFrame;
		}
	};

	class GuiTest3 : public tsl::Gui {
	public:
		GuiTest3() {	}	

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
			
			auto *miscellaneousCheatsItem = new tsl::elm::ListItem("Miscellaneous cheats", "...");
			miscellaneousCheatsItem->setClickListener([](u64 keys) {
				if (keys & HidNpadButton_A) {
					tsl::changeTo<GuiMisc>();
					return true;
				}

				return false;
			});
			
			auto *IPSpatches = new tsl::elm::ListItem("Public IPS patches", "...");
			IPSpatches->setClickListener([](u64 keys) {
				if (keys & HidNpadButton_A) {
					tsl::changeTo<GuiIPS>();
					return true;
				}

				return false;
			});

			list->addItem(changeColorsItem);
			list->addItem(miscellaneousCheatsItem);
			list->addItem(IPSpatches);

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
		for (unsigned int offset = 0x008FCFE418; offset <= 0x008FD00098; offset += 0xC0)
			dmntchtWriteCheatProcessMemory(metadata.heap_extents.base + offset + 0x28 * team, color, sizeof(float) * 3);
		
		tsl::goBack();
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
