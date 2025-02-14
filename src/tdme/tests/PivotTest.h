
#pragma once

#include <tdme/tdme.h>
#include <tdme/application/Application.h>
#include <tdme/engine/fwd-tdme.h>
#include <tdme/engine/model/fwd-tdme.h>
#include <tdme/engine/primitives/fwd-tdme.h>
#include <tdme/tests/fwd-tdme.h>
#include <tdme/utilities/ObjectDeleter.h>

using tdme::application::Application;
using tdme::engine::model::Model;
using tdme::engine::primitives::BoundingVolume;
using tdme::engine::Engine;
using tdme::utilities::ObjectDeleter;

/**
 * Engine with pivot test
 * @author andreas.drewke
 */
class tdme::tests::PivotTest final
	: public virtual Application
{
private:
	Engine* engine { nullptr };
	ObjectDeleter<Model> modelDeleter;
	ObjectDeleter<BoundingVolume> bvDeleter;

public:

	/**
	 * Main
	 * @param argc argument count
	 * @param argv argument values
	 */
	static void main(int argc, char** argv);

	/**
	 * Public constructor
	 */
	PivotTest();

	// overridden methods
	void display();
	void dispose();
	void initialize();
	void reshape(int32_t width, int32_t height);
};
