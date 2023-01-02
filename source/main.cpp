#define TESLA_INIT_IMPL // If you have more than one file using the tesla header, only define this in the main one
#include <tesla.hpp>    // The Tesla Header
#include <dmntcht.h>
#include <dmntcht.os.horizon.c>
#include <splatoon3/splatoon3.cpp>
#include <splatoon2/splatoon2.cpp>

u8 SPLATOON3BID[0x20] = {0xED, 0xA0, 0xA1, 0xDB, 0xA0, 0xEA, 0x1C, 0x0C, 0x47, 0x54, 0xE7, 0x2C, 0xBA, 0xDF, 0x16, 0xE7, 0x74, 0xBD, 0x49, 0xA3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
u8 SPLATOON2BID[0x20] = {0x03, 0xDC, 0xAD, 0x95, 0xE1, 0xAC, 0x64, 0x30, 0x72, 0xF3, 0xA6, 0x84, 0xF6, 0x74, 0x0C, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};




DmntCheatProcessMetadata metadata;

static bool initialized=false;



bool compareBuildID(u8 detectedBID[0x20], u8 BIDofSplatoon[0x10]);

bool debugService_isRunning();



class GuiTest : public tsl::Gui {
public:
    GuiTest() {	}	

    // Called when this Gui gets loaded to create the UI
    // Allocate all elements on the heap. libtesla will make sure to clean them up when not needed anymore
    virtual tsl::elm::Element* createUI() override {
        // A OverlayFrame is the base element every overlay consists of. This will draw the default Title and Subtitle.
        // If you need more information in the header or want to change it's look, use a HeaderOverlayFrame.
        auto frame = new tsl::elm::OverlayFrame("OVLsplatter", "v2.0.2");

        // A list that can contain sub elements and handles scrolling
        auto list = new tsl::elm::List();

        // List Items
        list->addItem(new tsl::elm::CategoryHeader("This game is not supported!!!"));
		
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



class OverlayTest : public tsl::Overlay {
public:
                                             // libtesla already initialized fs, hid, pl, pmdmnt, hid:sys, set:sys and the SD card
    virtual void initServices() override
    {
        if (debugService_isRunning())
        {
            dmntchtInitialize();
            dmntchtForceOpenCheatProcess();
            dmntchtGetCheatProcessMetadata(&metadata);

            initialized=true;
        }
    }  // Called at the start to initialize all services necessary for this Overlay
    virtual void exitServices() override
    {
        dmntchtExit();
        initialized=false;
    }  // Called at the end to clean up all services previously initialized

    virtual void onShow() override {}    // Called before overlay wants to change from invisible to visible state
    virtual void onHide() override {}    // Called before overlay wants to change from visible to invisible state
	
    virtual std::unique_ptr<tsl::Gui> loadInitialGui() override {
		
		if (compareBuildID(metadata.main_nso_build_id, SPLATOON3BID))
			return initially<Splatoon3::GuiTest3>();  // Initial Gui to load. It's possible to pass arguments to it's constructor like this
		
		if (compareBuildID(metadata.main_nso_build_id, SPLATOON2BID))
			return initially<Splatoon2::GuiTest2>();  // Initial Gui to load. It's possible to pass arguments to it's constructor like this
		
		
		return initially<GuiTest>();  // Initial Gui to load. It's possible to pass arguments to it's constructor like this
    }
};





int main(int argc, char **argv) {
	return tsl::loop<OverlayTest>(argc, argv);
}



bool compareBuildID(u8 detectedBID[0x20], u8 BIDofSplatoon[0x20]){
	
	bool res = true;
	for (int i = 0; i < 0x20; i ++)
		res &= detectedBID[i] == BIDofSplatoon[i];
	return res;
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
