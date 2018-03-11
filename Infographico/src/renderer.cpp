#include "renderer.h"
#include <math.h>

void Renderer::setup() {
	ofSetFrameRate(60);
	ofSetBackgroundColor(191);
	//ofEnableDepthTest();

	screenWidth = ofGetWidth();
	screenHeight = ofGetHeight();

	is2D = true;

	//Curseur
	mousePressX = 0;
	mousePressY = 0;
	mousePosX = 0;
	mousePosY = 0;
	isMouseButtonPressed= false;

	//Modele 3D
	isGenererModele3D = false;
	//eclairage
	ofEnableLighting();
	light.setAmbientColor(ofColor(0, 255, 0));
	light.setDiffuseColor(ofColor(255, 255, 255));
	light.setPosition(0.0f, 0.0f, 1000.0f);
	light.enable();
	

	//Ajout des �couteurs de l'interface
	boutonEllipse.addListener(this, &Renderer::boutonEllipsePressed);
	boutonRectangle.addListener(this, &Renderer::boutonRectanglePressed);
	boutonTriangle.addListener(this, &Renderer::boutonTrianglePressed);
	boutonTriangleRectangle.addListener(this, &Renderer::boutonTriangleRectanglePressed);
	boutonLigne.addListener(this, &Renderer::boutonLignePressed);

	boutonMode2D.addListener(this, &Renderer::boutonMode2DToggled);
	boutonMode3D.addListener(this, &Renderer::boutonMode3DToggled);
	boutonImporterImage.addListener(this, &Renderer::importImagePressed);
	boutonExporterImage.addListener(this, &Renderer::boutonExporterImagePressed);
	boutonUndo.addListener(this, &Renderer::boutonUndoPressed);
	boutonRedo.addListener(this, &Renderer::boutonRedoPressed);

	boutonTetraedre.addListener(this, &Renderer::boutonTetraedrePressed);
	boutonOctaedre.addListener(this, &Renderer::boutonOctaedrePressed);
	boutonLapin.addListener(this, &Renderer::boutonLapinPressed);
	boutonDragon.addListener(this, &Renderer::boutonDragonPressed);

	imageComposition.addListener(this, &Renderer::compositionToggled);
	convolutionFilter.addListener(this, &Renderer::convolutionToggled);
	proceduralTexture.addListener(this, &Renderer::proceduralToggled);
	


	//menu
	guiMenu.setup();
	guiMenu.setName("Menu");
	guiMenu.add(boutonMode2D.set("Mode 2D", true));
	guiMenu.add(boutonMode3D.set("Mode 3D", false));
	guiMenu.add(boutonImporterImage.setup("Importer image"));
	guiMenu.add(labelSourceImage.set("None"));
	guiMenu.add(toggleAfficherImage.set("Afficher image", false));
	guiMenu.add(boutonExporterImage.setup("Exporter image"));
	guiMenu.add(labelTraitementImage.setup("Traitement d'image", ""));
	guiMenu.add(imageComposition.set("Composition", false));
	guiMenu.add(convolutionFilter.set("Filtre de convolution", false));
	guiMenu.add(proceduralTexture.set("Texture Procedurale", false));

	guiMenu.setPosition(0, 0);
	

	//Interface dessin
	guiDessin.setup();
	guiDessin.setName("Dessin 2D");
	guiDessin.add(sliderPosX.setup("Position en X", screenWidth / 2, 0, screenWidth));
	guiDessin.add(sliderPosY.setup("Position en Y", screenHeight / 2, 0, screenHeight));
	guiDessin.add(sliderRotation.setup("Rotation", 0, 0, 360));

	//Conversion de la largeur (float) en string
	largeurForme = 0.0;
	std::ostringstream oss1;
	oss1 << largeurForme;
	guiDessin.add(labelLargeur.setup("Largeur", oss1.str()));
	//Conversion de l'hauteur (float) en string
	hauteurForme = 0.0;
	std::ostringstream oss2;
	oss2 << hauteurForme;
	guiDessin.add(labelHauteur.setup("Hauteur", oss2.str()));
	guiDessin.add(choixDeForme.setup("Choix de Forme", ""));
	guiDessin.add(boutonLigne.setup("Ligne"));
	guiDessin.add(boutonRectangle.setup("Rectangle"));
	guiDessin.add(boutonTriangle.setup("Triangle"));
	guiDessin.add(boutonEllipse.setup("Cercle"));
	guiDessin.add(boutonTriangleRectangle.setup("Triangle rectangle"));

	guiDessin.add(labelPropriteteDuDessin.setup("Propriete du dessin", ""));
	guiDessin.add(sliderEpaisseurLigneContour.setup("Epaisseur contour", 1, 1, 50));

	guiDessin.add(boutonUndo.setup("<-Undo"));
	guiDessin.add(boutonRedo.setup("Redo->"));

	guiDessin.setPosition(screenWidth - guiDessin.getWidth(), 0);


	//interface pour mod�le 3D
	guiModel3D.setup();
	guiModel3D.setName("Dessin 3D");

	guiModel3D.add(labelGenerationModel3D.setup("Modele 3D", ""));
	guiModel3D.add(boutonLapin.setup("Generer Lapin"));
	guiModel3D.add(boutonDragon.setup("Generer Dragon"));

	guiModel3D.add(labelRotation3D.setup("Rotation Modele 3D", ""));
	
	guiModel3D.add(sliderRotation3DX.setup("Rotation X", 0, 0, 360));
	guiModel3D.add(sliderRotation3DY.setup("Rotation Y", 0, 0, 360));
	guiModel3D.add(sliderRotation3DZ.setup("Rotation Z", 0, 0, 360));

	guiModel3D.add(labelProportion3D.setup("Proportion modele 3D", ""));

	guiModel3D.add(sliderProportion3DX.setup("Proportion en X", 1, 0.5, 2));
	guiModel3D.add(sliderProportion3DY.setup("Proportion en Y", 1, 0.5, 2));
	guiModel3D.add(sliderProportion3DZ.setup("Proportion en Z", 1, 0.5, 2));

	guiModel3D.add(labelEffet.setup("Effets", ""));
	guiModel3D.add(animer.setup("Animer", false));
	guiModel3D.add(dessierBoite.setup("Dessiner boite", false));

	//primitive geo
	guiModel3D.add(labelGenerationPrimitiveGeometrique.setup("Primitive geometrique 3D", ""));
	guiModel3D.add(boutonTetraedre.setup("Tetraedre"));
	guiModel3D.add(boutonOctaedre.setup("Octaedre"));

	guiModel3D.add(labelPrimitiveGeo.setup("Rotation Primitive Geo", ""));
	guiModel3D.add(buttonRotation3DX.setup("Activer rotation en X"));
	guiModel3D.add(buttonRotation3DY.setup("Activer rotation en Y"));
	guiModel3D.add(buttonRotation3DZ.setup("Activer rotation en Z"));
	guiModel3D.add(sliderRotationPrimitiveGeo.setup("Rotation sur X", 0, 0, 360));
	isRotation3DXPrimitiveGeo = true;
	isRotation3DYPrimitiveGeo = false;
	isRotation3DZPrimitiveGeo = false;
	

	guiModel3D.setPosition(-guiModel3D.getWidth(), 0);

	//color picker
	rgbMode.addListener(this, &Renderer::rgbModeSwitched);
	colorPickerGUI.setup();
	colorPickerGUI.add(rgbMode.setup("RGB mode", true));
	colorPickerGUI.add(redOrHue);
	colorPickerGUI.add(greenOrSaturation);
	colorPickerGUI.add(blueOrBrightness);
	colorPickerGUI.add(alpha.set("alpha", 255, 0, 255));
	

	buttonRotation3DX.addListener(this, &Renderer::buttonRotation3DXPressed);
	buttonRotation3DY.addListener(this, &Renderer::buttonRotation3DYPressed);
	buttonRotation3DZ.addListener(this, &Renderer::buttonRotation3DZPressed);

	//animer
	animationGrossit = true;

	// importer une image situ�e dans ./bin/data
	currentImage.load("field.jpg");

	// s�lectionner le filtre de convolution par d�faut
	kernel_type = ConvolutionKernel::identity;
	kernel_name = "identity";

	// initialisation de l'interface graphique de composition de texture
	boutonImporterTexture1.addListener(this, &Renderer::boutonImporterImageTexture1);
	boutonImporterTexture2.addListener(this, &Renderer::boutonImporterImageTexture2);
	textureCompositionGUI.setup();
	textureCompositionGUI.setName("TEXTURE COMPOSITION");
	textureCompositionGUI.add(boutonImporterTexture1.setup("Import texture 1"));
	textureCompositionGUI.add(textureName1.set("None"));
	textureCompositionGUI.add(boutonImporterTexture2.setup("Import texture 2"));
	textureCompositionGUI.add(textureName2.set("None"));
	textureCompositionGUI.add(compositionAdd.set("Composition: ADD", false));

	// initialisation de l'interface graphique pour filtrage par convolution
	aiguiser.addListener(this, &Renderer::aiguiserToggled);
	detectionBordure.addListener(this, &Renderer::detectionBordureToggled);
	bosseler.addListener(this, &Renderer::bosselerToggled);
	flou.addListener(this, &Renderer::flouToggled);
	filtrageConvolutionGUI.setup();
	filtrageConvolutionGUI.setName("CONVOLUTION FILTER");
	filtrageConvolutionGUI.add(aiguiser.set("Sharpen", false));
	filtrageConvolutionGUI.add(detectionBordure.set("Edge Detection", false));
	filtrageConvolutionGUI.add(bosseler.set("Emboss", false));
	filtrageConvolutionGUI.add(flou.set("Blur", false));

	// initialisation de l'interface graphique pour texture proc�dural
	binaryTree.addListener(this, &Renderer::binaryTreeToggled);
	ternaryTree.addListener(this, &Renderer::ternaryTreeToggled);
	textureProceduralGUI.setup();
	textureProceduralGUI.setName("PROCEDURAL TEXTURE");
	textureProceduralGUI.add(binaryTree.set("Binary Tree", false));
	textureProceduralGUI.add(ternaryTree.set("Ternary Tree", false));
	textureProceduralGUI.add(branchLength.set("branch length", 100, 100, 300));
	textureProceduralGUI.add(angle.set("branch angle", 30, 0, 360));

}

