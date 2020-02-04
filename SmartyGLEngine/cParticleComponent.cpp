#include "cParticleComponent.h"
#include "cEntityBuilder.h"
#include <algorithm>

// Predicate function for the sort
class cPredParticleIsCloser {
public:
	cPredParticleIsCloser(glm::vec3 cameraEye_){
		this->cameraEye = cameraEye_;
		return;
	}
	glm::vec3 cameraEye;
	
	bool operator() (const sParticle& P1, const sParticle &P2) {
		
		float P1_dist_to_camera = glm::distance(P1.position, this->cameraEye);
		float P2_dist_to_camera = glm::distance(P2.position, this->cameraEye);

		return P1_dist_to_camera < P2_dist_to_camera;
	}
};


sParticle::sParticle() {

	this->position = glm::vec3(0.0f);
	this->velocity = glm::vec3(0.0f);
	this->qOrientation = glm::quat(glm::vec3(0.0f));
	this->lifeRemaining = 0.0f;
	this->transparency = 1.0f; // Used for the "death fade" at the end of life
	this->index = 0;

	return;
}


cParticleComponent::cParticleComponent() : cComponent(7) {

	this->m_bIsInit = false;
	this->m_bParticleGenerationOn = false;
	this->m_nextParticleCountdownSeconds = 0.0f;

	this->particleEntity = cEntityBuilder::getInstance()->createEntity(22);

	return;
}

cParticleComponent::~cParticleComponent(){}

// EXPENSIVE operation, called when created or when
//  we want to drastically change the parameters
void cParticleComponent::init(sParticleCreationParams& params) {

	// Clear any existing particles
	this->m_vecParticles.clear();
	this->m_newParticleParams = params;

	// NOTE: This is really slow
	for (unsigned int index = 0; index != this->m_newParticleParams.totalNumberOfParticles; ++index) {

		sParticle newParticle;
		newParticle.lifeRemaining = -1.0f;
		newParticle.index = index;
		newParticle.position = this->m_newParticleParams.position;
		this->m_vecParticles.push_back(newParticle);
	}

	for (unsigned int index = 0; index != this->m_newParticleParams.totalNumberOfParticles; ++index) {

		this->m_vecParticles[index].index = index;
	}

	this->m_bIsInit = true;

	return;
}

// Uses initial number of particles as starting. 
// Emitter is NOT running at start
void cParticleComponent::reset(void) {

	unsigned int numParticles = this->getRandInRange(this->m_newParticleParams.minNumberNewParticles, this->m_newParticleParams.maxNumberNewParticles);
	this->reset(numParticles, false);

	return;
}

void cParticleComponent::reset(bool EnableParticleGeneration /*=true*/) {

	unsigned int numParticles = this->getRandInRange(this->m_newParticleParams.minNumberNewParticles, this->m_newParticleParams.maxNumberNewParticles);
	this->reset(numParticles, EnableParticleGeneration);

	return;
}

void cParticleComponent::reset(unsigned int initialNumberOfParticles, bool EnableParticleGeneration /*=true*/) {

	if (!this->m_bIsInit) return;

	// Set all the particles in motion
	unsigned int totalParticles = (unsigned int)this->m_vecParticles.size();
	unsigned index = 0;

	// "Kill" all the particles
	for (; index != totalParticles; ++index) {

		this->m_vecParticles[index].lifeRemaining = -1.0f;
	}

	this->m_bParticleGenerationOn = EnableParticleGeneration;

	if (initialNumberOfParticles > this->m_newParticleParams.totalNumberOfParticles) {
		initialNumberOfParticles = this->m_newParticleParams.totalNumberOfParticles;
	}

	sParticle* pCurParticle = nullptr;
	for (int count = 0; count != initialNumberOfParticles; ++count) {

		if (this->m_findNextDeadParticle(pCurParticle)) {
			this->m_resetParticle(pCurParticle);
		}
		else {
			// No more dead particles to "recycle"
			break;
		}
	}

	return;
}

