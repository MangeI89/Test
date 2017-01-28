#include "GlobalFunctions.h"

#define WIDTH 1000
#define HEIGHT 600
#define NEAR 0.1f
#define FAR 200.0f
#define FOV 70.0f

bool HandleEvents(const shared_ptr<Display> display, const shared_ptr<Skybox> skybox, const shared_ptr<Camera> camera, const shared_ptr<Light>& sunLight,
	const shared_ptr<Light>& fireLight, const shared_ptr<Light>& starsLight, const shared_ptr<Particles>& fireParticles, 
	const shared_ptr<Particles>& starsParticles, shared_ptr<EventHolder>& eventHolder,const shared_ptr<Entity>& waterEntity,const vector<shared_ptr<Shader>> waterShaders) {

	SDL_Event event;
	bool newframe = true;
	bool firstFrame = false;


	while (SDL_PollEvent(&event)) {

		if (event.type == SDL_QUIT) {
			display->CloseWindow();
		}

		if (event.type == SDL_MOUSEMOTION && newframe) {

			GLfloat xOffset = event.motion.xrel;
			GLfloat yOffset = event.motion.yrel;
			camera->UpdateForward(xOffset, yOffset);
			SDL_WarpMouseInWindow(display->GetWindow(), 750, 400);
			newframe = false;
		}

		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_w:
				camera->MoveForward();
				break;

			case SDLK_s:
				camera->MoveBackwards();
				break;

			case SDLK_a:
				camera->MoveLeft();
				break;

			case SDLK_d:
				camera->MoveRight();
				break;

			case SDLK_i:
				sunLight->MoveNegativeZ();
				break;

			case SDLK_k:
				sunLight->MovePositiveZ();
				break;

			case SDLK_j:
				sunLight->MoveNegativeX();
				break;

			case SDLK_l:
				sunLight->MovePositiveX();
				break;

			case SDLK_1:
				cout << "Environment on/off" << endl;
				eventHolder->ToggleRenderEnvironment();
				break;

			case SDLK_2:
				cout << "ReflectionFBO on/off" << endl;
				eventHolder->ToggleRenderReflectionFBO();
				break;

			case SDLK_3:
				cout << "RefractionFBO on/off" << endl;
				eventHolder->ToggleRenderRefractionFBO();
				break;

			case SDLK_4:
				cout << "BottomDepthFBO on/off" << endl;
				eventHolder->ToggleRenderBottomDepthFBO();
				break;

			case SDLK_5:
				cout << "ShadowFBO on/off" << endl;
				eventHolder->ToggleRenderShadowFBO();
				break;

			case SDLK_6:
				cout << "Particles on/off" << endl;
				eventHolder->ToggleRenderParticles();
				break;

			case SDLK_7:
				cout << "ParticleFBO on/off" << endl;
				eventHolder->ToggleRenderParticleFBOS();
				break;

			case SDLK_8:
				cout << "Fire/Stars on/off" << endl;
				eventHolder->ToggleRenderFire();
				break;

			case SDLK_9:
				cout << "ParticleFBO on/off" << endl;
				eventHolder->ToggleRenderFireFBO();
				break;

			case SDLK_0:
				skybox->ToggleSkybox();
				break;
			case SDLK_o:
				fireLight->ToggleRendering();
				fireParticles->ToggleRendering();
				fireParticles->ToggleAnimating();
				break;

			case SDLK_p:
				firstFrame = true;
				break;

			case SDLK_z:
				cout << "Watershader0 - with lightning" << endl;
				waterEntity->AttachShader(waterShaders.at(0));
				eventHolder->SetReflection(false);
				eventHolder->SetRefraction(false);
				eventHolder->SetBottomDepth(false);
				eventHolder->SetShadows(false);
				break;

			case SDLK_x:
				cout << "Watershader1 - with normalmap" << endl;
				waterEntity->AttachShader(waterShaders.at(1));
				eventHolder->SetReflection(false);
				eventHolder->SetRefraction(false);
				eventHolder->SetBottomDepth(false);
				eventHolder->SetShadows(false);
				break;

			case SDLK_c:
				cout << "Watershader2 with reflection" << endl;
				waterEntity->AttachShader(waterShaders.at(2));
				eventHolder->SetReflection(true);
				eventHolder->SetRefraction(false);
				eventHolder->SetBottomDepth(false);
				eventHolder->SetShadows(false);
				break;

			case SDLK_v:
				cout << "Watershader3 with refraction" << endl;
				waterEntity->AttachShader(waterShaders.at(3));
				eventHolder->SetReflection(true);
				eventHolder->SetRefraction(true);
				eventHolder->SetBottomDepth(false);
				eventHolder->SetShadows(false);
				break;

			case SDLK_b:
				cout << "Watershader4 with bottomdepth" << endl;
				waterEntity->AttachShader(waterShaders.at(4));
				eventHolder->SetReflection(true);
				eventHolder->SetRefraction(true);
				eventHolder->SetBottomDepth(true);
				eventHolder->SetShadows(false);
				break;

			case SDLK_n:
				cout << "Watershader5 with shadows" << endl;
				waterEntity->AttachShader(waterShaders.at(5));
				eventHolder->SetReflection(true);
				eventHolder->SetRefraction(true);
				eventHolder->SetBottomDepth(true);
				eventHolder->SetShadows(true);
				break;

			case SDLK_m:
				cout << "Water motion on/off" << endl;
				eventHolder->ToggleWaterMotion();
				break;
			case SDLK_f:
				cout << "Watershader6 with multi-shadows" << endl;
				waterEntity->AttachShader(waterShaders.at(6));
				eventHolder->SetReflection(true);
				eventHolder->SetRefraction(true);
				eventHolder->SetBottomDepth(true);
				eventHolder->SetShadows(true);
				break;
			case SDLK_g:
				cout << "Watershader with multi-shadows and lowpass-filtering" << endl;
				waterEntity->AttachShader(waterShaders.at(7));
				eventHolder->SetReflection(true);
				eventHolder->SetRefraction(true);
				eventHolder->SetBottomDepth(true);
				eventHolder->SetShadows(true);
				break;

			case SDLK_ESCAPE:
				display->CloseWindow();
			default:
				break;
			}

		}
	}

	return firstFrame;
}