void Renderer::draw() {
	//Color picker
	if (rgbMode){
		currentColor = ofColor(redOrHue, greenOrSaturation, blueOrBrightness, alpha);
	}
	else {
		currentColor = ofColor::fromHsb(redOrHue, greenOrSaturation, blueOrBrightness, alpha);
	}
	
	if (compositionAdd && is_texture1_loaded && is_texture2_loaded) {
		//if (texture1.getWidth() > 0 && texture1.getHeight() > 0 && texture2.getWidth() > 0 && texture2.getHeight() > 0)
		//	ofSetWindowShape(max(texture1.getWidth(), texture2.getWidth()), max(texture1.getHeight(), texture2.getHeight()));
		//ofEnableBlendMode(OF_BLENDMODE_ADD);
		texture1.draw(guiMenu.getWidth(), 0, texture1.getWidth(), texture1.getHeight());
		ofSetColor(255, 255, 255, 150);
		texture2.draw(guiMenu.getWidth(), 0, texture2.getWidth(), texture2.getHeight());
		ofSetColor(255, 255, 255, 255);
		//ofDisableBlendMode();
	}
	else if (aiguiser || detectionBordure || bosseler || flou) {
		filteredImage.allocate(currentImage.getWidth(), currentImage.getHeight(), OF_IMAGE_COLOR);
		filter();
		filteredImage.draw(guiMenu.getWidth(), 0, currentImage.getWidth(), currentImage.getHeight());
	}
	else if (ternaryTree || binaryTree) {
		ofSetBackgroundColor(255, 255, 255, 255);
		ofSetColor(0, 0, 0, 255);
		ofSetLineWidth(2);

		if (ternaryTree) {
			ofPushMatrix();
			ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
			drawTernaryTree(branchLength);
			ofPopMatrix();
		}
		else if (binaryTree) {
			ofPushMatrix();
			ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
			drawBinaryTree(branchLength);
			ofPopMatrix();
		}
	}
	else if(toggleAfficherImage) {
		currentImage.draw(guiMenu.getWidth(), 0, currentImage.getWidth(), currentImage.getHeight());
	}
		

	//GUI et affichage
	guiMenu.setPosition(0, 0);
	guiMenu.draw();
	dessinerCurseur(mousePosX, mousePosY);
	if (!is2D) {
		guiModel3D.setPosition(screenWidth - guiDessin.getWidth(), 0);
		guiModel3D.draw();
		if (isGenererModele3D) {
			genererModele3D();
			if (dessierBoite) {
				genererBoite();
			}
		}
		else if (isGenererTetraedre) {
			genererTetraedre();
		}
		else if (isGenererOctaedre) {
			genererOctaedre();
		}
	}
	else {
		guiDessin.setPosition(screenWidth - guiDessin.getWidth(), 0);
		guiDessin.draw();
	}

	// dessiner le color picker
	colorPickerGUI.setPosition(0, guiMenu.getHeight());
	colorPickerGUI.draw();
	ofSetColor(currentColor);
	ofDrawRectangle(0, colorPickerGUI.getPosition().y + colorPickerGUI.getHeight(), colorPickerGUI.getWidth(), 20);
	ofNoFill();
	ofSetLineWidth(5);
	ofSetColor(0, 0, 0);
	ofDrawRectangle(0, colorPickerGUI.getPosition().y + colorPickerGUI.getHeight(), colorPickerGUI.getWidth(), 20);
	ofFill();
	ofSetColor(255, 255, 255);

	// definir position de l'interface graphique de composition de texture
	if (imageComposition) {
		textureCompositionGUI.setPosition(0, colorPickerGUI.getPosition().y + colorPickerGUI.getHeight() + 20);
		textureCompositionGUI.draw();
	}
	// definir position de l'interface graphique de filtrage par convolution
	if (convolutionFilter) {
		filtrageConvolutionGUI.setPosition(0, colorPickerGUI.getPosition().y + colorPickerGUI.getHeight() + 20);
		filtrageConvolutionGUI.draw();
	}
	// definir position de l'interface graphique de texture proc�dural
	if (proceduralTexture) {
		textureProceduralGUI.setPosition(0, colorPickerGUI.getPosition().y + colorPickerGUI.getHeight() + 20);
		textureProceduralGUI.draw();
	}
}

