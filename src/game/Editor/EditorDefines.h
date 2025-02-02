#ifndef __EDITORDEFINES_H
#define __EDITORDEFINES_H

#include "Button_System.h"
#include "Types.h"
#include "Button_Defines.h"

enum ToolbarMode
{
	TBAR_MODE_NONE,
	TBAR_MODE_DRAW,
	TBAR_MODE_ERASE,
	TBAR_MODE_UNDO,
	TBAR_MODE_EXIT_EDIT,
	TBAR_MODE_QUIT_GAME,
	TBAR_MODE_NEW_MAP,
	TBAR_MODE_SAVE,
	TBAR_MODE_LOAD,
	TBAR_MODE_RADARMAP,
	TBAR_MODE_SET_BGRND,
	TBAR_MODE_CHANGE_BRUSH,
	TBAR_MODE_ERASE_OFF,
	TBAR_MODE_FILL_AREA,
	TBAR_MODE_FILL_AREA_OFF,

	TBAR_MODE_DRAW_DEBRIS,
	TBAR_MODE_DRAW_BANKS,
	TBAR_MODE_DRAW_MERC,
	TBAR_MODE_DRAW_NPC1,
	TBAR_MODE_DRAW_NPC2,
	TBAR_MODE_DRAW_NPC3,
	TBAR_MODE_DRAW_NPC4,
	TBAR_MODE_DRAW_OSTRUCTS,
	TBAR_MODE_DRAW_OSTRUCTS1,
	TBAR_MODE_DRAW_OSTRUCTS2,

	TBAR_MODE_GET_WALL,
	TBAR_MODE_GET_DOOR,
	TBAR_MODE_GET_WINDOW,
	TBAR_MODE_GET_ROOF,
	TBAR_MODE_GET_BROKEN_WALL,
	TBAR_MODE_GET_DECOR,
	TBAR_MODE_GET_DECAL,
	TBAR_MODE_GET_FLOOR,
	TBAR_MODE_GET_TOILET,
	TBAR_MODE_GET_OSTRUCTS,
	TBAR_MODE_GET_OSTRUCTS1,
	TBAR_MODE_GET_OSTRUCTS2,
	TBAR_MODE_GET_FGRND_TILE,
	TBAR_MODE_GET_BGRND_TILE,
	TBAR_MODE_GET_DEBRIS,
	TBAR_MODE_GET_BANKS,
	TBAR_MODE_GET_ROADS,
	TBAR_MODE_GET_ROOM,
	TBAR_MODE_GET_NEW_ROOF,
	TBAR_MODE_GET_TILE_TO_ROOM,

	TBAR_MODE_CHANGE_TILESET,
	TBAR_MODE_CIVILIAN_GROUP,
	TBAR_MODE_FAKE_LIGHTING,
	TBAR_MODE_LIGHT_UP,
	TBAR_MODE_LIGHT_DWN,

	TBAR_MODE_RAISE_LAND,
	TBAR_MODE_LOWER_LAND,
	TBAR_MODE_DENS_UP,
	TBAR_MODE_DENS_DWN,

	TBAR_MODE_MAKE_NEW_ROOM,

	TBAR_MODE_ITEM_WEAPONS,
	TBAR_MODE_ITEM_AMMO,
	TBAR_MODE_ITEM_ARMOUR,
	TBAR_MODE_ITEM_EXPLOSIVES,
	TBAR_MODE_ITEM_EQUIPMENT1,
	TBAR_MODE_ITEM_EQUIPMENT2,
	TBAR_MODE_ITEM_EQUIPMENT3,
	TBAR_MODE_ITEM_TRIGGERS,
	TBAR_MODE_ITEM_KEYS,
};

enum TaskMode
{
	TASK_NONE,
	TASK_TERRAIN,
	TASK_BUILDINGS,
	TASK_ITEMS,
	TASK_MERCS,
	TASK_MAPINFO,
	TASK_OPTIONS
};

enum EditorActions
{
	ACTION_NULL,
	ACTION_SUB_INDEX_UP,
	ACTION_SUB_INDEX_DWN,
	ACTION_SET_NEW_BACKGROUND,
	ACTION_EXIT_EDITOR,
	ACTION_QUIT_GAME,
	ACTION_NEXT_FGRND,
	ACTION_SWITCH_SCRN,
	ACTION_NEXT_LEVEL,
	ACTION_NEXT_WALL,
	ACTION_FILL_AREA,
	ACTION_NEXT_DEBRIS,
	ACTION_NEXT_DECAL,
	ACTION_CHANGE_ELEV,
	ACTION_PREV_SELECTIONTYPE,
	ACTION_NEXT_SELECTIONTYPE,
	ACTION_SET_FNAME,
	ACTION_SAVE_MAP,
	ACTION_LOAD_MAP,
	ACTION_RADARMAP,
	ACTION_UNDO,
	ACTION_NEXT_BANK,
	ACTION_NEXT_ROAD,
	ACTION_NEXT_CLIFF,
	ACTION_SHADE_UP,
	ACTION_SHADE_DWN,
	ACTION_WALL_PASTE1,
	ACTION_WALL_PASTE2,
	ACTION_NEXT_STRUCT,

	ACTION_GET_ROOM,
	ACTION_NEW_MAP,