void cParticleComponent::update(double deltaTime) {

	if (!this->m_bIsInit) return;

	// Update each "living" particle using Euler
	float fDeltaTime = float(deltaTime);
	unsigned int totalParticles = (unsigned int)this->m_vecParticles.size();
	unsigned int index = 0;

	sParticle* pCurParticle = &(this->m_vecParticles[index]);

	for (; index != totalParticles; ++index) {

		pCurParticle = &(this->m_vecParticles[index]);

		// Is the particle alive?
		if (pCurParticle->lifeRemaining > 0.0f) {

			pCurParticle->velocity += (fDeltaTime * this->m_newParticleParams.acceleration);
			pCurParticle->position += (fDeltaTime * pCurParticle->velocity);

			// Update scale
			float scaleChange = getRandInRange(this->m_newParticleParams.minScaleChange, this->m_newParticleParams.maxScaleChange);
			pCurParticle->scale += scaleChange;

			// Update rotation
			// This is a SLERP, so we pick a value between the min angle (no change) 
			//  to the maximum angle (qOrientationChangePerFrame), spherically interpolated 
			//  by how much time has passed THIS FRAME (fDeltaTime)
			glm::quat qAngleAdjustThisFrame = glm::slerp(glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)),	// Angle of 0 degrees on all axes
				pCurParticle->qOrientationPerFrame,
				fDeltaTime);
			pCurParticle->qOrientation = pCurParticle->qOrientation * qAngleAdjustThisFrame;

			// Are we near the end of life (and are to fade)?
			if (pCurParticle->lifeRemaining < this->m_newParticleParams.deathTransparencyFadeTimeSeconds) {
				// Calculate how much transparency is left
				pCurParticle->transparency = 1.0f - ((this->m_newParticleParams.deathTransparencyFadeTimeSeconds - pCurParticle->lifeRemaining) / this->m_newParticleParams.deathTransparencyFadeTimeSeconds);
				// Clamp to 0.0f (if life is -ve)
				pCurParticle->transparency = fmax(pCurParticle->transparency, 0.0f);
			}

			// Take some life away
			pCurParticle->lifeRemaining -= fDeltaTime;
		}
	}

	this->m_generateNewParticles(deltaTime);

	return;
}

void cParticleComponent::getAliveParticles(std::vector<sParticle>& vecParticles) {

	vecParticles.clear();
	vecParticles.reserve(this->m_vecParticles.size());

	unsigned int numParticles = (unsigned int)this->m_vecParticles.size();

	for (unsigned int index = 0; index != numParticles; ++index) {

		if (this->m_vecParticles[index].lifeRemaining > 0.0f) {
			vecParticles.push_back(this->m_vecParticles[index]);
		}
	}

	return;
}

void cParticleComponent::sortParticlesBackToFront(std::vector<sParticle>& vecParticles, glm::vec3 cameraEye) {

	cPredParticleIsCloser functionCamera(cameraEye);
	std::sort(vecParticles.begin(), vecParticles.end(), functionCamera);

	return;
}

void cParticleComponent::particleGenerationOn(void) {

	this->m_bParticleGenerationOn = true;

	return;
}

void cParticleComponent::particleGenerationOff(void) {

	this->m_bParticleGenerationOn = false;

	return;
}