void Renderer::update() {
	if (isGenererModele3D) {
		if (animer) {
			animerMaillage();
		}
	}
}

void Renderer::image_export(const string name, const string extension) const {
	ofImage image;

	// extraire des donn�es temporelles format�es
	string time_stamp = ofGetTimestampString("-%y%m%d-%H%M%S-%i");

	// g�n�rer un nom de fichier unique et ordonn�
	string file_name = name + time_stamp + "." + extension;

	// capturer le contenu du framebuffer actif
	image.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());

	// sauvegarder le fichier image
	image.save(file_name);

	ofLog() << "<export image: " << file_name << ">";
}

//*******************************************//
//*********Ecouteur de l'interface***********//
//*******************************************//

void Renderer::boutonLignePressed() {

}

void Renderer::boutonEllipsePressed() {

}

void Renderer::boutonRectanglePressed() {

}
void Renderer::boutonTrianglePressed() {

}

void Renderer::boutonTriangleRectanglePressed() {

}

//Change l'interface de modele 3D pour l'interface de dessin 2D
void Renderer::boutonMode2DToggled(bool &mode2D) {
	if (mode2D) {
		is2D = true;
		guiDessin.setPosition(screenWidth - guiDessin.getWidth(), 0);
		guiModel3D.setPosition(-guiModel3D.getWidth(), 0);
		boutonMode3D.set(false);
	}
}

//Change l'interface de dessin pour l'interface de modele 3D
void Renderer::boutonMode3DToggled(bool &mode3D) {
	if (mode3D) {
		is2D = false;
		guiDessin.setPosition(-guiDessin.getWidth(), 0);
		guiModel3D.setPosition(screenWidth - guiModel3D.getWidth(), 0);
		boutonMode2D.set(false);
	}
}


void Renderer::boutonExporterImagePressed() {
	

}

void Renderer::boutonUndoPressed() {

}

void Renderer::boutonRedoPressed() {

}

void Renderer::boutonTetraedrePressed() {
	isGenererModele3D = false;
	isGenererOctaedre = false;
	isGenererTetraedre = true;

}

void Renderer::boutonOctaedrePressed() {
	isGenererModele3D = false;
	isGenererTetraedre = false;
	isGenererOctaedre = true;

}

//Si on appuie sur le bouton lapin, le modele 3D devient le lapin
void Renderer::boutonLapinPressed() {
	isGenererOctaedre = false;
	isGenererTetraedre = false;
	modele.loadModel("lapin.obj");
	isGenererLapin = true;
	isGenererDragon = false;
	isGenererModele3D = true;
}

//Si on appuie sur le bouton dragon, le modele 3D devient le dragon
void Renderer::boutonDragonPressed(){
	isGenererOctaedre = false;
	isGenererTetraedre = false;
	modele.loadModel("dragon.obj");
	isGenererDragon = true;
	isGenererLapin = false;
	isGenererModele3D = true;
}

//***************************************************//
//*******************Curseurs************************//
//***************************************************//

//Determine quel curseur utiliser 
void Renderer::dessinerCurseur(float x, float y) const {
	ofSetLineWidth(2);

	if (is2D) {
		dessinerCurseurTriforce(x, y);
	}
	else if(!is2D){
		if (isGenererModele3D) {
			dessinerCurseurVise(x, y);
		}
		else if (isGenererTetraedre || isGenererOctaedre) {
			dessinerCurseurPoint(x, y);
		}
		else {
			dessinerCurseurEtoile(x, y);
		}
	}	
}

//Curseur Etoile
void Renderer::dessinerCurseurEtoile(float x, float y) const {

	float taille = 10.0f;
	if (isMouseButtonPressed) {
		ofSetColor(255, 0, 0);
	}
	else {
		ofSetColor(223, 238, 26);
	}
	ofDrawLine(x, y - (taille / 4) - taille, x+taille/4, y - (taille / 4));
	ofDrawLine(x, y - (taille / 4) - taille, x-taille/4, y - (taille / 4));
	ofDrawLine(x + taille / 4, y - (taille / 4), x + (taille / 4)+taille, y);
	ofDrawLine(x - taille / 4, y - (taille / 4), x - (taille / 4) - taille, y);
	ofDrawLine(x + (taille / 4) + taille, y, x + taille / 4,y + (taille / 2) - (taille / 4));
	ofDrawLine(x - (taille / 4) - taille, y, x - taille / 4, y + (taille / 2) - (taille / 4));
	ofDrawLine(x + taille / 4, y + (taille / 2) - (taille / 4), x, y+(taille)+(taille/2) - (taille / 4));
	ofDrawLine(x - taille / 4, y + (taille / 2) - (taille / 4), x, y + taille + (taille / 2) - (taille / 4));
}