void UpdateLightMatrices(const shared_ptr<Camera>& camera, const shared_ptr<Light>& light,const float& shadowDistance) {
	
	vec3 cameraForward = camera->GetForward();
	vec3 cameraPosition = camera->GetPosition();
	mat4 cameraRotation = camera->GetRotationMatrix();

	vec3 cameraUp = normalize(vec3(cameraRotation * vec4(0, 1, 0, 1))); //Should be ok without normalizing but just in case
	vec3 cameraLeft = cross(cameraUp, cameraForward);

	//Center coordinates for far and near plane in world coordinates 
	vec3 toFar = cameraForward * shadowDistance;
	vec3 toNear = cameraForward * NEAR;

	vec3 centerFar = cameraPosition + toFar;
	vec3 centerNear = cameraPosition + toNear;
	vec3 center = cameraPosition + cameraForward * (shadowDistance - NEAR) / 2.0f;

	//light->UpdateTarget(center);

	float farHalfWidth = (float)(shadowDistance * tan(radians(FOV) / 2));
	float nearHalfWidth = (float)(NEAR * tan(radians(FOV) / 2));
	
	float farHalfHeight = farHalfWidth * HEIGHT / WIDTH;
	float nearHalfHeight = nearHalfWidth * HEIGHT / WIDTH;
	
	//Far frustum corners in world coordinates
	vec3 topLeftFar = centerFar + cameraUp * farHalfHeight + cameraLeft * farHalfWidth;
	vec3 topRightFar = centerFar + cameraUp * farHalfHeight - cameraLeft * farHalfWidth;
	vec3 bottomLeftFar = centerFar - cameraUp * farHalfHeight + cameraLeft * farHalfWidth;
	vec3 bottomRightFar = centerFar - cameraUp * farHalfHeight - cameraLeft * farHalfWidth;
	
	//Near frustum corners in world coordinates
	vec3 topLeftNear = centerNear + cameraUp * nearHalfHeight + cameraLeft * nearHalfWidth;
	vec3 topRightNear = centerNear + cameraUp * nearHalfHeight - cameraLeft * nearHalfWidth;
	vec3 bottomLeftNear = centerNear - cameraUp * nearHalfHeight + cameraLeft * nearHalfWidth;
	vec3 bottomRightNear = centerNear - cameraUp * nearHalfHeight - cameraLeft * nearHalfWidth;

	//Far frustum cornerns in light coordinates
	mat4 lightViewMatrix = light->GetViewMatrix(vec3(0,0,0));

	vector<vec3> frustumCorners;

	vec3 minVector, maxVector;

	minVector = vec3(lightViewMatrix * vec4(topLeftFar, 1.0));
	maxVector = minVector;

	frustumCorners.push_back(vec3(lightViewMatrix * vec4(topRightFar, 1.0)));
	frustumCorners.push_back(vec3(lightViewMatrix * vec4(bottomLeftFar, 1.0)));
	frustumCorners.push_back(vec3(lightViewMatrix * vec4(bottomRightFar, 1.0)));

	//Near frustum corners in light coordinates
	frustumCorners.push_back(vec3(lightViewMatrix * vec4(topLeftNear, 1.0)));
	frustumCorners.push_back(vec3(lightViewMatrix * vec4(topRightNear, 1.0)));
	frustumCorners.push_back(vec3(lightViewMatrix * vec4(bottomLeftNear, 1.0)));
	frustumCorners.push_back(vec3(lightViewMatrix * vec4(bottomRightNear, 1.0)));

	for (auto& corner : frustumCorners) {
		if (corner.x > maxVector.x) {
			maxVector.x = corner.x;
		} else if (corner.x < minVector.x) {
			minVector.x = corner.x;
		}

		if (corner.y > maxVector.y) {
			maxVector.y = corner.y;
		}
		else if (corner.y < minVector.y) {
			minVector.y = corner.y;
		}

		if (corner.z > maxVector.z) {
			maxVector.z = corner.z;
		}
		else if (corner.z < minVector.z) {
			minVector.z = corner.z;
		}
	}
	vec3 halfDifference = (maxVector - minVector) / 2.0f;

	mat4 lightProjection = ortho(-halfDifference.x, halfDifference.x, -halfDifference.y, halfDifference.y, -halfDifference.z, halfDifference.z);
	cout << "xMin: " << minVector.x << endl;
	cout << "xMax: " << maxVector.x << endl;

	light->UpdateLightProjection(lightProjection);


}