void cParticleComponent::serializeComponent(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) {

	//Start Component
	writer.StartObject();

	writer.String("componentType");
	writer.Int(this->COMPONENT_TYPE_ID);

	writer.String("minLifeTime");
	writer.Double(this->m_newParticleParams.minLifeTime);

	writer.String("maxLifeTime");
	writer.Double(this->m_newParticleParams.maxLifeTime);

	writer.String("totalParticles");
	writer.Uint(this->m_newParticleParams.totalNumberOfParticles);

	writer.String(("position"));
	writer.StartObject();
	writer.String("x");
	writer.Double(this->m_newParticleParams.position.x);
	writer.String("y");
	writer.Double(this->m_newParticleParams.position.y);
	writer.String("z");
	writer.Double(this->m_newParticleParams.position.z);
	writer.EndObject();

	writer.String(("minInitialVelocity"));
	writer.StartObject();
	writer.String("x");
	writer.Double(this->m_newParticleParams.minInitialVelocity.x);
	writer.String("y");
	writer.Double(this->m_newParticleParams.minInitialVelocity.y);
	writer.String("z");
	writer.Double(this->m_newParticleParams.minInitialVelocity.z);
	writer.EndObject();

	writer.String(("maxInitialVelocity"));
	writer.StartObject();
	writer.String("x");
	writer.Double(this->m_newParticleParams.maxInitialVelocity.x);
	writer.String("y");
	writer.Double(this->m_newParticleParams.maxInitialVelocity.y);
	writer.String("z");
	writer.Double(this->m_newParticleParams.maxInitialVelocity.z);
	writer.EndObject();

	writer.String(("acceleration"));
	writer.StartObject();
	writer.String("x");
	writer.Double(this->m_newParticleParams.acceleration.x);
	writer.String("y");
	writer.Double(this->m_newParticleParams.acceleration.y);
	writer.String("z");
	writer.Double(this->m_newParticleParams.acceleration.z);
	writer.EndObject();

	writer.String(("minInitialOrientation"));
	writer.StartObject();
	writer.String("x");
	writer.Double(this->m_newParticleParams.minInitialOrientationEuler.x);
	writer.String("y");
	writer.Double(this->m_newParticleParams.minInitialOrientationEuler.y);
	writer.String("z");
	writer.Double(this->m_newParticleParams.minInitialOrientationEuler.z);
	writer.EndObject();

	writer.String(("maxInitialOrientation"));
	writer.StartObject();
	writer.String("x");
	writer.Double(this->m_newParticleParams.maxInitialOrientationEuler.x);
	writer.String("y");
	writer.Double(this->m_newParticleParams.maxInitialOrientationEuler.x);
	writer.String("z");
	writer.Double(this->m_newParticleParams.maxInitialOrientationEuler.x);
	writer.EndObject();

	writer.String(("minOrientationChange"));
	writer.StartObject();
	writer.String("x");
	writer.Double(this->m_newParticleParams.minOrientationChangeEuler.x);
	writer.String("y");
	writer.Double(this->m_newParticleParams.minOrientationChangeEuler.y);
	writer.String("z");
	writer.Double(this->m_newParticleParams.minOrientationChangeEuler.z);
	writer.EndObject();

	writer.String(("maxOrientationChange"));
	writer.StartObject();
	writer.String("x");
	writer.Double(this->m_newParticleParams.maxOrientationChangeEuler.x);
	writer.String("y");
	writer.Double(this->m_newParticleParams.maxOrientationChangeEuler.y);
	writer.String("z");
	writer.Double(this->m_newParticleParams.maxOrientationChangeEuler.z);
	writer.EndObject();
	
	writer.String("minInitialScale");
	writer.Double(this->m_newParticleParams.minInitialScale);

	writer.String("maxInitialScale");
	writer.Double(this->m_newParticleParams.maxInitialScale);

	writer.String("minScaleChange");
	writer.Double(this->m_newParticleParams.minScaleChange);

	writer.String("maxScaleChange");
	writer.Double(this->m_newParticleParams.maxScaleChange);

	writer.String("deathFadeTime");
	writer.Double(this->m_newParticleParams.deathTransparencyFadeTimeSeconds);

	writer.String("minNewParticles");
	writer.Uint(this->m_newParticleParams.minNumberNewParticles);

	writer.String("maxNewParticles");
	writer.Uint(this->m_newParticleParams.maxNumberNewParticles);

	writer.String("minTimeBetweenGen");
	writer.Double(this->m_newParticleParams.minTimeBetweenParticleGenSeconds);

	writer.String("maxTimeBetweenGen");
	writer.Double(this->m_newParticleParams.maxTimeBetweenParticleGenSeconds);
	
	writer.EndObject();

	return;
}

bool cParticleComponent::is_activated()
{
	return this->m_bParticleGenerationOn;
}