//Curseur Triforce
void Renderer::dessinerCurseurTriforce(float x, float y) const {
	float taille = 20.0f;
	
	if (isMouseButtonPressed) {
		ofSetColor(255,0,0);
	}
	else {
		ofSetColor(223, 238, 26);
	}
	ofDrawLine(x, y, x, y+ taille);
	ofDrawLine(x, y, x - taille, y);
	ofDrawLine(x, y+ taille, x- taille, y);
	ofDrawLine(x, y+ taille / 2, x- taille/ 2, y);
	ofDrawLine(x-taille/2, y, x- taille / 2, y+ taille / 2);
	ofDrawLine(x, y+ taille / 2, x-taille/2, y+ taille / 2);
}

//Curseur Point
void Renderer::dessinerCurseurPoint(float x, float y) const {
	if (isMouseButtonPressed) {
		ofSetColor(255, 0, 0);
	}
	else {
		ofSetColor(0);
	}
	ofDrawCircle(x, y, 5);
}

//Curseur Vise
void Renderer::dessinerCurseurVise(float x, float y) const {
	float offset = 5.0f;
	float taille = 12.0f;
	
	ofSetColor(0);
	ofDrawLine(x+offset, y, x+offset+taille, y);
	ofDrawLine(x-offset, y, x-offset-taille, y);
	ofDrawLine(x, y-offset, x, y-offset-taille);
	ofDrawLine(x, y+offset, x, y+offset+taille);

	if (isMouseButtonPressed) {
		ofSetColor(255, 0, 0);
	}
	else {
		ofSetColor(0);
	}
	ofDrawCircle(x, y, 3);
}

//Curseur Fleche
void Renderer::dessinerCurseurFleche(float x, float y) const {

	float taille = 40.0f;
	ofSetColor(0);
	ofDrawArrow(ofVec3f(x, y, 0), ofVec3f(x - taille, y + taille, 0));
	ofDrawLine(x - taille, y + taille, x - taille - (taille*0.2), y + taille);
	ofDrawLine(x - taille, y + taille, x - taille, y + taille + (taille*0.2));

	if (isMouseButtonPressed) {
		ofSetColor(255, 0, 0);
	}
	else {
		ofSetColor(0);
	}
	ofDrawTriangle(x, y, x - taille / 4, y, x, y + taille / 4);
}

//Curseur Peace
void Renderer::dessinerCurseurPeace(float x, float y) const {

	float taille = 20.0f;
	float racineDe2 = 1.4142;
	if (isMouseButtonPressed) {
		ofSetColor(0, 255, 0);
	}
	else {
		ofSetColor(0);
	}
	ofNoFill();
	ofDrawCircle(x, y, taille);
	ofFill();
	ofDrawLine(x, y, x, y-taille);
	ofDrawLine(x, y, x+ (taille / racineDe2), y+ (taille / racineDe2));
	ofDrawLine(x, y, x- (taille / racineDe2), y+ (taille / racineDe2));
}


void Renderer::genererModele3D() {
	ofEnableDepthTest();
	ofFill();
	ofSetLineWidth(1);
	ofSetColor(1);

	//On ajuste la proportion du modele selon les sliders de proportion
	modele.setScale(sliderProportion3DX / 2, sliderProportion3DY / 2, sliderProportion3DZ / 2);
	//On ajuste l'angle de rotation selon les sliders de rotation
	modele.setRotation(0, sliderRotation3DX, 1, 0, 0);
	modele.setRotation(1, sliderRotation3DY, 0, 1, 0);
	modele.setRotation(2, sliderRotation3DZ, 0, 0, 1);

	modele.setPosition(screenWidth / 2, screenHeight*0.75, 0);
	modele.draw(OF_MESH_FILL);
	modele.drawFaces();

	ofDisableDepthTest();
}