void Diamond(vec3 *vertexArray, const GLuint& x, const GLuint& z, const GLuint offset, GLuint size, float scale, float borderHeight) {

	float a;
	float b;
	float c;
	float d;

	float f = (float)rand() / RAND_MAX;
	float value = -1.0*scale + f * 2.0*scale;

	if (x == 0) {
		a = vertexArray[size - offset - 1 + z*size].y;
		b = vertexArray[x + offset + z*size].y;
		c = vertexArray[x + z*size - offset*size].y;
		d = vertexArray[x + z*size + offset*size].y;
		vertexArray[x + z*size] = vec3(x, (b + c + d) / 3 + value, z);
		vertexArray[x + z*size] = vec3(x, borderHeight + value / 5.0, z);
	}
	else if (x == size - 1) {
		a = vertexArray[x - offset + z*size].y;
		b = vertexArray[offset + z*size].y;
		c = vertexArray[x + z*size - offset*size].y;
		d = vertexArray[x + z*size + offset*size].y;
		vertexArray[x + z*size] = vec3(x, (a + c + d) / 3 + value, z);
		vertexArray[x + z*size] = vec3(x, borderHeight + value / 5.0, z);
	}
	else if (z == 0) {
		a = vertexArray[x - offset + z*size].y;
		b = vertexArray[x + offset + z*size].y;
		c = vertexArray[x + (size - offset - 1)*size].y;
		d = vertexArray[x + z*size + offset*size].y;
		vertexArray[x + z*size] = vec3(x, (a + b + d) / 3 + value, z);
		vertexArray[x + z*size] = vec3(x, borderHeight + value / 5.0, z);
	}
	else if (z == size - 1) {
		a = vertexArray[x - offset + z*size].y;
		b = vertexArray[x + offset + z*size].y;
		c = vertexArray[x + z*size - offset*size].y;
		d = vertexArray[x + offset*size].y;
		vertexArray[x + z*size] = vec3(x, (a + b + c) / 3 + value, z);
		vertexArray[x + z*size] = vec3(x, borderHeight + value / 5.0, z);
	}
	else {
		a = vertexArray[x - offset + z*size].y;
		b = vertexArray[x + offset + z*size].y;
		c = vertexArray[x + z*size + offset*size].y;
		d = vertexArray[x + z*size + offset*size].y;
		vertexArray[x + z*size] = vec3(x, (a + b + c + d) / 4 + value, z);
	}

}

void Square(vec3 *vertexArray, GLuint x, GLuint z, GLuint offset, GLuint size, float scale, float borderHeight) {

	float a;
	float b;
	float c;
	float d;

	float f = (float)rand() / RAND_MAX;
	float value = -1.0 * scale + f * 3.0 * scale;


	if (x == 0) {
		a = vertexArray[size - offset + z*size - offset*size].y;
		b = vertexArray[x + offset + z*size - offset*size].y;
		c = vertexArray[size - offset + z*size + offset*size].y;
		d = vertexArray[x + offset + z*size + offset*size].y;
		vertexArray[x + z*size] = vec3(x, (b + c + d) / 3 + value / 5.0, z);
		vertexArray[x + z*size] = vec3(x, borderHeight, z);
	}
	else if (x == size - 1) {
		a = vertexArray[x - offset + z*size - offset*size].y;
		b = vertexArray[offset - 1 + z*size - offset*size].y;
		c = vertexArray[x - offset + z*size + offset*size].y;
		d = vertexArray[offset - 1 + z*size + offset*size].y;
		vertexArray[x + z*size] = vec3(x, (a + c + d) / 3 + value, z);
		vertexArray[x + z*size] = vec3(x, borderHeight + value / 5.0, z);
	}
	else if (z == 0) {
		a = vertexArray[x - offset + (size - offset)*size].y;
		b = vertexArray[x + offset + (size - offset)*size].y;
		c = vertexArray[x - offset + z*size + offset*size].y;
		d = vertexArray[x + offset + z*size + offset*size].y;
		vertexArray[x + z*size] = vec3(x, (a + b + d) / 3 + value, z);
		vertexArray[x + z*size] = vec3(x, borderHeight + value / 5.0, z);
	}
	else if (z == size - 1) {
		a = vertexArray[x - offset + z*size - offset*size].y;
		b = vertexArray[x + offset + z*size - offset*size].y;
		c = vertexArray[x - offset + (offset - 1)*size].y;
		d = vertexArray[x + offset + (offset - 1)*size].y;
		vertexArray[x + z*size] = vec3(x, (a + b + c) / 3 + value, z);
		vertexArray[x + z*size] = vec3(x, borderHeight + value / 5.0, z);
	}
	else {
		a = vertexArray[x - offset + z*size - offset*size].y;
		b = vertexArray[x + offset + z*size - offset*size].y;
		c = vertexArray[x - offset + z*size + offset*size].y;
		d = vertexArray[x + offset + z*size + offset*size].y;
		vertexArray[x + z*size] = vec3(x, (a + b + c + d) / 4 + value, z);
	}
}

