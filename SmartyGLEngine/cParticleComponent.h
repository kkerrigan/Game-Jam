#ifndef _cParticleComponent_HG_
#define _cParticleComponent_HG_

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#define GLM_ENABLE_EXPERIMENTAL		
#include <glm/gtx/quaternion.hpp>
#include <vector>

#include "cComponent.h"
#include "cEntity.h"

struct sParticle {
	
	sParticle();

	glm::vec3 position;
	glm::vec3 velocity;
	float scale;
	glm::quat qOrientation;
	glm::quat qOrientationPerFrame;
	float lifeRemaining;
	float transparency;
	unsigned int index;
};

class cParticleComponent : public cComponent {
public:

	// Added this so:
	//	- The defaults are set
	//	- There isn't a ton of parameters passed on Init
	struct sParticleCreationParams {

		sParticleCreationParams() :
			minLifeTime(0.0f), maxLifeTime(1.0f),
			totalNumberOfParticles(100),
			position(glm::vec3(0.0f, 0.0f, 0.0f)),
			minInitialVelocity(glm::vec3(-1.0f, -1.0f, -1.0f)),
			maxInitialVelocity(glm::vec3(1.0f, 1.0f, 1.0f)),
			acceleration(glm::vec3(0.0f, 0.0f, 0.0f)),

			minInitialOrientationEuler(glm::vec3(0.0f, 0.0f, 0.0f)),
			maxInitialOrientationEuler(glm::vec3(0.0f, 0.0f, 0.0f)),
			minOrientationChangeEuler(glm::vec3(0.0f, 0.0f, 0.0f)),
			maxOrientationChangeEuler(glm::vec3(0.0f, 0.0f, 0.0f)),

			minInitialScale(0.1f), maxInitialScale(1.0f),
			minScaleChange(0.0f), maxScaleChange(0.0f),

			minNumberNewParticles(0), maxNumberNewParticles(1),
			minTimeBetweenParticleGenSeconds(0.0f),
			maxTimeBetweenParticleGenSeconds(0.1f),

			deathTransparencyFadeTimeSeconds(0.0f)
		{};

		float minLifeTime;
		float maxLifeTime;
		unsigned int totalNumberOfParticles;

		glm::vec3 position;
		glm::vec3 minInitialVelocity;
		glm::vec3 maxInitialVelocity;
		glm::vec3 acceleration;

		glm::vec3 minInitialOrientationEuler;
		glm::vec3 maxInitialOrientationEuler;
		glm::vec3 minOrientationChangeEuler; // Set these both to (0,0,0) to have no rotation over time
		glm::vec3 maxOrientationChangeEuler;

		float minInitialScale;
		float maxInitialScale;
		float minScaleChange; // Set these both to (0,0,0) to have no change
		float maxScaleChange;

		float deathTransparencyFadeTimeSeconds;

		unsigned int minNumberNewParticles;
		unsigned int maxNumberNewParticles;
		float minTimeBetweenParticleGenSeconds; // Number of seconds until next particle is generated
		float maxTimeBetweenParticleGenSeconds;

		
	};

	cEntity* particleEntity;

	cParticleComponent();
	~cParticleComponent();

	void init(sParticleCreationParams& params);

	// These will reset the entire generator to the initial state.
	// You can call this for an explosion by passing a large number. 
	void reset(void);		
	void reset(bool EnableParticleGeneration = true);
	void reset(unsigned int initialNumberOfParticles, bool EnableParticleGeneration = true);

	// Called every frame
	void update(double deltaTime); // HACK: Might make sense to register to a system

	void getAliveParticles(std::vector<sParticle> &vecParticles);
	void sortParticlesBackToFront(std::vector<sParticle> &vecParticles, glm::vec3 cameraEye);

	void particleGenerationOn(void);
	void particleGenerationOff(void);

	void serializeComponent(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);

	bool is_activated();

private:
	std::vector<sParticle> m_vecParticles;
	bool m_bIsInit;	// = false;
	bool m_bParticleGenerationOn;
	sParticleCreationParams m_newParticleParams;
	float m_nextParticleCountdownSeconds;

	void m_generateNewParticles(double deltaTime);
	// Resets a single particle to the "starting" state
	void m_resetParticle(sParticle* pParticle);
	bool m_findNextDeadParticle(sParticle* &pFoundParticle);

	float getRandInRange(float min, float max);
	unsigned int getRandInRange(unsigned int min, unsigned int max);
	glm::vec3 getRandInRange(glm::vec3 min, glm::vec3 max);
};



#endif // !_cParticleComponent_HG_