//genere un tetraedre
void Renderer::genererTetraedre() {
	ofFill();
	ofSetColor(currentColor);
	float taille = screenHeight/4;
	float origineX = screenWidth / 2;
	float origineY = screenHeight/2;
	float origineZ = 0;
	

	//origine
	ofVec3f origineTetraedre = ofVec3f(origineX, origineY, origineZ);
	/****************************/
	/***generation des points****/
	/***x' = x cos f - y sin f***/
	/***y' = y cos f + x sin f***/
	/****************************/

	//sommet du haut
	ofVec3f sommet1 = ofVec3f(0, -(taille*0.75) / 2, 0);

	//rotation x
	if (isRotation3DXPrimitiveGeo) {
		sommet1 = ofVec3f(0,
			-((taille*0.75) / 2)*cos(ofDegToRad(sliderRotationPrimitiveGeo)),
			-((taille*0.75) / 2)*sin(ofDegToRad(sliderRotationPrimitiveGeo)));
	}

	//rotation z
	else if (isRotation3DZPrimitiveGeo) {
		sommet1 = ofVec3f(((taille*0.75) / 2)*sin(ofDegToRad(sliderRotationPrimitiveGeo)),
			((-(taille*0.75) / 2)*cos(ofDegToRad(sliderRotationPrimitiveGeo))), 0);
	}
	//ajoute l'origine
	sommet1 += origineTetraedre;

	//sommet de derriere
	ofVec3f sommet2 = ofVec3f(0, (taille*0.75) / 2, -(taille / (2* (sqrt(0.75)))));

	//rotation x
	if (isRotation3DXPrimitiveGeo) {
		sommet2 = ofVec3f(0,
			((taille*0.75) / 2)*cos(ofDegToRad(sliderRotationPrimitiveGeo)) + (-(taille / (2 * (sqrt(0.75)))))*sin(ofDegToRad(sliderRotationPrimitiveGeo)),
			-(taille / (2 * (sqrt(0.75))))*cos(ofDegToRad(sliderRotationPrimitiveGeo)) - ((taille*0.75) / 2)*sin(ofDegToRad(sliderRotationPrimitiveGeo)));
	}

	//rotation y
	else if (isRotation3DYPrimitiveGeo) {
		sommet2 = ofVec3f((taille / (2 * (sqrt(0.75))))*sin(ofDegToRad(sliderRotationPrimitiveGeo)), (taille*0.75) / 2, -(taille / (2 * (sqrt(0.75))))*cos(ofDegToRad(sliderRotationPrimitiveGeo)));
	}

	//rotation z
	else if (isRotation3DZPrimitiveGeo) {
		sommet2 = ofVec3f(-((taille*0.75) / 2)*sin(ofDegToRad(sliderRotationPrimitiveGeo)), ((taille*0.75) / 2)*cos(ofDegToRad(sliderRotationPrimitiveGeo)), -(taille / (2 * (sqrt(0.75)))));

	}

	//ajoute l'origine
	sommet2 += origineTetraedre;


	//sommet avant gauche
	ofVec3f sommet3 = ofVec3f(-(taille/2), (taille*0.75)/2, (taille / 4* sqrt(0.75)));

	//rotation x
	if (isRotation3DXPrimitiveGeo) {
		sommet3 = ofVec3f(-(taille / 2),
			(((taille*0.75) / 2)*cos(ofDegToRad(sliderRotationPrimitiveGeo)) + (taille / 4 * sqrt(0.75))*sin(ofDegToRad(sliderRotationPrimitiveGeo))),
			((taille / 4 * sqrt(0.75))*cos(ofDegToRad(sliderRotationPrimitiveGeo)) - ((taille*0.75) / 2)*sin(ofDegToRad(sliderRotationPrimitiveGeo))));
	}
	
	//rotation y
	else if (isRotation3DYPrimitiveGeo) {
		sommet3 = ofVec3f(-(taille / 2)*cos(ofDegToRad(sliderRotationPrimitiveGeo)) - (taille / 4 * sqrt(0.75))*sin(ofDegToRad(sliderRotationPrimitiveGeo)),
			(taille*0.75) / 2,
			(taille / 4 * sqrt(0.75))*cos(ofDegToRad(sliderRotationPrimitiveGeo)) + (-(taille / 2)*sin(ofDegToRad(sliderRotationPrimitiveGeo))));
	}
	
	//rotation z
	else if (isRotation3DZPrimitiveGeo) {
		sommet3 = ofVec3f(((-(taille / 2))*cos(ofDegToRad(sliderRotationPrimitiveGeo))) - (((taille*0.75) / 2)*sin(ofDegToRad(sliderRotationPrimitiveGeo))),
			(((taille*0.75) / 2)*cos(ofDegToRad(sliderRotationPrimitiveGeo))) + ((-(taille / 2))*sin(ofDegToRad(sliderRotationPrimitiveGeo))),
			(taille / 4 * sqrt(0.75)));
	}

	//ajoute l'origine
	sommet3 += origineTetraedre;


	//sommet avant droit
	ofVec3f sommet4 = ofVec3f((taille / 2), (taille*0.75) / 2, (taille / 4 * sqrt(0.75)));

	//rotation x
	if (isRotation3DXPrimitiveGeo) {
		sommet4 = ofVec3f((taille / 2),
			(((taille*0.75) / 2)*cos(ofDegToRad(sliderRotationPrimitiveGeo)) + (taille / 4 * sqrt(0.75))*sin(ofDegToRad(sliderRotationPrimitiveGeo))),
			((taille / 4 * sqrt(0.75))*cos(ofDegToRad(sliderRotationPrimitiveGeo)) - ((taille*0.75) / 2)*sin(ofDegToRad(sliderRotationPrimitiveGeo))));
	}
	
	//rotation y
	else if (isRotation3DYPrimitiveGeo) {
		sommet4 = ofVec3f((taille / 2)*cos(ofDegToRad(sliderRotationPrimitiveGeo)) - (taille / 4 * sqrt(0.75))*sin(ofDegToRad(sliderRotationPrimitiveGeo)),
			(taille*0.75) / 2,
			(taille / 4 * sqrt(0.75))*cos(ofDegToRad(sliderRotationPrimitiveGeo)) + ((taille / 2)*sin(ofDegToRad(sliderRotationPrimitiveGeo))));
	}
	
	//rotation z
	else if (isRotation3DZPrimitiveGeo) {
		sommet4 = ofVec3f((((taille / 2))*cos(ofDegToRad(sliderRotationPrimitiveGeo))) - (((taille*0.75) / 2)*sin(ofDegToRad(sliderRotationPrimitiveGeo))),
			(((taille*0.75) / 2)*cos(ofDegToRad(sliderRotationPrimitiveGeo))) + (((taille / 2))*sin(ofDegToRad(sliderRotationPrimitiveGeo))),
			(taille / 4 * sqrt(0.75)));
	}

	//ajoute l'origine
	sommet4 += origineTetraedre;
	
	//dessin des triangles avec les points
	ofDrawTriangle(sommet1, sommet2, sommet3);
	ofDrawTriangle(sommet1, sommet2, sommet4);
	ofDrawTriangle(sommet1, sommet3, sommet4);
	ofDrawTriangle(sommet2, sommet3, sommet4);

	//dessin des aretes
	ofSetColor(255, 255, 255);
	ofDrawLine(sommet1, sommet2);
	ofDrawLine(sommet1, sommet3);
	ofDrawLine(sommet1, sommet4);
	ofDrawLine(sommet2, sommet3);
	ofDrawLine(sommet2, sommet4);
	ofDrawLine(sommet3, sommet4);
}


