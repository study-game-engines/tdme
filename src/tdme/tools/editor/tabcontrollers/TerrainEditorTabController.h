#pragma once

#include <array>
#include <string>
#include <unordered_map>
#include <vector>

#include <tdme/tdme.h>
#include <tdme/engine/fileio/textures/fwd-tdme.h>
#include <tdme/engine/Transformations.h>
#include <tdme/gui/events/fwd-tdme.h>
#include <tdme/gui/events/GUIActionListener.h>
#include <tdme/gui/events/GUIActionListener.h>
#include <tdme/gui/events/GUIChangeListener.h>
#include <tdme/gui/nodes/fwd-tdme.h>
#include <tdme/tools/editor/misc/FileDialogPath.h>
#include <tdme/tools/editor/tabcontrollers/subcontrollers/fwd-tdme.h>
#include <tdme/tools/editor/tabcontrollers/TabController.h>
#include <tdme/tools/editor/tabviews/fwd-tdme.h>
#include <tdme/utilities/Terrain2.h>

#include <ext/tinyxml/tinyxml.h>

using std::array;
using std::string;
using std::unordered_map;
using std::vector;

using tdme::engine::fileio::textures::Texture;
using tdme::engine::Transformations;
using tdme::gui::events::GUIActionListener;
using tdme::gui::events::GUIActionListenerType;
using tdme::gui::events::GUIChangeListener;
using tdme::gui::nodes::GUIElementNode;
using tdme::gui::nodes::GUIParentNode;
using tdme::gui::nodes::GUIScreenNode;
using tdme::gui::nodes::GUITextNode;
using tdme::tools::editor::misc::FileDialogPath;
using tdme::tools::editor::misc::PopUps;
using tdme::tools::editor::tabcontrollers::subcontrollers::BasePropertiesSubController;
using tdme::tools::editor::tabcontrollers::TabController;
using tdme::tools::editor::tabviews::TerrainEditorTabView;
using tdme::utilities::Terrain2;

using tinyxml::TiXmlAttribute;
using tinyxml::TiXmlDocument;
using tinyxml::TiXmlElement;

/**
 * Terrain editor tab controller
 * @author Andreas Drewke
 * @version $Id$
 */
