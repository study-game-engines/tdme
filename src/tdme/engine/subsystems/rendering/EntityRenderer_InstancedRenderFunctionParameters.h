#pragma once

#include <string>

#include <tdme/tdme.h>
#include <tdme/engine/fwd-tdme.h>
#include <tdme/engine/subsystems/rendering/fwd-tdme.h>

using std::string;

using tdme::engine::Object3D;

/** 
 * Entity renderer parameters
 * @author Andreas Drewke
 * @version $Id$
 */
struct tdme::engine::subsystems::rendering::EntityRenderer_InstancedRenderFunctionParameters final {
	vector<Object3D*> objects;
	uint32_t renderTypes;
	bool collectTransparentFaces;
};