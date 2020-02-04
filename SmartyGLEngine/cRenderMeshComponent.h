#ifndef _cRenderMeshComponent_HG_
#define _cRenderMeshComponent_HG_

#include <vector>
#include <string>

#include "Global.h"
#include "cComponent.h"
#include "sTextureInfo.h"

#include <rapidjson/prettywriter.h>

class cRenderMeshComponent : public cComponent {
public:
	int shaderId;
	std::string meshName;
	bool bIsWireFrame;	
	bool bIsVisible;
	glm::vec4 objColour;
	glm::vec4 specular;

	bool bUseVertexColour;		// = false (default)
	std::vector<cRenderMeshComponent*> vec_pChildObjectsToDraw;
	std::string friendlyName;		// Human "Bob" "Ali" "Player"
	bool bDontLight;
	bool colorByHeight;
	bool use_offscreen_texture = false;
	std::vector<sTextureInfo> textures;
	bool is_in_world_fbo = false;
	bool render_target_overlay = false;

	bool bAddReflect = false;
	bool bAddRefract = false;
	bool use_discard_texture = false;
	bool use_stencil_buffer = false;
	bool bUseSplatMap = false;
	bool bUseWaterHeightMaps = false;

	cRenderMeshComponent() : cComponent(3), specular(glm::vec4(0.0f,0.0f,0.0f, 1.0f)), colorByHeight(false){}

	void serializeComponent(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) {
		//Start Component
		writer.StartObject();

		writer.String("componentType");
		writer.Int(this->COMPONENT_TYPE_ID);

		writer.String("meshName");
		writer.String(this->meshName.c_str());

		writer.String("isWireframe");
		writer.Bool(this->bIsWireFrame);

		writer.String("isVisible");
		writer.Bool(this->bIsVisible);

		// Colour
		writer.String(("color"));
		writer.StartObject();

		writer.String("r");
		writer.Double(this->objColour.x);
		writer.String("g");
		writer.Double(this->objColour.y);
		writer.String("b");
		writer.Double(this->objColour.z);
		writer.String("a");
		writer.Double(this->objColour.w);
		writer.EndObject();

		writer.String("textures");
		writer.StartArray();

		for (size_t i = 0; i < this->textures.size(); ++i) {
			writer.StartObject();
			
			writer.String("name");
			writer.String(this->textures[i].name.c_str());

			writer.String("strength");
			writer.Double(this->textures[i].strength);

			writer.EndObject();
		}

		writer.EndArray();

		// Specular
		writer.String("specular");
		writer.StartObject();
		writer.String("r");
		writer.Double(this->specular.x);
		writer.String("g");
		writer.Double(this->specular.y);
		writer.String("b");
		writer.Double(this->specular.z);
		writer.String("shinyness");
		writer.Double(this->specular.w);
		writer.EndObject();

		writer.String("useVertexColor");
		writer.Bool(this->bUseVertexColour);

		//writer.String("FriendlyName");
		//writer.String(this->friendlyName.c_str());

		writer.String("dontLight");
		writer.Bool(this->bDontLight);

		writer.EndObject();  // End Component
	}
};

#endif
