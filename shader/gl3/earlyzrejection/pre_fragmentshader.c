#version 330

uniform sampler2D diffuseTextureUnit;
uniform int diffuseTextureAvailable;
uniform int diffuseTextureMaskedTransparency;
uniform float diffuseTextureMaskedTransparencyThreshold;

// passed from geometry shader
in vec2 gsFragTextureUV;

void main() {
	// retrieve diffuse texture color value
	if (diffuseTextureAvailable == 1) {
		// fetch from texture
		vec4 diffuseTextureColor = texture(diffuseTextureUnit, gsFragTextureUV);
		// check if to handle diffuse texture masked transparency
		if (diffuseTextureMaskedTransparency == 1) {
			// discard if beeing transparent
			if (diffuseTextureColor.a < diffuseTextureMaskedTransparencyThreshold) discard;
		}
	}
}