class tdme::tools::editor::tabcontrollers::TerrainEditorTabController final
	: public TabController
{

private:
	BasePropertiesSubController* basePropertiesSubController { nullptr };
	TerrainEditorTabView* view { nullptr };
	GUIScreenNode* screenNode { nullptr };
	PopUps* popUps { nullptr };
	FileDialogPath brushTexturePath;
	FileDialogPath brushPrototypePath;
	FileDialogPath terrainPath;

	vector<unordered_map<int, vector<Transformations>>> newFoliageMaps;
	unordered_set<int> recreateFoliagePartitions;

	bool haveCurrentTerrainBrushHeight { false };
	float currentTerrainBrushHeight { 0.0f };
	float currentTerrainBrushScale { 1.0f };
	float currentTerrainBrushStrength { 1.0f };
	string currentTerrainBrushTextureFileName;
	Texture* currentTerrainBrushTexture { nullptr };
	Texture* rampTerrainBrushTexture { nullptr };
	Terrain2::BrushOperation currentTerrainBrushOperation { Terrain2::BRUSHOPERATION_NONE };
	Terrain2::BrushOperation currentFoliageBrushOperation { Terrain2::BRUSHOPERATION_NONE };

	Terrain2::FoliageBrush foliageBrush = {
		.brushTexture = nullptr,
		.brushScale = 1.0f,
		.brushDensity = 1.0f
	};
	vector<Terrain2::FoliageBrushPrototype> foliageBrushPrototypes;

	array<string, 2> textureBrushApplyNodes = {
		"terrainbrush_size",
		"terrainbrush_strength"
	};

	array<string, 2> foliageBrushApplyNodes = {
		"foliagebrush_size",
		"foliagebrush_density"
	};

	array<string, 16> foliageBrushPrototypeApplyNodes = {
		"foliagebrush_prototype_object_count",
		"foliagebrush_prototype_normalalign",
		"foliagebrush_prototype_rotationrange_x_min",
		"foliagebrush_prototype_rotationrange_x_max",
		"foliagebrush_prototype_rotationrange_y_min",
		"foliagebrush_prototype_rotationrange_y_max",
		"foliagebrush_prototype_rotationrange_z_min",
		"foliagebrush_prototype_rotationrange_z_max",
		"foliagebrush_prototype_scalerange_min",
		"foliagebrush_prototype_scalerange_max",
		"foliagebrush_prototype_scalerange_max",
		"foliagebrush_prototype_heightrange_max",
		"foliagebrush_prototype_heightrange_min",
		"foliagebrush_prototype_heightrange_max",
		"foliagebrush_prototype_sloperange_max",
		"foliagebrush_prototype_sloperange_max"
	};

public:
	/**
	 * Public constructor
	 * @param view view
	 */
	TerrainEditorTabController(TerrainEditorTabView* view);

	/**
	 * Destructor
	 */
	virtual ~TerrainEditorTabController();

	/**
	 * Get view
	 */
	TerrainEditorTabView* getView();

	// overridden method
	GUIScreenNode* getScreenNode() override;

	/**
	 * @return current terrain brush operation
	 */
	inline Terrain2::BrushOperation getTerrainBrushOperation() {
		return currentTerrainBrushOperation;
	}

	/**
	 * @return current foliage brush operation
	 */
	inline Terrain2::BrushOperation getFoliageBrushOperation() {
		return currentFoliageBrushOperation;
	}

	// overridden methods
	void initialize(GUIScreenNode* screenNode) override;
	void dispose() override;
	void save() override;
	void saveAs() override;

	// overridden methods
	void onValueChanged(GUIElementNode* node) override;
	void onActionPerformed(GUIActionListenerType type, GUIElementNode* node) override;
	void onFocus(GUIElementNode* node) override;
	void onUnfocus(GUIElementNode* node) override;
	void onContextMenuRequested(GUIElementNode* node, int mouseX, int mouseY) override;

	/**
	 * Set outliner content
	 */
	void setOutlinerContent();

	/**
	 * Set outliner add drop down content
	 */
	void setOutlinerAddDropDownContent();

	/**
	 * On create terrain
	 */
	void onCreateTerrain();

	/**
	 * Set brush scale
	 * @param scale scale
	 */
	void setBrushScale(float scale);

	/**
	 * Set brush density/strength
	 * @param densityStrength density/strength
	 */
	void setBrushDensityStrength(float densityStrength);

	/**
	 * Set terrain details
	 */
	void setTerrainDetails();

	/**
	 * Set terrain brush details
	 */
	void setTerrainBrushDetails();

	/**
	 * Update terrain brush details
	 */
	void updateTerrainBrushDetails();

	/**
	 * Apply terrain brush details
	 */
	void applyTerrainBrushDetails();

	/**
	 * Set foliage brush details
	 */
	void setFoliageBrushDetails();

	/**
	 * Apply foliage brush details
	 */
	void applyFoliageBrushDetails();

	/**
	 * Set foliage brush details
	 */
	void setFoliageBrushPrototypeDetails();

	/**
	 * Apply foliage brush prototype details
	 */
	void applyFoliageBrushPrototypeDetails();

	/**
	 * Check if outliner selection is foliage brush prototype
	 * @param outlinerNode outliner node
	 * @param foliageBrushIdx foliage brush index
	 * @param foliageBrushPrototypeIdx foliage brush prototype index
	 */
	bool checkOutlinerFoliageBrushPrototype(const string& outlinerNode, int& foliageBrushIdx, int& foliageBrushPrototypeIdx);

	/**
	 * Update details
	 * @param outlinerNode outliner node
	 */
	void updateDetails(const string& outlinerNode);

	/**
	 * Initialize terrain
	 */
	void initializeTerrain();

	/**
	 * Apply current brush to terrain at given brush center position
	 * @param terrainBoundingBox terrain bounding box
	 * @param terrainModels terrain models
	 * @param brushCenterPosition brush center position
	 * @param deltaTime delta time between last frame and this frame
	 */
	void applyTerrainBrush(BoundingBox& terrainBoundingBox, vector<Model*>& terrainModels, const Vector3& brushCenterPosition, int64_t deltaTime);

	/**
	 * Determine current brush flatten height
	 * @param terrainBoundingBox terrain bounding box
	 * @param terrainModels terrain models
	 * @param brushCenterPosition brush center position
	 * @return success
	 */
	bool determineCurrentBrushHeight(BoundingBox& terrainBoundingBox, vector<Model*> terrainModels, const Vector3& brushCenterPosition);

	/**
	 * Determine ramp height
	 * @param terrainBoundingBox terrain bounding box
	 * @param terrainModels terrain models
	 * @param position position
	 * @param height height
	 * @return success
	 */
	bool determineRampHeight(BoundingBox& terrainBoundingBox, vector<Model*> terrainModels, const Vector3& position, float& height);

	/**
	 * Unset current brush flatten height
	 */
	void unsetCurrentBrushFlattenHeight();

	/**
	 * Apply current brush to terrain at given brush center position
	 * @param terrainBoundingBox terrain bounding box
	 * @param terrainModels terrain models
	 * @param position position
	 * @param rotation rotation
	 * @param scale scale
	 * @param minHeight min height
	 * @param maxHeigth max heigth
	 */
	void applyRampTerrainBrush(BoundingBox& terrainBoundingBox, vector<Model*>& terrainModels, const Vector3& position, float rotation, const Vector2& scale, float minHeight, float maxHeight);

	/**
	 * Create water using a auto fill like algorithm
	 * @param terrainBoundingBox terrain bounding box
	 * @param brushCenterPosition brush center position
	 * @param waterModels water models
	 * @param waterReflectionEnvironmentMappingPosition water reflection environment mapping position
	 */
	void createWater(BoundingBox& terrainBoundingBox, const Vector3& brushCenterPosition, vector<Model*>& waterModels, Vector3& waterReflectionEnvironmentMappingPosition);

	/**
	 * Delete water models
	 * @param waterPositionMapIdx water position map index
	 */
	void deleteWater(int waterPositionMapIdx);

	/**
	 * Set foliage brush
	 */
	void setFoliageBrush();

	/**
	 * Update foliage brush structs
	 * @param foliageBrush foliage brush
	 * @param foliageBrushPrototypes foliage brush prototypes
	 */
	void updateFoliageBrushStructs();

	/**
	 * Apply current brush to foliage at given brush center position
	 * @param terrainBoundingBox terrain bounding box
	 * @param brushCenterPosition brush center position
	 * @param deltaTime delta time between last frame and this frame
	 */
	void applyFoliageBrush(BoundingBox& terrainBoundingBox, const Vector3& brushCenterPosition, int64_t deltaTime);

	/**
	 * Shows the error pop up
	 * @param caption caption
	 * @param message message
	 */
	void showErrorPopUp(const string& caption, const string& message);

};