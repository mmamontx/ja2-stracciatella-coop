#pragma once

#include "GraphicModel.h"

#include <string_theory/string>

/*
 * RakNet headers included across the project introduce conflicts with
 * windows.h. The following line fixes this.
 */
#undef small

class InventoryGraphicsModel {
	public:
		InventoryGraphicsModel(GraphicModel small, GraphicModel big);

		GraphicModel small;
		GraphicModel big;

		static InventoryGraphicsModel deserialize(const JsonValue &json);
		JsonValue serialize() const;
};