void Renderer::genererOctaedre() {
	ofFill();
	ofSetColor(currentColor);
	float taille = screenHeight / 4;
	float origineX = screenWidth / 2;
	float origineY = screenHeight / 2;
	float origineZ = 0;

	//origine
	ofVec3f origineOctaedre = ofVec3f(origineX, origineY, origineZ);
	/****************************/
	/***generation des points****/
	/***x' = x cos f - y sin f***/
	/***y' = y cos f + x sin f***/
	/****************************/

	//sommet du haut
	ofVec3f sommet1 = ofVec3f(0, -(taille*0.75), 0);

	//rotation x
	if (isRotation3DXPrimitiveGeo) {
		sommet1 = ofVec3f(0,
			-((taille*0.75))*cos(ofDegToRad(sliderRotationPrimitiveGeo)),
			-((taille*0.75))*sin(ofDegToRad(sliderRotationPrimitiveGeo)));
	}

	//rotation z
	else if (isRotation3DZPrimitiveGeo) {
		sommet1 = ofVec3f(((taille*0.75))*sin(ofDegToRad(sliderRotationPrimitiveGeo)),
			((-(taille*0.75))*cos(ofDegToRad(sliderRotationPrimitiveGeo))), 0);
	}
	//ajoute l'origine
	sommet1 += origineOctaedre;

	//sommet de derriere
	ofVec3f sommet2 = ofVec3f(0, 0, -(taille) / (2* sqrt(0.75)));

	//rotation x
	if (isRotation3DXPrimitiveGeo) {
		sommet2 = ofVec3f(0,
			(-(taille) / (2 * sqrt(0.75)))*sin(ofDegToRad(sliderRotationPrimitiveGeo)),
			((taille) / (2 * sqrt(0.75)))*cos(ofDegToRad(sliderRotationPrimitiveGeo)));
	}

	//rotation y
	else if (isRotation3DYPrimitiveGeo) {
		sommet2 = ofVec3f(((taille) / (2 * sqrt(0.75)))*sin(ofDegToRad(sliderRotationPrimitiveGeo)), 0, -((taille) / (2 * sqrt(0.75)))*cos(ofDegToRad(sliderRotationPrimitiveGeo)));
	}

	//ajoute l'origine
	sommet2 += origineOctaedre;


	//sommet avant gauche
	ofVec3f sommet3 = ofVec3f(-(taille / 2), 0, (taille) / (4* sqrt(0.75)));

	//rotation x
	if (isRotation3DXPrimitiveGeo) {
		sommet3 = ofVec3f(-(taille / 2),
			(((taille) / (4 * sqrt(0.75)))*sin(ofDegToRad(sliderRotationPrimitiveGeo))),
			(((taille) / (4 * sqrt(0.75)))*cos(ofDegToRad(sliderRotationPrimitiveGeo))));

	}

	//rotation y
	else if (isRotation3DYPrimitiveGeo) {
		sommet3 = ofVec3f(-(taille / 2)*cos(ofDegToRad(sliderRotationPrimitiveGeo)) - ((taille) / (4 * sqrt(0.75)))*sin(ofDegToRad(sliderRotationPrimitiveGeo)),
			0,
			((taille) / (4 * sqrt(0.75)))*cos(ofDegToRad(sliderRotationPrimitiveGeo)) + (-(taille / 2)*sin(ofDegToRad(sliderRotationPrimitiveGeo))));
	}

	//rotation z
	else if (isRotation3DZPrimitiveGeo) {
		sommet3 = ofVec3f(((-(taille / 2))*cos(ofDegToRad(sliderRotationPrimitiveGeo))),
			((-(taille / 2))*sin(ofDegToRad(sliderRotationPrimitiveGeo))),
			(taille) / (4 * sqrt(0.75)));
	}

	//ajoute l'origine
	sommet3 += origineOctaedre;


	/***x' = x cos f - y sin f***/
	/***y' = y cos f + x sin f***/

	//sommet avant droit
	ofVec3f sommet4 = ofVec3f((taille / 2), 0, (taille) / (4 * sqrt(0.75)));

	//rotation x
	if (isRotation3DXPrimitiveGeo) {
		sommet4 = ofVec3f((taille / 2),
			((taille) / (4 * sqrt(0.75)))*sin(ofDegToRad(sliderRotationPrimitiveGeo))),
			(((taille) / (4 * sqrt(0.75)))*cos(ofDegToRad(sliderRotationPrimitiveGeo)));
	}

	//rotation y
	else if (isRotation3DYPrimitiveGeo) {
		sommet4 = ofVec3f((taille / 2)*cos(ofDegToRad(sliderRotationPrimitiveGeo)) - ((taille) / (4 * sqrt(0.75)))*sin(ofDegToRad(sliderRotationPrimitiveGeo)),
			0,
			((taille) / (4 * sqrt(0.75)))*cos(ofDegToRad(sliderRotationPrimitiveGeo)) + ((taille / 2)*sin(ofDegToRad(sliderRotationPrimitiveGeo))));
	}

	//rotation z
	else if (isRotation3DZPrimitiveGeo) {
		sommet4 = ofVec3f((((taille / 2))*cos(ofDegToRad(sliderRotationPrimitiveGeo))),
			(((taille / 2))*sin(ofDegToRad(sliderRotationPrimitiveGeo))),
			(taille) / (4 * sqrt(0.75)));
	}

	//ajoute l'origine
	sommet4 += origineOctaedre;

	//sommet du dessous
	ofVec3f sommet5 = ofVec3f(0, (taille*0.75), 0);

	//rotation x
	if (isRotation3DXPrimitiveGeo) {
		sommet5 = ofVec3f(0,
			((taille*0.75))*cos(ofDegToRad(sliderRotationPrimitiveGeo)),
			((taille*0.75))*sin(ofDegToRad(sliderRotationPrimitiveGeo)));
	}

	//rotation z
	else if (isRotation3DZPrimitiveGeo) {
		sommet5 = ofVec3f((-(taille*0.75))*sin(ofDegToRad(sliderRotationPrimitiveGeo)),
			(((taille*0.75))*cos(ofDegToRad(sliderRotationPrimitiveGeo))), 0);
	}
	//ajoute l'origine
	sommet5 += origineOctaedre;


	//dessin des triangles avec les points
	ofDrawTriangle(sommet1, sommet2, sommet3);
	ofDrawTriangle(sommet1, sommet2, sommet4);
	ofDrawTriangle(sommet1, sommet3, sommet4);
	ofDrawTriangle(sommet2, sommet3, sommet4);
	ofDrawTriangle(sommet5, sommet2, sommet3);
	ofDrawTriangle(sommet5, sommet2, sommet4);
	ofDrawTriangle(sommet5, sommet3, sommet4);

	//dessin des aretes
	ofSetColor(255, 255, 255);
	ofDrawLine(sommet1, sommet2);
	ofDrawLine(sommet1, sommet3);
	ofDrawLine(sommet1, sommet4);
	ofDrawLine(sommet2, sommet3);
	ofDrawLine(sommet2, sommet4);
	ofDrawLine(sommet3, sommet4);
	ofDrawLine(sommet5, sommet2);
	ofDrawLine(sommet5, sommet3);
	ofDrawLine(sommet5, sommet4);
}

//Color picker
void Renderer::rgbModeSwitched(bool &rgbMode) {
	if (rgbMode)
	{
		colorPickerGUI.setName("RGB Color Picker");
		redOrHue.set("red", currentColor.r, 0, 255);
		greenOrSaturation.set("green", currentColor.g, 0, 255);
		blueOrBrightness.set("blue", currentColor.b, 0, 255);
	}
	else
	{
		colorPickerGUI.setName("HSB Color Picker");
		redOrHue.set("hue", currentColor.getHue(), 0, 360);
		greenOrSaturation.set("saturation", currentColor.getSaturation(), 0, 100);
		blueOrBrightness.set("brightness", currentColor.getBrightness(), 0, 100);
	}
}