Terrain GenerateTerrain(const GLuint& scale) {

	GLuint size = pow(2, scale) + 1;
	float randomScale = 1.0 * (float)(size - 1) / 4.0;
	float borderHeight = (float)(size - 1) / 8.0;
	float maxWaterDepth = (float)(size - 1) / 2.0;

	//randomScale = 8;
	//borderHeight = 2;
	//maxWaterDepth = 16;

	unsigned int vertexCount = size * size;
	unsigned int triangleCount = (size - 1) * (size - 1) * 2;
	unsigned int numIndices = triangleCount * 3;
	int x, z;

	//vec3 *vertexArray = (vec3*)malloc(sizeof(vec3) * vertexCount);
	vec3 *vertexArray = new vec3[vertexCount];
	vec3 *smoothedVertexArray = new vec3[vertexCount];
	vec3 *normalArray = new vec3[vertexCount];
	vec2 *texCoordArray = new vec2[vertexCount];
	GLuint *indexArray = new GLuint[numIndices];

	Vertex *finalVertexArray = new Vertex[vertexCount];

	//INIT CORNERS
	x = 0;
	z = 0;
	vertexArray[x + z*size] = vec3(x, borderHeight, z);

	x = size - 1;
	z = 0;

	vertexArray[x + z*size] = vec3(x, borderHeight, z);

	x = 0;
	z = size - 1;
	vertexArray[x + z * size] = vec3(x, borderHeight, z);

	x = size - 1;
	z = size - 1;
	vertexArray[x + z * size] = vec3(x, borderHeight, z);

	x = (size - 1) / 2;
	z = (size - 1) / 2;

	int fullOffset = (size - 1);
	int halfOffset = (size - 1) / 2;

	while (halfOffset >= 1) {

		for (int x = halfOffset; x < size - 1; x += fullOffset) {
			for (int z = halfOffset; z < size - 1; z += fullOffset) {
				if (halfOffset == (size - 1) / 2) {
					vertexArray[x + z*size] = vec3(x, -maxWaterDepth, z);
				}
				else {
					Square(vertexArray, x, z, halfOffset, size, randomScale, borderHeight);
				}
			}
		}

		for (int x = 0; x < size; x += fullOffset) {
			for (int z = 0; z < size; z += fullOffset) {
				if (z + halfOffset < size) {
					Diamond(vertexArray, x, z + halfOffset, halfOffset, size, randomScale, borderHeight);
				}
				if (x + halfOffset < size) {
					Diamond(vertexArray, x + halfOffset, z, halfOffset, size, randomScale, borderHeight);
				}
			}
		}
		fullOffset /= 2;
		halfOffset /= 2;
		randomScale /= 2.0;
	}

	for (int x = 0; x < size; x++) {
		for (int z = 0; z < size; z++) {
			double y;
;			if (x == 0) {
				if (z == 0) {
					y = (
						vertexArray[x + z*size].y + vertexArray[x + (z + 1)*size].y + vertexArray[x + (z + 2)*size].y +
						vertexArray[x + 1 + z*size].y + vertexArray[x + 1 + (z + 1)*size].y + vertexArray[x + 1 + (z + 2)*size].y +
						vertexArray[x + 2 + z*size].y + vertexArray[x + 2 + (z + 1)*size].y + vertexArray[x + 2 + (z + 2)*size].y) / 9.0;
				}
				else if (z == (size - 1)) {
					y = (
						vertexArray[x + (z - 2)*size].y + vertexArray[x + (z - 1)*size].y + vertexArray[x + z*size].y +
						vertexArray[x + 1 + (z - 2)*size].y + vertexArray[x + 1 + (z - 1)*size].y + vertexArray[x + 1 + z*size].y +
						vertexArray[x + 2 + (z - 2)*size].y + vertexArray[x + 2 + (z - 1)*size].y + vertexArray[x + 2 + z*size].y) / 9.0;
				}
				else if (z == 1) {
					y = (
						vertexArray[x + (z - 1)*size].y + vertexArray[x + z*size].y +
						vertexArray[x + (z + 1)*size].y + vertexArray[x + (z + 2)*size].y +
						vertexArray[x + 1 + (z - 1)*size].y + vertexArray[x + 1 + z*size].y +
						vertexArray[x + 1 + (z + 1)*size].y + vertexArray[x + 1 + (z + 2)*size].y +
						vertexArray[x + 2 + (z - 1)*size].y + vertexArray[x + 2 + z*size].y +
						vertexArray[x + 2 + (z + 1)*size].y + vertexArray[x + 2 + (z + 2)*size].y) / 12.0;
				}
				else if (z == (size - 2)) {
					y = (
						vertexArray[x + (z - 2)*size].y + vertexArray[x + (z - 1)*size].y + vertexArray[x + z*size].y +
						vertexArray[x + (z + 1)*size].y +
						vertexArray[x + 1 + (z - 2)*size].y + vertexArray[x + 1 + (z - 1)*size].y + vertexArray[x + 1 + z*size].y +
						vertexArray[x + 1 + (z + 1)*size].y +
						vertexArray[x + 2 + (z - 2)*size].y + vertexArray[x + 2 + (z - 1)*size].y + vertexArray[x + 2 + z*size].y +
						vertexArray[x + 2 + (z + 1)*size].y) / 12.0;
				}
				else {
					y = (
						vertexArray[x + (z - 2)*size].y + vertexArray[x + (z - 1)*size].y + vertexArray[x + z*size].y +
						vertexArray[x + (z + 1)*size].y + vertexArray[x + (z + 2)*size].y +
						vertexArray[x + 1 + (z - 2)*size].y + vertexArray[x + 1 + (z - 1)*size].y + vertexArray[x + 1 + z*size].y +
						vertexArray[x + 1 + (z + 1)*size].y + vertexArray[x + 1 + (z + 2)*size].y +
						vertexArray[x + 2 + (z - 2)*size].y + vertexArray[x + 2 + (z - 1)*size].y + vertexArray[x + 2 + z*size].y +
						vertexArray[x + 2 + (z + 1)*size].y + vertexArray[x + 2 + (z + 2)*size].y) / 15.0;
				}

			}
			else if (x == (size - 1)) {
				if (z == 0) {
					y = (
						vertexArray[x - 2 + z*size].y + vertexArray[x - 2 + (z + 1)*size].y + vertexArray[x - 2 + (z + 2)*size].y +
						vertexArray[x - 1 + z*size].y + vertexArray[x - 1 + (z + 1)*size].y + vertexArray[x - 1 + (z + 2)*size].y +
						vertexArray[x + z*size].y + vertexArray[x + (z + 1)*size].y + vertexArray[x + (z + 2)*size].y) / 9.0;
				}
				else if (z == (size - 1)) {
					y = (
						vertexArray[x - 2 + (z - 2)*size].y + vertexArray[x - 2 + (z - 1)*size].y + vertexArray[x - 2 + z*size].y +
						vertexArray[x - 2 + (z + 1)*size].y + vertexArray[x - 2 + (z + 2)*size].y +
						vertexArray[x - 1 + (z - 2)*size].y + vertexArray[x - 1 + (z - 1)*size].y + vertexArray[x - 1 + z*size].y +
						vertexArray[x + (z - 2)*size].y + vertexArray[x + (z - 1)*size].y + vertexArray[x + z*size].y) / 9.0;
				}
				else if(z == 1) {
					y = (
						vertexArray[x - 2 + (z - 1)*size].y + vertexArray[x - 2 + z*size].y +
						vertexArray[x - 2 + (z + 1)*size].y + vertexArray[x - 2 + (z + 2)*size].y +
						vertexArray[x - 1 + (z - 1)*size].y + vertexArray[x - 1 + z*size].y +
						vertexArray[x - 1 + (z + 1)*size].y + vertexArray[x - 1 + (z + 2)*size].y +
						vertexArray[x + (z - 1)*size].y + vertexArray[x + z*size].y +
						vertexArray[x + (z + 1)*size].y + vertexArray[x + (z + 2)*size].y) / 12.0;
				}
				else if (z == (size - 2)) {
					y = (
						vertexArray[x - 2 + (z - 2)*size].y + vertexArray[x - 2 + (z - 1)*size].y + vertexArray[x - 2 + z*size].y +
						vertexArray[x - 2 + (z + 1)*size].y + 
						vertexArray[x - 1 + (z - 2)*size].y + vertexArray[x - 1 + (z - 1)*size].y + vertexArray[x - 1 + z*size].y +
						vertexArray[x - 1 + (z + 1)*size].y + 
						vertexArray[x + (z - 2)*size].y + vertexArray[x + (z - 1)*size].y + vertexArray[x + z*size].y +
						vertexArray[x + (z + 1)*size].y) / 12.0;
				}
				else {
					y = (
						vertexArray[x - 2 + (z - 2)*size].y + vertexArray[x - 2 + (z - 1)*size].y + vertexArray[x - 2 + z*size].y +
						vertexArray[x - 2 + (z + 1)*size].y + vertexArray[x - 2 + (z + 2)*size].y +
						vertexArray[x - 1 + (z - 2)*size].y + vertexArray[x - 1 + (z - 1)*size].y + vertexArray[x - 1 + z*size].y +
						vertexArray[x - 1 + (z + 1)*size].y + vertexArray[x - 1 + (z + 2)*size].y +
						vertexArray[x + (z - 2)*size].y + vertexArray[x + (z - 1)*size].y + vertexArray[x + z*size].y +
						vertexArray[x + (z + 1)*size].y + vertexArray[x + (z + 2)*size].y) / 15.0;
				}
			}
			else if (z == 0) {
				if (x == 1) {
					y = (
						vertexArray[x - 1 + z*size].y + vertexArray[x - 1 + (z + 1)*size].y + vertexArray[x - 1 + (z + 2)*size].y +
						vertexArray[x + z*size].y + vertexArray[x + (z + 1)*size].y + vertexArray[x + (z + 2)*size].y +
						vertexArray[x + 1 + z*size].y + vertexArray[x + 1 + (z + 1)*size].y + vertexArray[x + 1 + (z + 2)*size].y +
						vertexArray[x + 2 + z*size].y + vertexArray[x + 2 + (z + 1)*size].y + vertexArray[x + 2 + (z + 2)*size].y) / 12.0;
				}
				else if (x == (size - 2)) {
					y = (
						vertexArray[x - 2 + z*size].y + vertexArray[x - 2 + (z + 1)*size].y + vertexArray[x - 2 + (z + 2)*size].y +
						vertexArray[x - 1 + z*size].y + vertexArray[x - 1 + (z + 1)*size].y + vertexArray[x - 1 + (z + 2)*size].y +
						vertexArray[x + z*size].y + vertexArray[x + (z + 1)*size].y + vertexArray[x + (z + 2)*size].y +
						vertexArray[x + 1 + z*size].y + vertexArray[x + 1 + (z + 1)*size].y + vertexArray[x + 1 + (z + 2)*size].y) / 12.0;
				}
				else {
					y = (
						vertexArray[x - 2 + z*size].y + vertexArray[x - 2 + (z + 1)*size].y + vertexArray[x - 2 + (z + 2)*size].y +
						vertexArray[x - 1 + z*size].y + vertexArray[x - 1 + (z + 1)*size].y + vertexArray[x - 1 + (z + 2)*size].y +
						vertexArray[x + z*size].y + vertexArray[x + (z + 1)*size].y + vertexArray[x + (z + 2)*size].y +
						vertexArray[x + 1 + z*size].y + vertexArray[x + 1 + (z + 1)*size].y + vertexArray[x + 1 + (z + 2)*size].y +
						vertexArray[x + 2 + z*size].y + vertexArray[x + 2 + (z + 1)*size].y + vertexArray[x + 2 + (z + 2)*size].y) / 15.0;
				}
			}
			else if (z == (size - 1)) {
				if (x == 1) {
					y = (
						vertexArray[x - 1 + (z - 2)*size].y + vertexArray[x - 1 + (z - 1)*size].y + vertexArray[x - 1 + z*size].y +
						vertexArray[x + (z - 2)*size].y + vertexArray[x + (z - 1)*size].y + vertexArray[x + z*size].y +
						vertexArray[x + 1 + (z - 2)*size].y + vertexArray[x + 1 + (z - 1)*size].y + vertexArray[x + 1 + z*size].y +
						vertexArray[x + 2 + (z - 2)*size].y + vertexArray[x + 2 + (z - 1)*size].y + vertexArray[x + 2 + z*size].y) / 12.0;
				}
				else if (x == (size - 2)) {
					y = (
						vertexArray[x - 2 + (z - 2)*size].y + vertexArray[x - 2 + (z - 1)*size].y + vertexArray[x - 2 + z*size].y +
						vertexArray[x - 1 + (z - 2)*size].y + vertexArray[x - 1 + (z - 1)*size].y + vertexArray[x - 1 + z*size].y +
						vertexArray[x + (z - 2)*size].y + vertexArray[x + (z - 1)*size].y + vertexArray[x + z*size].y +
						vertexArray[x + 1 + (z - 2)*size].y + vertexArray[x + 1 + (z - 1)*size].y + vertexArray[x + 1 + z*size].y) / 12.0;
				}
				else {
					y = (
						vertexArray[x - 2 + (z - 2)*size].y + vertexArray[x - 2 + (z - 1)*size].y + vertexArray[x - 2 + z*size].y +
						vertexArray[x - 1 + (z - 2)*size].y + vertexArray[x - 1 + (z - 1)*size].y + vertexArray[x - 1 + z*size].y +
						vertexArray[x + (z - 2)*size].y + vertexArray[x + (z - 1)*size].y + vertexArray[x + z*size].y +
						vertexArray[x + 1 + (z - 2)*size].y + vertexArray[x + 1 + (z - 1)*size].y + vertexArray[x + 1 + z*size].y +
						vertexArray[x + 2 + (z - 2)*size].y + vertexArray[x + 2 + (z - 1)*size].y + vertexArray[x + 2 + z*size].y) / 15.0;
				}

			}
			else if (x == 1) {
				if (z == 1) {
					y = (
						vertexArray[x - 1 + (z - 1)*size].y + vertexArray[x - 1 + z*size].y +
						vertexArray[x - 1 + (z + 1)*size].y + vertexArray[x - 1 + (z + 2)*size].y +
						vertexArray[x + (z - 1)*size].y + vertexArray[x + z*size].y +
						vertexArray[x + (z + 1)*size].y + vertexArray[x + (z + 2)*size].y +
						vertexArray[x + 1 + (z - 1)*size].y + vertexArray[x + 1 + z*size].y +
						vertexArray[x + 1 + (z + 1)*size].y + vertexArray[x + 1 + (z + 2)*size].y +
						vertexArray[x + 2 + (z - 1)*size].y + vertexArray[x + 2 + z*size].y +
						vertexArray[x + 2 + (z + 1)*size].y + vertexArray[x + 2 + (z + 2)*size].y) / 16.0;
				}
				else if (z == (size - 2)) {
					y = (
						vertexArray[x - 1 + (z - 2)*size].y + vertexArray[x - 1 + (z - 1)*size].y + vertexArray[x - 1 + z*size].y +
						vertexArray[x - 1 + (z + 1)*size].y +
						vertexArray[x + (z - 2)*size].y + vertexArray[x + (z - 1)*size].y + vertexArray[x + z*size].y +
						vertexArray[x + (z + 1)*size].y +
						vertexArray[x + 1 + (z - 2)*size].y + vertexArray[x + 1 + (z - 1)*size].y + vertexArray[x + 1 + z*size].y +
						vertexArray[x + 1 + (z + 1)*size].y +
						vertexArray[x + 2 + (z - 2)*size].y + vertexArray[x + 2 + (z - 1)*size].y + vertexArray[x + 2 + z*size].y +
						vertexArray[x + 2 + (z + 1)*size].y) / 16.0;
				}
				else {
					y = (
						vertexArray[x - 1 + (z - 2)*size].y + vertexArray[x - 1 + (z - 1)*size].y + vertexArray[x - 1 + z*size].y +
						vertexArray[x - 1 + (z + 1)*size].y + vertexArray[x - 1 + (z + 2)*size].y +
						vertexArray[x + (z - 2)*size].y + vertexArray[x + (z - 1)*size].y + vertexArray[x + z*size].y +
						vertexArray[x + (z + 1)*size].y + vertexArray[x + (z + 2)*size].y +
						vertexArray[x + 1 + (z - 2)*size].y + vertexArray[x + 1 + (z - 1)*size].y + vertexArray[x + 1 + z*size].y +
						vertexArray[x + 1 + (z + 1)*size].y + vertexArray[x + 1 + (z + 2)*size].y +
						vertexArray[x + 2 + (z - 2)*size].y + vertexArray[x + 2 + (z - 1)*size].y + vertexArray[x + 2 + z*size].y +
						vertexArray[x + 2 + (z + 1)*size].y + vertexArray[x + 2 + (z + 2)*size].y) / 20.0;
				}
			}
			else if (x == (size - 2)) {
				if (z == 1) {
					y = (
						vertexArray[x - 2 + (z - 1)*size].y + vertexArray[x - 2 + z*size].y +
						vertexArray[x - 2 + (z + 1)*size].y + vertexArray[x - 2 + (z + 2)*size].y +
						vertexArray[x - 1 + (z - 1)*size].y + vertexArray[x - 1 + z*size].y +
						vertexArray[x - 1 + (z + 1)*size].y + vertexArray[x - 1 + (z + 2)*size].y +
						vertexArray[x + (z - 1)*size].y + vertexArray[x + z*size].y +
						vertexArray[x + (z + 1)*size].y + vertexArray[x + (z + 2)*size].y +
						vertexArray[x + 1 + (z - 1)*size].y + vertexArray[x + 1 + z*size].y +
						vertexArray[x + 1 + (z + 1)*size].y + vertexArray[x + 1 + (z + 2)*size].y) / 16.0;
				}
				else if (z == (size - 2)) {
					y = (
						vertexArray[x - 2 + (z - 2)*size].y + vertexArray[x - 2 + (z - 1)*size].y + vertexArray[x - 2 + z*size].y +
						vertexArray[x - 2 + (z + 1)*size].y +
						vertexArray[x - 1 + (z - 2)*size].y + vertexArray[x - 1 + (z - 1)*size].y + vertexArray[x - 1 + z*size].y +
						vertexArray[x - 1 + (z + 1)*size].y +
						vertexArray[x + (z - 2)*size].y + vertexArray[x + (z - 1)*size].y + vertexArray[x + z*size].y +
						vertexArray[x + (z + 1)*size].y +
						vertexArray[x + 1 + (z - 2)*size].y + vertexArray[x + 1 + (z - 1)*size].y + vertexArray[x + 1 + z*size].y +
						vertexArray[x + 1 + (z + 1)*size].y) / 16.0;
				}
				else {
					y = (
						vertexArray[x - 2 + (z - 2)*size].y + vertexArray[x - 2 + (z - 1)*size].y + vertexArray[x - 2 + z*size].y +
						vertexArray[x - 2 + (z + 1)*size].y + vertexArray[x - 2 + (z + 2)*size].y +
						vertexArray[x - 1 + (z - 2)*size].y + vertexArray[x - 1 + (z - 1)*size].y + vertexArray[x - 1 + z*size].y +
						vertexArray[x - 1 + (z + 1)*size].y + vertexArray[x - 1 + (z + 2)*size].y +
						vertexArray[x + (z - 2)*size].y + vertexArray[x + (z - 1)*size].y + vertexArray[x + z*size].y +
						vertexArray[x + (z + 1)*size].y + vertexArray[x + (z + 2)*size].y +
						vertexArray[x + 1 + (z - 2)*size].y + vertexArray[x + 1 + (z - 1)*size].y + vertexArray[x + 1 + z*size].y +
						vertexArray[x + 1 + (z + 1)*size].y + vertexArray[x + 1 + (z + 2)*size].y) / 20.0;
				}
			}
			else if (z == 1) {
				y = (
					vertexArray[x - 2 + (z - 1)*size].y + vertexArray[x - 2 + z*size].y +
					vertexArray[x - 2 + (z + 1)*size].y + vertexArray[x - 2 + (z + 2)*size].y +
					vertexArray[x - 1 + (z - 1)*size].y + vertexArray[x - 1 + z*size].y +
					vertexArray[x - 1 + (z + 1)*size].y + vertexArray[x - 1 + (z + 2)*size].y +
					vertexArray[x + (z - 1)*size].y + vertexArray[x + z*size].y +
					vertexArray[x + (z + 1)*size].y + vertexArray[x + (z + 2)*size].y +
					vertexArray[x + 1 + (z - 1)*size].y + vertexArray[x + 1 + z*size].y +
					vertexArray[x + 1 + (z + 1)*size].y + vertexArray[x + 1 + (z + 2)*size].y +
					vertexArray[x + 2 + (z - 1)*size].y + vertexArray[x + 2 + z*size].y +
					vertexArray[x + 2 + (z + 1)*size].y + vertexArray[x + 2 + (z + 2)*size].y) / 20.0;
			}
			else if (z == (size - 2)) {
				y = (
					vertexArray[x - 2 + (z - 2)*size].y + vertexArray[x - 2 + (z - 1)*size].y + vertexArray[x - 2 + z*size].y +
					vertexArray[x - 2 + (z + 1)*size].y +
					vertexArray[x - 1 + (z - 2)*size].y + vertexArray[x - 1 + (z - 1)*size].y + vertexArray[x - 1 + z*size].y +
					vertexArray[x - 1 + (z + 1)*size].y +
					vertexArray[x + (z - 2)*size].y + vertexArray[x + (z - 1)*size].y + vertexArray[x + z*size].y +
					vertexArray[x + (z + 1)*size].y +
					vertexArray[x + 1 + (z - 2)*size].y + vertexArray[x + 1 + (z - 1)*size].y + vertexArray[x + 1 + z*size].y +
					vertexArray[x + 1 + (z + 1)*size].y +
					vertexArray[x + 2 + (z - 2)*size].y + vertexArray[x + 2 + (z - 1)*size].y + vertexArray[x + 2 + z*size].y +
					vertexArray[x + 2 + (z + 1)*size].y) / 25.0;
			}
			else {
				y = (
					vertexArray[x - 2 + (z - 2)*size].y + vertexArray[x - 2 + (z - 1)*size].y + vertexArray[x - 2+ z*size].y +
					vertexArray[x - 2 + (z + 1)*size].y + vertexArray[x - 2 + (z + 2)*size].y +
					vertexArray[x - 1 + (z - 2)*size].y + vertexArray[x - 1 + (z - 1)*size].y + vertexArray[x - 1 + z*size].y +
					vertexArray[x - 1 + (z + 1)*size].y + vertexArray[x - 1 + (z + 2)*size].y +
					vertexArray[x + (z - 2)*size].y + vertexArray[x + (z - 1)*size].y + vertexArray[x + z*size].y +
					vertexArray[x + (z + 1)*size].y + vertexArray[x + (z + 2)*size].y +
					vertexArray[x + 1 + (z - 2)*size].y + vertexArray[x + 1 + (z - 1)*size].y + vertexArray[x + 1 + z*size].y +
					vertexArray[x + 1 + (z + 1)*size].y + vertexArray[x + 1 + (z + 2)*size].y +
					vertexArray[x + 2 + (z - 2)*size].y + vertexArray[x + 2 + (z - 1)*size].y + vertexArray[x + 2 + z*size].y +
					vertexArray[x + 2 + (z + 1)*size].y + vertexArray[x + 2 + (z + 2)*size].y) / 25.0;
			}
			smoothedVertexArray[x + z*size] = vec3(x, y, z);
		}
	}

	for (int x = 0; x < size - 1; x++) {
		for (int z = 0; z < size - 1; z++) {

			indexArray[(x + z * (size - 1)) * 6 + 0] = x + z * size;
			indexArray[(x + z * (size - 1)) * 6 + 1] = x + (z + 1) * size;
			indexArray[(x + z * (size - 1)) * 6 + 2] = x + 1 + z * size;

			indexArray[(x + z * (size - 1)) * 6 + 3] = x + 1 + z * size;
			indexArray[(x + z * (size - 1)) * 6 + 4] = x + (z + 1) * size;
			indexArray[(x + z * (size - 1)) * 6 + 5] = x + 1 + (z + 1) * size;
		}
	}

	for (int x = 0; x < size; x++) {
		for (int z = 0; z < size; z++) {
			if (x == 0 || x == size - 1 || z == 0 || z == size - 1)
			{
				normalArray[x + z*size] = vec3(0, 1, 0);
			}
			else {
				
				vec3 middle = smoothedVertexArray[x + z * size];

				vec3 V1 = smoothedVertexArray[x - 1 + (z - 1) * size] - middle;
				vec3 V2 = smoothedVertexArray[x + (z - 1) * size] - middle;
				vec3 V3 = smoothedVertexArray[x + 1 + (z - 1) * size] - middle;

				vec3 V4 = smoothedVertexArray[x - 1 + z * size] - middle;
				vec3 V5 = smoothedVertexArray[x + 1 + z * size] - middle;

				vec3 V6 = smoothedVertexArray[x - 1 + (z + 1) * size] - middle;
				vec3 V7 = smoothedVertexArray[x + (z + 1) * size] - middle;
				vec3 V8 = smoothedVertexArray[x + 1 + (z + 1) * size] - middle;

				vec3 N1 = normalize(cross(V1, V4));
				vec3 N2 = normalize(cross(V4, V6));
				vec3 N3 = normalize(cross(V6, V7));
				vec3 N4 = normalize(cross(V7, V8));

				vec3 N5 = normalize(cross(V8, V5));
				vec3 N6 = normalize(cross(V5, V3));
				vec3 N7 = normalize(cross(V3, V2));
				vec3 N8 = normalize(cross(V2, V1));

				normalArray[x + z*size] = (N1 + N2 + N3 + N4 + N5 + N6 + N7 + N8) / 8.0f;
			}
		}
	}

	for (int x = 0; x < size; x++) {
		for (int z = 0; z < size; z++) {

			texCoordArray[x + z*size].x = (float)x / (size - 1);
			texCoordArray[x + z*size].y = (float)z / (size - 1);
		}
	}

	for (int x = 0; x < size; x++) {
		for (int z = 0; z < size; z++) {

			finalVertexArray[x + z*size] = (Vertex(smoothedVertexArray[x + z*size], texCoordArray[x + z*size], normalArray[x + z*size]));
		}
	}

	Terrain terrain(finalVertexArray, indexArray, numIndices, vertexCount);

	return terrain;
}



