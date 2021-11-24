#include <tdme/engine/subsystems/lighting/LightingShaderPBRFoliageImplementation.h>

#include <string>

#include <tdme/engine/subsystems/lighting/LightingShaderPBRBaseImplementation.h>
#include <tdme/engine/subsystems/renderer/Renderer.h>
#include <tdme/engine/EntityShaderParameters.h>
#include <tdme/engine/ShaderParameter.h>
#include <tdme/engine/Engine.h>
#include <tdme/engine/Timing.h>
#include <tdme/os/filesystem/FileSystem.h>
#include <tdme/os/filesystem/FileSystemInterface.h>

using std::string;
using std::to_string;

using tdme::engine::subsystems::lighting::LightingShaderPBRBaseImplementation;
using tdme::engine::subsystems::lighting::LightingShaderPBRFoliageImplementation;
using tdme::engine::subsystems::renderer::Renderer;
using tdme::engine::EntityShaderParameters;
using tdme::engine::ShaderParameter;
using tdme::engine::Engine;
using tdme::engine::Timing;
using tdme::os::filesystem::FileSystem;
using tdme::os::filesystem::FileSystemInterface;

bool LightingShaderPBRFoliageImplementation::isSupported(Renderer* renderer) {
	return renderer->isPBRAvailable();
}

LightingShaderPBRFoliageImplementation::LightingShaderPBRFoliageImplementation(Renderer* renderer): LightingShaderPBRBaseImplementation(renderer)
{
}

const string LightingShaderPBRFoliageImplementation::getId() {
	return "pbr-foliage";
}

void LightingShaderPBRFoliageImplementation::initialize()
{
	auto shaderVersion = renderer->getShaderVersion();

	// lighting
	//	vertex shader
	vertexShaderId = renderer->loadShader(
		renderer->SHADER_VERTEX_SHADER,
		"shader/" + shaderVersion + "/lighting/pbr",
		"render_vertexshader.vert",
		"#define HAVE_FOLIAGE\n",
		FileSystem::getInstance()->getContentAsString(
			"shader/" + shaderVersion + "/functions",
			"create_rotation_matrix.inc.glsl"
		) +
		"\n\n" +
		FileSystem::getInstance()->getContentAsString(
			"shader/" + shaderVersion + "/functions",
			"create_translation_matrix.inc.glsl"
		) +
		"\n\n" +
		FileSystem::getInstance()->getContentAsString(
			"shader/" + shaderVersion + "/functions",
			"create_foliage_transform_matrix.inc.glsl"
		) + "\n\n"
	);
	if (vertexShaderId == 0) return;

	//	fragment shader
	fragmentShaderId = renderer->loadShader(
		renderer->SHADER_FRAGMENT_SHADER,
		"shader/" + shaderVersion + "/lighting/pbr",
		"render_fragmentshader.frag",
		"#define LIGHT_COUNT " + to_string(Engine::LIGHTS_MAX) + "\n#define USE_PUNCTUAL\n#define MATERIAL_METALLICROUGHNESS\n#define USE_IBL\n",
		FileSystem::getInstance()->getContentAsString(
			"shader/" + shaderVersion + "/functions/pbr",
			"tonemapping.inc.glsl"
		) +
		"\n\n" +
		FileSystem::getInstance()->getContentAsString(
			"shader/" + shaderVersion + "/functions/pbr",
			"textures.inc.glsl"
		) +
		"\n\n" +
		FileSystem::getInstance()->getContentAsString(
			"shader/" + shaderVersion + "/functions/pbr",
			"functions.inc.glsl"
		) +
		"\n\n" +
		FileSystem::getInstance()->getContentAsString(
			"shader/" + shaderVersion + "/functions/pbr",
			"pbr_lighting.inc.glsl"
		) +
		"\n\n"
	);
	if (fragmentShaderId == 0) return;

	// create, attach and link program
	programId = renderer->createProgram(renderer->PROGRAM_OBJECTS);
	renderer->attachShaderToProgram(programId, vertexShaderId);
	renderer->attachShaderToProgram(programId, fragmentShaderId);

	//
	LightingShaderPBRBaseImplementation::initialize();

	//
	if (initialized == false) return;

	//
	initialized = false;

	// uniforms
	uniformSpeed = renderer->getProgramUniformLocation(programId, "speed");
	if (uniformSpeed == -1) return;
	uniformTime = renderer->getProgramUniformLocation(programId, "time");
	if (uniformTime == -1) return;
	uniformAmplitudeDefault = renderer->getProgramUniformLocation(programId, "amplitudeDefault");
	if (uniformAmplitudeDefault == -1) return;
	uniformAmplitudeMax = renderer->getProgramUniformLocation(programId, "amplitudeMax");
	if (uniformAmplitudeMax == -1) return;

	//
	initialized = true;
}

void LightingShaderPBRFoliageImplementation::registerShader() {
	Engine::registerShader(
		Engine::ShaderType::SHADERTYPE_OBJECT3D,
		getId(),
		{
			{ "speed", ShaderParameter(1.0f) },
			{ "amplitudeDefault", ShaderParameter(0.0f) },
			{ "amplitudeMax", ShaderParameter(20.0f) }
		}
	);
}

void LightingShaderPBRFoliageImplementation::useProgram(Engine* engine, void* context)
{
	LightingShaderPBRBaseImplementation::useProgram(engine, context);

	// time
	if (uniformTime != -1) renderer->setProgramUniformFloat(context, uniformTime, static_cast<float>(engine->getTiming()->getTotalTime()) / 1000.0f);
}

void LightingShaderPBRFoliageImplementation::updateShaderParameters(Renderer* renderer, void* context) {
	auto& shaderParameters = renderer->getShaderParameters(context);
	if (uniformSpeed != -1) renderer->setProgramUniformFloat(context, uniformSpeed, shaderParameters.getShaderParameter("speed").getFloatValue());
	if (uniformAmplitudeDefault != -1) renderer->setProgramUniformFloat(context, uniformAmplitudeDefault, shaderParameters.getShaderParameter("amplitudeDefault").getFloatValue());
	if (uniformAmplitudeMax != -1) renderer->setProgramUniformFloat(context, uniformAmplitudeMax, shaderParameters.getShaderParameter("amplitudeMax").getFloatValue());
}