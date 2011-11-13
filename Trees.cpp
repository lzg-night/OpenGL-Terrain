#include "StdAfx.h"
#include "Trees.h"


Trees::Trees(void) {
	treeLvl = 6;
	leaves = false;
}


Trees::~Trees(void) {
}

void Trees::DecreaseComplexity(void) {
	glDeleteLists(displayList, 1);
	if (treeLvl > 1) treeLvl--;
	Init();
}

void Trees::IncreaseComplexity(void) {
	glDeleteLists(displayList, 1);
	if (treeLvl < 8) treeLvl++;
	Init();
}

void Trees::Regen(void) {
	glDeleteLists(displayList, 1);
	Init();
}

int Trees::GetComplexity(void) {
	return treeLvl;
}

/**
* Initialise by loading textures etc.
*/
void Trees::Init(void) {
	//load textures
	RawLoader rawLoader;
	Shapes shapes;

	quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GL_TRUE);
	gluQuadricTexture(quadric, 1);

	woodTexture = rawLoader.LoadTextureRAW("woodtexture.raw", 1, 512, 512);

	// load the skybox to a display list
	displayList = glGenLists(1);
	glNewList(displayList, GL_COMPILE);

	glEnable(GL_TEXTURE_2D); // enable drawing the texture
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, woodTexture); // bind the texture
	tree(treeLvl);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glEndList();
}

void Trees::Display(void) {
	glPushMatrix();
	glTranslatef(440.0f, 203.0f, 450.0f);
	glScalef(17.0f, 17.0f, 17.0f);
	glCallList(displayList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(570.0f, 167.0f, 520.0f);
	glScalef(17.0f, 17.0f, 17.0f);
	glCallList(displayList);
	glPopMatrix();
}

// Draw the tree and leaves recursively
void Trees::tree(int level)  {
	srand(rand());

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