//primitive geo
void Renderer::buttonRotation3DXPressed() {
	isRotation3DXPrimitiveGeo = true;
	isRotation3DYPrimitiveGeo = false;
	isRotation3DZPrimitiveGeo = false;
	sliderRotationPrimitiveGeo.setName("Rotation sur X");
}
void Renderer::buttonRotation3DYPressed() {
	isRotation3DXPrimitiveGeo = false;
	isRotation3DYPrimitiveGeo = true;
	isRotation3DZPrimitiveGeo = false;
	sliderRotationPrimitiveGeo.setName("Rotation sur Y");

}
void Renderer::buttonRotation3DZPressed() {
	isRotation3DXPrimitiveGeo = false;
	isRotation3DYPrimitiveGeo = false;
	isRotation3DZPrimitiveGeo = true;
	sliderRotationPrimitiveGeo.setName("Rotation sur Z");

}

//Generer boite autour des modeles 3D
void Renderer::genererBoite() {

	ofSetColor(currentColor);

	if (isGenererLapin) {
		boite = ofBoxPrimitive(500, 700, 350);
		//On met la boite � la meme position que le modele pour obtenir un rotation symetrique au modele
		boite.setPosition((screenWidth / 2), screenHeight*0.75, 0);
		//On suit la meme rotation que le modele
		boite.rotate(-sliderRotation3DX, 1, 0, 0);
		boite.rotate(-sliderRotation3DY, 0, 1, 0);
		boite.rotate(sliderRotation3DZ, 0, 0, 1);
		boite.setScale(sliderProportion3DX, sliderProportion3DY, sliderProportion3DZ);
		boite.draw();
	}
	else if (isGenererDragon) {
		boite = ofBoxPrimitive(400, 600, 200);
		//On met la boite � la meme position que le modele pour obtenir un rotation symetrique au modele
		boite.setPosition((screenWidth / 2), screenHeight*0.75, 0);
		//On suit la meme rotation que le modele
		boite.rotate(-sliderRotation3DX, 1, 0, 0);
		boite.rotate(-sliderRotation3DY, 0, 1, 0);
		boite.rotate(sliderRotation3DZ, 0, 0, 1);

		boite.setScale(sliderProportion3DX, sliderProportion3DY, sliderProportion3DZ);
		boite.draw();
	}
	
}

//animation des modeles 3D

void Renderer::animerMaillage() {

	//On augmente la rotation
	sliderRotation3DY = sliderRotation3DY + 1.3;
	if (sliderRotation3DY >= 360) {
		sliderRotation3DY = 0;
	}

	if (sliderProportion3DX >= sliderProportion3DX.getMax() || 
		sliderProportion3DY >= sliderProportion3DY.getMax() || 
		sliderProportion3DZ >= sliderProportion3DZ.getMax()) {
		animationGrossit = false;
	}
	else if (sliderProportion3DX <= sliderProportion3DX.getMin() || 
		sliderProportion3DY <= sliderProportion3DY.getMin() || 
		sliderProportion3DZ <= sliderProportion3DZ.getMin()) {
		animationGrossit = true;
	}
	//On augmente ou on diminue la proportion
	if (animationGrossit) {
		sliderProportion3DX = sliderProportion3DX + 0.001;
		sliderProportion3DY = sliderProportion3DY + 0.001;
		sliderProportion3DZ = sliderProportion3DZ + 0.001;
	}
	else if(animationGrossit==false){
		sliderProportion3DX = sliderProportion3DX - 0.001;
		sliderProportion3DY = sliderProportion3DY - 0.001;
		sliderProportion3DZ = sliderProportion3DZ - 0.001;
	}
}

//Composition d'image
void Renderer::load_image(const std::string path, ofImage &image, bool &imageLoadSuccess) {
	try {
		image.load(path);
		imageLoadSuccess = true;
		ofLog() << "< the file has been loaded>";
	}
	catch (runtime_error& e) {
		imageLoadSuccess = false;
		ofLog() << "< file not found>";
	}
}

void Renderer::boutonImporterImageTexture1() {
	ofFileDialogResult result = ofSystemLoadDialog("Load file");
	if (result.bSuccess) {
		string path = result.getPath();
		load_image(path, texture1, is_texture1_loaded);
		is_texture1_loaded ? textureName1.set(result.getName()) : textureName1.set("None");
	}
}

void Renderer::boutonImporterImageTexture2() {
	ofFileDialogResult result = ofSystemLoadDialog("Load file");
	if (result.bSuccess) {
		string path = result.getPath();
		load_image(path, texture2, is_texture2_loaded);
		if (is_texture2_loaded) {
			textureName2.set(result.getName());
		}
		else {
			textureName2.set("None");
		}
	}
}

//Traitement d'image par convolution
void Renderer::aiguiserToggled(bool & aiguiser) {
	if (aiguiser) {
		detectionBordure.set(false);
		bosseler.set(false);
		flou.set(false);
		kernel_type = ConvolutionKernel::sharpen;
		kernel_name = "aiguiser";
	}
}

void Renderer::detectionBordureToggled(bool & detectionBordure) {
	if (detectionBordure) {
		aiguiser.set(false);
		bosseler.set(false);
		flou.set(false);
		kernel_type = ConvolutionKernel::edge_detect;
		kernel_name = "d�tection de bordure";
	}
}

void Renderer::bosselerToggled(bool & bosseler) {
	if (bosseler) {
		aiguiser.set(false);
		detectionBordure.set(false);
		flou.set(false);
		kernel_type = ConvolutionKernel::emboss;
		kernel_name = "bosseler";
	}
}

void Renderer::flouToggled(bool & flou) {
	if (flou) {
		aiguiser.set(false);
		detectionBordure.set(false);
		bosseler.set(false);
		kernel_type = ConvolutionKernel::blur;
		kernel_name = "flou";
	}
}

