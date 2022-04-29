#include <tdme/engine/subsystems/decals/DecalObjectInternal.h>

#include <string>
#include <vector>

#include <tdme/tdme.h>
#include <tdme/engine/fileio/textures/Texture.h>
#include <tdme/engine/fileio/textures/TextureReader.h>
#include <tdme/engine/model/Color4.h>
#include <tdme/engine/primitives/BoundingBox.h>
#include <tdme/engine/primitives/OrientedBoundingBox.h>
#include <tdme/engine/Transformations.h>

using std::string;
using std::vector;

using tdme::engine::fileio::textures::Texture;
using tdme::engine::fileio::textures::TextureReader;
using tdme::engine::model::Color4;
using tdme::engine::primitives::BoundingBox;
using tdme::engine::primitives::OrientedBoundingBox;
using tdme::engine::subsystems::decals::DecalObjectInternal;
using tdme::engine::Transformations;

DecalObjectInternal::DecalObjectInternal(const string& id, OrientedBoundingBox* obb, Texture* texture)
{
	this->id = id;
	this->enabled = true;
	this->effectColorMul.set(1.0f, 1.0f, 1.0f, 1.0f);
	this->effectColorAdd.set(0.0f, 0.0f, 0.0f, 0.0f);
	this->pickable = false;
	this->contributesShadows = false;
	this->receivesShadows = false;
	this->obb = obb;
	this->texture = texture;
	boundingBox = BoundingBox(obb);
	updateBoundingBox();
}

DecalObjectInternal::~DecalObjectInternal() {
}

void DecalObjectInternal::update()
{
	Transformations::update();
	updateBoundingBox();
}

void DecalObjectInternal::fromTransformations(const Transformations& transformations)
{
	Transformations::fromTransformations(transformations);
	updateBoundingBox();
}

void DecalObjectInternal::initialize() {
	texture = TextureReader::read("resources/engine/textures", "point.png");
}

void DecalObjectInternal::dispose()
{
	if (texture != nullptr) {
		texture->releaseReference();
		texture = nullptr;
	}
}