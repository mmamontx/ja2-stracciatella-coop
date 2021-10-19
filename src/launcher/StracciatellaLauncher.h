// generated by Fast Light User Interface Designer (fluid) version 1.0307

#ifndef StracciatellaLauncher_h
#define StracciatellaLauncher_h
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Browser.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Check_Button.H>

class StracciatellaLauncher {
public:
  StracciatellaLauncher();
  Fl_Double_Window *stracciatellaLauncher;
  Fl_Button *editorButton;
  Fl_Button *playButton;
  Fl_Input *gameDirectoryInput;
  Fl_Button *browseJa2DirectoryButton;
  Fl_Choice *gameVersionInput;
  Fl_Button *guessVersionButton;
  Fl_Browser *enabledModsBrowser;
  Fl_Button *enableModsButton;
  Fl_Button *disableModsButton;
  Fl_Button *moveUpModsButton;
  Fl_Button *moveDownModsButton;
  Fl_Browser *availableModsBrowser;
  Fl_Text_Display *modDetails;
  Fl_Output *gameSettingsOutput;
  Fl_Choice *scalingModeChoice;
  Fl_Value_Input *resolutionXInput;
  Fl_Value_Input *resolutionYInput;
  Fl_Menu_Button *predefinedResolutionMenuButton;
  Fl_Box *invalidResolutionLabel;
  Fl_Check_Button *fullscreenCheckbox;
  Fl_Check_Button *playSoundsCheckbox;
  Fl_Output *ja2JsonPathOutput;
  Fl_Button *ja2JsonReloadBtn;
  Fl_Button *ja2JsonSaveBtn;
};
#endif