// fonction de filtrage par convolution
void Renderer::filter()
{
	// r�solution du kernel de convolution
	const int kernel_size = 3;

	// d�calage � partir du centre du kernel
	const int kernel_offset = kernel_size / 2;

	// nombre de composantes de couleur (RGB)
	const int color_component_count = 3;

	// indices de l'image
	int x, y;

	// indices du kernel
	int i, j;

	// index des composantes de couleur
	int c;

	// index du pixel de l'image source utilis� pour le filtrage
	int pixel_index_img_src;

	// index du pixel de l'image de destination en cours de filtrage
	int pixel_index_img_dst;

	// index du pixel de l'image de destination en cours de filtrage
	int kernel_index;

	// valeur � un des indices du kernel de convolution
	float kernel_value;

	// extraire les pixels de l'image source
	ofPixels pixel_array_src = currentImage.getPixels();

	// extraire les pixels de l'image de destination
	ofPixels pixel_array_dst = filteredImage.getPixels();

	// couleur du pixel lu dans l'image source
	ofColor pixel_color_src;

	// couleur du pixel � �crire dans l'image de destination
	ofColor pixel_color_dst;

	// somme du kernel appliqu�e � chaque composante de couleur d'un pixel
	float sum[color_component_count];

	// it�ration sur les rang�es des pixels de l'image source
	for (y = 0; y < currentImage.getHeight(); ++y)
	{
		// it�ration sur les colonnes des pixels de l'image source
		for (x = 0; x < currentImage.getWidth(); ++x)
		{
			// initialiser le tableau o� les valeurs de filtrage sont accumul�es
			for (c = 0; c < color_component_count; ++c)
				sum[c] = 0;

			// d�terminer l'index du pixel de l'image de destination
			pixel_index_img_dst = (currentImage.getWidth() * y + x) * color_component_count;

			// it�ration sur les colonnes du kernel de convolution
			for (j = -kernel_offset; j <= kernel_offset; ++j)
			{
				// it�ration sur les rang�es du kernel de convolution
				for (i = -kernel_offset; i <= kernel_offset; ++i)
				{
					// d�terminer l'index du pixel de l'image source � lire
					pixel_index_img_src = (currentImage.getWidth() * (y - j) + (x - i)) * color_component_count;
					if (pixel_index_img_src < 0 || pixel_index_img_src >= currentImage.getWidth() * currentImage.getHeight() * color_component_count)
						pixel_color_src = 0;
					else
						// lire la couleur du pixel de l'image source
						pixel_color_src = pixel_array_src.getColor(pixel_index_img_src);

					// d�terminer l'indice du facteur � lire dans le kernel de convolution
					kernel_index = kernel_size * (j + kernel_offset) + (i + kernel_offset);

					// extraction de la valeur � cet index du kernel
					switch (kernel_type)
					{
					case ConvolutionKernel::identity:
						kernel_value = convolution_kernel_identity.at(kernel_index);
						break;

					case ConvolutionKernel::emboss:
						kernel_value = convolution_kernel_emboss.at(kernel_index);
						break;

					case ConvolutionKernel::sharpen:
						kernel_value = convolution_kernel_sharpen.at(kernel_index);
						break;

					case ConvolutionKernel::edge_detect:
						kernel_value = convolution_kernel_edge_detect.at(kernel_index);
						break;

					case ConvolutionKernel::blur:
						kernel_value = convolution_kernel_blur.at(kernel_index);
						break;

					default:
						kernel_value = convolution_kernel_identity.at(kernel_index);
						break;
					}

					// it�ration sur les composantes de couleur
					for (c = 0; c < color_component_count; ++c)
					{
						// accumuler les valeurs de filtrage en fonction du kernel de convolution
						sum[c] = sum[c] + kernel_value * pixel_color_src[c];
					}
				}
			}

			// d�terminer la couleur du pixel � partir des valeurs de filtrage accumul�es pour chaque composante
			for (c = 0; c < color_component_count; ++c)
			{
				// conversion vers entier et validation des bornes de l'espace de couleur
				pixel_color_dst[c] = (int)ofClamp(sum[c], 0, 255);
			}

			// �crire la couleur � l'index du pixel en cours de filtrage
			pixel_array_dst.setColor(pixel_index_img_dst, pixel_color_dst);
		}
	}

	// �crire les pixels dans l'image de destination
	filteredImage.setFromPixels(pixel_array_dst);

	ofLog() << "<convolution filter done>";
}



//Texture proc�dural
void Renderer::binaryTreeToggled(bool & binaryTree) {
	if (binaryTree) {
		ternaryTree.set(false);
	}
}

void Renderer::ternaryTreeToggled(bool & ternaryTree) {
	if (ternaryTree) {
		binaryTree.set(false);
	}
}

void Renderer::drawTernaryTree(int length)
{
	ofDrawLine(0, 0, 0, -length);

	ofPushMatrix();
	ofRotate(angle);
	ofDrawLine(0, 0, 0, -length);
	ofPopMatrix();

	ofPushMatrix();
	ofRotate(-angle);
	ofDrawLine(0, 0, 0, -length);
	ofPopMatrix();

	float tempLength = length;
	length *= 0.5;

	ofPushMatrix();
	ofTranslate(0, -tempLength);
	if (length > 2) {
		drawTernaryTree(length);
	}
	ofPopMatrix();

	ofPushMatrix();
	ofRotate(angle);
	ofTranslate(0, -tempLength);
	if (length > 2) {
		drawTernaryTree(length);
	}
	ofPopMatrix();

	ofPushMatrix();
	ofRotate(-angle);
	ofTranslate(0, -tempLength);
	if (length > 2) {
		drawTernaryTree(length);
	}
	ofPopMatrix();
}

void Renderer::drawBinaryTree(int length) {
	ofDrawLine(0, 0, 0, -length);
	ofTranslate(0, -length);

	length *= 0.66;

	if (length > 2) {
		ofPushMatrix();
		ofRotate(angle);
		drawBinaryTree(length);
		ofPopMatrix();

		ofPushMatrix();
		ofRotate(-angle);
		drawBinaryTree(length);
		ofPopMatrix();
	}
}

//Traitement d'image
void Renderer::compositionToggled(bool &composition) {
	if (composition) {
		convolutionFilter.set(false);
		proceduralTexture.set(false);
	}
}
void Renderer::convolutionToggled(bool &convolution) {
	if (convolution) {
		imageComposition.set(false);
		proceduralTexture.set(false);
	}
}
void Renderer::proceduralToggled(bool &procedural) {
	if (procedural) {
		imageComposition.set(false);
		convolutionFilter.set(false);
	}
}

void Renderer::importImagePressed() {
	ofFileDialogResult result = ofSystemLoadDialog("Load file");
	if (result.bSuccess) {
		string path = result.getPath();
		load_image(path, currentImage, is_current_image_loaded);
		is_current_image_loaded ? labelSourceImage.set(result.getName()) : labelSourceImage.set("None");
	}
}

