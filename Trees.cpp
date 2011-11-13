#include "StdAfx.h"
#include "Trees.h"


Trees::Trees(void) {
	treeLvl = 4;
	leaves = false;
	loaded = false;
}


Trees::~Trees(void) {
}

void Trees::DecreaseComplexity(void) {
	for (unsigned int i = 0; i < numTrees; i++) {
		glDeleteLists(treeLists[i], 1);
	}

	if (treeLvl > 1) treeLvl--;

	Init();
}

void Trees::IncreaseComplexity(void) {
	for (unsigned int i = 0; i < numTrees; i++) {
		glDeleteLists(treeLists[i], 1);
	}

	if (treeLvl < 8) treeLvl++;

	Init();
}

void Trees::Regen(void) {
	for (unsigned int i = 0; i < numTrees; i++) {
		glDeleteLists(treeLists[i], 1);
	}
	
	Init();
}

int Trees::GetComplexity(void) {
	return treeLvl;
}

void Trees::SetUpHeights(Terrain &terrain, float lavaHeight) {
	srand(rand());
	
	for (int i = 0; i < numTrees; i++) {
		SetUpHeight(i, terrain, lavaHeight);		
	}
}

void Trees::SetUpHeight(int i, Terrain &terrain, float lavaHeight) {
	float x = (float)random(1, 1024);
	float z = (float)random(1, 1024);
	float y = terrain.GetHeightAt((unsigned int)x, (unsigned int)z);

	if (y < (lavaHeight + 50)) {
		SetUpHeight(i, terrain, lavaHeight);
	} else {
		randomHeights[i].x = x;
		randomHeights[i].y = y;
		randomHeights[i].z = z;
	}
}

/**
* Initialise by loading textures etc.
*/
void Trees::Init() {
	//load textures
	RawLoader rawLoader;
	Shapes shapes;

	if (!loaded) {
		quadric = gluNewQuadric();
		gluQuadricNormals(quadric, GL_TRUE);
		gluQuadricTexture(quadric, 1);

		woodTexture = rawLoader.LoadTextureRAW("woodtexture.raw", 1, 512, 512);
		loaded = true;
	}

	srand(rand());

	for (unsigned int i = 0; i < numTrees; i++) {
		treeLists[i] = glGenLists(1);
		glNewList(treeLists[i], GL_COMPILE);
		glScalef(17.0f, 17.0f, 17.0f);
		tree(treeLvl);
		glEndList();
	}
}

void Trees::Display() {
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, woodTexture); // bind the texture

	for (unsigned int i = 0; i < numTrees; i++) {
		glPushMatrix();
		glTranslatef(randomHeights[i].x, randomHeights[i].y, randomHeights[i].z);
		glCallList(treeLists[i]);
		glPopMatrix();
	}

	glPopMatrix();
}

// Draw the tree and leaves recursively
void Trees::tree(int level)  {
	if (level == 0) {
		glPushMatrix();
		glRotatef(-90.0f, 1.0f, 0, 0);
		gluCylinder(quadric, 0.102, 0.08, 1.025, 8, 1);
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glTranslatef(0, 1.0f, 0);
	} else {
		// trunk
		tree(0);

		for (int i = 0; i < 3; i++) {
			glPushMatrix();
			glScalef(0.6f, 0.6f, 0.6f);

			// Random angle for the three branches (per parent branch)
			float randAngle = (float)random(0 + (i * 100), ((i + 1) * 100));
			glRotatef(randAngle, 0.0, 1.0, 0.0);
			randAngle = (float)random(25, 35);
			glRotatef(-randAngle, 0.0, 0.0, 1.0);
			tree(level - 1);
			glPopMatrix();
		}
	}
}


int Trees::random(int min, int max) {
	int randNum = (rand() % max) + min;

	return randNum;
}