	//using the selection method for getting objects for buildings
	ACTION_GET_WALL,
	ACTION_GET_DOOR,
	ACTION_GET_WINDOW,
	ACTION_GET_ROOF,
	ACTION_GET_BROKEN_WALL,
	ACTION_GET_DECOR,
	ACTION_GET_DECAL,
	ACTION_GET_FLOOR,
	ACTION_GET_TOILET,
	ACTION_GET_NEW_ROOF,

	ACTION_SET_MERC,
	ACTION_QUICK_ERASE,
	ACTION_HELPSCREEN,
	ACTION_NEXT_STRUCT1,
	ACTION_NEXT_STRUCT2,
	ACTION_DRAW_BADMERC,
	ACTION_EDIT_BADMERC,
	ACTION_DRAW_GOODMERC,
	ACTION_EDIT_GOODMERC,
	ACTION_RAISE_LAND,
	ACTION_LOWER_LAND,
	ACTION_SET_WAYPOINT,
	ACTION_ERASE_WAYPOINT,
	ACTION_DENSITY_UP,
	ACTION_DENSITY_DOWN,

	ACTION_COPY_MERC_PLACEMENT,
	ACTION_PASTE_MERC_PLACEMENT,
};

enum
{
	DRAW_MODE_NOTHING,
	//Terrain
	DRAW_MODE_GROUND,
	DRAW_MODE_NEW_GROUND,
	DRAW_MODE_DEBRIS,
	DRAW_MODE_BANKS,
	DRAW_MODE_ROADS,
	DRAW_MODE_OSTRUCTS,
	DRAW_MODE_OSTRUCTS1,
	DRAW_MODE_OSTRUCTS2,
	//Selection method for buildings
	DRAW_MODE_WALLS,
	DRAW_MODE_DOORS,
	DRAW_MODE_WINDOWS,
	DRAW_MODE_ROOFS,
	DRAW_MODE_NEWROOF,
	DRAW_MODE_BROKEN_WALLS,
	DRAW_MODE_DECOR,
	DRAW_MODE_DECALS,
	DRAW_MODE_FLOORS,
	//Smart method for buildings
	DRAW_MODE_SMART_WALLS,
	DRAW_MODE_SMART_WINDOWS,
	DRAW_MODE_SMART_DOORS,
	DRAW_MODE_SMART_BROKEN_WALLS,
	DRAW_MODE_DOORKEYS,
	//Other stuff
	DRAW_MODE_TOILET,
	DRAW_MODE_ROOM,
	DRAW_MODE_SAW_ROOM,
	DRAW_MODE_ROOMNUM,
	DRAW_MODE_CAVES,
	DRAW_MODE_SLANTED_ROOF,
	DRAW_MODE_KILL_BUILDING,
	DRAW_MODE_COPY_BUILDING,
	DRAW_MODE_MOVE_BUILDING,
	DRAW_MODE_SHOW_TILESET,
	DRAW_MODE_LIGHT,
	DRAW_MODE_PLAYER,
	DRAW_MODE_ENEMY,
	DRAW_MODE_CREATURE,
	DRAW_MODE_REBEL,
	DRAW_MODE_CIVILIAN,

	DRAW_MODE_SCHEDULEACTION,
	DRAW_MODE_SCHEDULECONFIRM,

	DRAW_MODE_EXITGRID,
	DRAW_MODE_EXITGRID_QUEUE,
	DRAW_MODE_NORTHPOINT,
	DRAW_MODE_WESTPOINT,
	DRAW_MODE_EASTPOINT,
	DRAW_MODE_SOUTHPOINT,
	DRAW_MODE_CENTERPOINT,
	DRAW_MODE_ISOLATEDPOINT,

	DRAW_MODE_PLACE_ITEM,
	DRAW_MODE_SELECT_BRUSH = 200,
	DRAW_MODE_FILL_AREA,
	DRAW_MODE_UNDO,
	DRAW_MODE_ERASE = 1000
};

extern GUIButtonRef iEditorButton[NUMBER_EDITOR_BUTTONS];
extern ToolbarMode  iEditorToolbarState;
extern INT32 iDrawMode;
extern INT32 iCurrentAction;
extern INT32 iActionParam;

extern UINT16	gusLightLevel;
extern UINT16 gusLightLevelPercentage;
extern INT16 gsLightRadius;

extern BOOLEAN gfRenderWorld;
extern BOOLEAN gfRenderTaskbar;
extern BOOLEAN gfShowTerrainTileButtons;
extern BOOLEAN gfRenderDrawingMode;

extern BOOLEAN gfPendingBasement;
extern BOOLEAN gfPendingCaves;

//When in merc-inventory mode, this flag is set when the user wishes to get an item, which requires hooking
//into the item editing features.  It is defined here, so editoritems can get access to this mode which makes
//certain special cases.
extern BOOLEAN gfMercGetItem;

//When actually editing item statistics, this value will be set, for the purposes of rendering, input, etc.
extern BOOLEAN gfShowItemStatsPanel;

#define TASKBAR_Y	(EDITOR_TASKBAR_POS_Y)

#define MERCPANEL_X	177
#define MERCPANEL_Y	(EDITOR_TASKBAR_POS_Y + 1)

#endif