void cParticleComponent::m_generateNewParticles(double deltaTime) {

	if (!this->m_bParticleGenerationOn) return;

	// Has enough time passed between particle generation?
	if (this->m_nextParticleCountdownSeconds > 0.0f) {
		// No so we wait a little more
		this->m_nextParticleCountdownSeconds -= (float)deltaTime;
	}

	// Enoguh time has passes, to make the particles
	unsigned int numParticles = this->getRandInRange(this->m_newParticleParams.minNumberNewParticles, this->m_newParticleParams.maxNumberNewParticles);
	sParticle* pCurParticle = nullptr;

	for (int count = 0; count != numParticles; ++count) {

		if (this->m_findNextDeadParticle(pCurParticle)) {
			this->m_resetParticle(pCurParticle);
		}
		else {
			// No more dead particles to "recycle"
			break;
		}
	}

	// Pick a new "time to wait between particles"
	this->m_nextParticleCountdownSeconds = this->getRandInRange(this->m_newParticleParams.minTimeBetweenParticleGenSeconds, this->m_newParticleParams.maxTimeBetweenParticleGenSeconds);

	return;
}

void cParticleComponent::m_resetParticle(sParticle* pParticle) {

	if (!this->m_bIsInit) return;

	// Pick a random lifetime
	pParticle->lifeRemaining = this->getRandInRange(this->m_newParticleParams.minLifeTime, this->m_newParticleParams.maxLifeTime);

	// Pick a random velocity
	pParticle->velocity = this->getRandInRange(this->m_newParticleParams.minInitialVelocity, this->m_newParticleParams.maxInitialVelocity);

	pParticle->qOrientation = glm::quat(
		/* Pick Euler angle, which is converted to quaternion in constructor */
		glm::vec3(this->getRandInRange(this->m_newParticleParams.minInitialOrientationEuler.x,
				this->m_newParticleParams.maxInitialOrientationEuler.x),
			
			this->getRandInRange(this->m_newParticleParams.minInitialOrientationEuler.y,
				this->m_newParticleParams.maxInitialOrientationEuler.y),
			
			this->getRandInRange(this->m_newParticleParams.minInitialOrientationEuler.z,
				this->m_newParticleParams.maxInitialOrientationEuler.z)));

	// Rotation change over time
	pParticle->qOrientationPerFrame = glm::quat(
		/* Pick Euler angle, which is converted to quaternion in constructor */
		glm::vec3(this->getRandInRange(this->m_newParticleParams.minOrientationChangeEuler.x,
				this->m_newParticleParams.maxOrientationChangeEuler.x),
			
			this->getRandInRange(this->m_newParticleParams.minOrientationChangeEuler.y,
				this->m_newParticleParams.maxOrientationChangeEuler.y),
			
			this->getRandInRange(this->m_newParticleParams.minOrientationChangeEuler.z,
				this->m_newParticleParams.maxOrientationChangeEuler.z)));

	pParticle->scale = this->getRandInRange(this->m_newParticleParams.minInitialScale, this->m_newParticleParams.maxInitialScale);
	pParticle->position = this->m_newParticleParams.position;
	pParticle->transparency = 1.0f;

	return;
}

bool cParticleComponent::m_findNextDeadParticle(sParticle*& pFoundParticle) {
	
	unsigned int totalParticles = (unsigned int)this->m_vecParticles.size();
	unsigned index = 0;

	for (; index != totalParticles; ++index) {

		if (this->m_vecParticles[index].lifeRemaining <= 0.0f) {
			// It's dead Jim....
			pFoundParticle = &(this->m_vecParticles[index]);

			return true;
		}
	}
	
	return false;
}

float cParticleComponent::getRandInRange(float min, float max) {
	
	float value =
		min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (static_cast<float>(max - min))));
	return value;
}

unsigned int cParticleComponent::getRandInRange(unsigned int min, unsigned int max) {
	
	// You could do this with the modulus operator (since it's an int)
	float value =
		min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (static_cast<float>(max - min))));
	return (unsigned int)value;
}

glm::vec3 cParticleComponent::getRandInRange(glm::vec3 min, glm::vec3 max) {
	glm::vec3 returnVal;

	returnVal.x = this->getRandInRange(min.x, max.x);
	returnVal.y = this->getRandInRange(min.y, max.y);
	returnVal.z = this->getRandInRange(min.z, max.z);

	return returnVal;
}
