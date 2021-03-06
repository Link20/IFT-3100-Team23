#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "model2D.h"
#include "primitives/line.h"
#include "primitives/rectangle.h"
#include "primitives/circle.h"
#include "primitives/triangle.h"
#include "primitives/triangleRect.h"
#include "historic.h"
#include "raycast_classes/Vector.h"
#include "raycast_classes/RayCastRay.h"
#include "raycast_classes/RayCastSphere.h"
#include "raycast_classes/RayCastCamera.h"
#include "raycast_classes/RayCastImage.h"
#include "ofxAssimpModelLoader.h"

#include <random>
#include <cmath>
#include <limits>
#define _USE_MATH_DEFINES
#include <math.h>
#include <sstream> 
#include <fstream>
#include <iostream>

// énumération des types de kernel de convolution
enum class ConvolutionKernel
{
	identity,
	emboss,
	sharpen,
	edge_detect,
	blur
};

// kernel de convolution (3x3) : identité
const std::array<float, 9> convolution_kernel_identity
{
	0.0f,  0.0f,  0.0f,
	0.0f,  1.0f,  0.0f,
	0.0f,  0.0f,  0.0f
};

// kernel de convolution (3x3) : aiguiser
const std::array<float, 9> convolution_kernel_sharpen
{
	0.0f, -1.0f,  0.0f,
	-1.0f,  5.0f, -1.0f,
	0.0f, -1.0f,  0.0f
};

// kernel de convolution (3x3) : détection de bordure
const std::array<float, 9> convolution_kernel_edge_detect
{
	0.0f,  1.0f,  0.0f,
	1.0f, -4.0f,  1.0f,
	0.0f,  1.0f,  0.0f
};

// kernel de convolution (3x3) : bosseler
const std::array<float, 9> convolution_kernel_emboss
{
	-2.0f, -1.0f,  0.0f,
	-1.0f,  1.0f,  1.0f,
	0.0f,  1.0f,  2.0f
};

// kernel de convolution (3x3) : flou
const std::array<float, 9> convolution_kernel_blur
{
	1.0f / 9.0f,  1.0f / 9.0f,  1.0f / 9.0f,
	1.0f / 9.0f,  1.0f / 9.0f,  1.0f / 9.0f,
	1.0f / 9.0f,  1.0f / 9.0f,  1.0f / 9.0f
};

enum class DrawTool {select, primitive};
enum class DrawPrimitive {line, circle, rectangle, triangle, triangleRect};

//position camera
enum class Camera { front, back, left, right, top, down, free, orbit };

//courbe parametrique
enum class CurveType { bezier, hermite, splineDeBezier };

enum class ShaderType {color_fill, lambert, gouraud, phong, blinn_phong, double_light, triple_light};

enum class Light_select {light1, light2, light3, ambient};

enum class Material_aspect {ambient, diffuse, specular};

enum class Light_aspect {ambient, diffuse, specular};

enum class Select_3D {modele, lumiere, material, shader, primitive3D, surface};

inline void bezier(float t,
	float p1x, float p1y, float p1z,
	float p2x, float p2y, float p2z,
	float p3x, float p3y, float p3z,
	float p4x, float p4y, float p4z,
	float&  x, float& y, float&  z)
{
	float u = 1 - t;
	float uu = u * u;
	float uuu = uu * u;
	float tt = t * t;
	float ttt = tt * t;

	x = uuu * p1x + 3 * uu * t * p2x + 3 * u * tt * p3x + ttt * p4x;
	y = uuu * p1y + 3 * uu * t * p2y + 3 * u * tt * p3y + ttt * p4y;
	z = uuu * p1z + 3 * uu * t * p2z + 3 * u * tt * p3z + ttt * p4z;
}

inline void hermite(
	float t,
	float p1x, float p1y, float p1z,
	float p2x, float p2y, float p2z,
	float p3x, float p3y, float p3z,
	float p4x, float p4y, float p4z,
	float&  x, float& y, float&  z)
{
	float u = 1 - t;
	float uu = u * u;
	float uuu = uu * u;
	float tt = t * t;
	float ttt = tt * t;

	x = (2 * ttt - 3 * tt + 1) * p1x + (ttt - 2 * tt + t) * p2x + (ttt - tt) * p3x + (-2 * ttt + 3 * tt) * p4x;
	y = (2 * ttt - 3 * tt + 1) * p1y + (ttt - 2 * tt + t) * p2y + (ttt - tt) * p3y + (-2 * ttt + 3 * tt) * p4y;
	z = (2 * ttt - 3 * tt + 1) * p1z + (ttt - 2 * tt + t) * p2z + (ttt - tt) * p3z + (-2 * ttt + 3 * tt) * p4z;
}

class Renderer {
public:

	float screenWidth;
	float screenHeight;

	bool is2D;
    bool is_filled;

	//Curseur
	float mouse_press_x;
	float mouse_press_y;

	float mouse_current_x;
	float mouse_current_y;
	bool is_mouse_button_pressed;
	bool is_mouse_button_legit;
	float current_thickness;
	Primitive* preview_primitive;
    bool is_preview;
    bool is_selected;
	DrawTool draw_tool;
	DrawPrimitive draw_primitive;
    float origin_slider_x;
    float origin_slider_y;
    float temp_alpha;

	Model2D model2D;
	History history;
	ofImage background_image;
	bool is_ctrl_pressed;
	bool is_background_image_loaded;
	bool is_menu_displayed;
    bool is_current_primitive_selected;

	//Geometrie
	bool isGenererTetraedre;
	bool isGenererOctaedre;

	//Modele 3D
	bool isGenererModele3D;
	ofxAssimpModelLoader modele;
	ofVec3f modele_angle;

	Select_3D selected_3D_instance;
	Light_select light_selected;
    Material_aspect material_aspect_selected;
	Light_aspect light_selected_aspect;

	ofLight light_1;
	ofLight light_2;
	ofLight light_3;
    ofLight ambient_light;

	ofVec3f light_1_angle;
	ofVec3f light_2_angle;
	ofVec3f light_3_angle;

	int number_of_light;

	bool isLight2Enabled;

	bool isActiveTranslation3D;
	bool isActiveRotationX3D;
	bool isActiveRotationY3D;
	bool isActiveRotationZ3D;
	bool isActiveProportion3D;

	//Color picker
	ofColor currentColor;
	ofxPanel colorPickerGUI;
	ofxToggle rgbMode;
    ofxToggle fillingMode;
	ofParameter<float> redOrHue;
	ofParameter<float> greenOrSaturation;
	ofParameter<float> blueOrBrightness;
	ofParameter<float> alpha;

	//Generer boite
	ofBoxPrimitive boite;
	//modele 3D
	bool isGenererLapin;
	bool isGenererDragon;

	//animation
	bool animationGrossit;


	//image
	ofImage currentImage;
	bool is_current_image_loaded;

	//Selecting texture mode
	ofParameter<bool> imageComposition;
	ofParameter<bool> convolutionFilter;
	ofParameter<bool> proceduralTexture;
	ofParameter<bool> raycastRenderer;

	//Image selector pour composition d'image
	ofxPanel textureCompositionGUI;
	ofxButton boutonImporterTexture1;
	ofxButton boutonImporterTexture2;
	ofParameter<string> textureName1;
	ofParameter<string> textureName2;
	ofParameter<bool> compositionAdd;
	ofImage texture1;
	ofImage texture2;
	bool is_texture1_loaded;
	bool is_texture2_loaded;

	//Traitement d'image par convolution
	ofxPanel filtrageConvolutionGUI;
	ofParameter<bool> aiguiser;
	ofParameter<bool> detectionBordure;
	ofParameter<bool> bosseler;
	ofParameter<bool> flou;
	ofImage filteredImage;
	ConvolutionKernel kernel_type;
	string kernel_name;

	//Texture procédurale
	ofxPanel textureProceduralGUI;
	ofParameter<bool> binaryTree;
	ofParameter<bool> ternaryTree;
	ofParameter<int> branchLength;
	ofParameter<int> angle;

	//Ray Casting
	ofxPanel rayCasterGUI;
	ofParameter<int> widthOfRenderedImage;
	ofParameter<int> heightOfRenderedImage;
	ofParameter<int> raysPerPixel;
	ofxButton renderRayCastImage;
	ofParameter<bool> viewLastRenderedImage;

	string lastRayCastResultPath;
	ofImage lastRayCastResult;
	bool rayCastResultLoaded;

	// paramètres du programme
	const int max_depth = 5;
	const double camera_fov = 0.5135;
	double gamma_correction = 1 / 2.2;
	int image_width = 0;
	int image_height = 0;
	int ray_per_pixel = 0;
	int pixel_count = 0;
	std::vector<RayCastSphere> scene; // déclaration d'un graphe de scène
	Vector ray_cast_camera_orientation = Vector(0, -0.042612, -1).normalize();
	Vector ray_cast_camera_position = Vector(50, 52, 295.6);
	RayCastCamera ray_cast_camera = RayCastCamera(ray_cast_camera_position, ray_cast_camera_orientation); // déclaration de la caméra utilisée pour rendre la scène
	RayCastImage image; // déclaration d'une image
	std::random_device rd; // source d'entropie
	std::mt19937 rng{ rd() }; // générateur de nombres pseudo-aléatoires (mersenne twister)
	std::uniform_real_distribution<double> random01{ 0.0, 1.0 }; // distribution uniforme entre 0 et 1


	//*******************************************//
	//********************GUI********************//
	//*******************************************//

	//menu general
	ofxPanel guiMenu;
	ofParameter<bool> boutonMode2D;
	ofParameter<bool> boutonMode3D;
	ofxButton boutonImporterImage;
	ofxButton boutonExporterImage;
	ofxLabel labelTraitementImage;
	ofParameter<string> labelSourceImage;
	ofParameter<bool> toggleAfficherImage;


	//interface dessin
	ofxPanel guiDessin;
	ofxIntSlider sliderPosX;
	ofxIntSlider sliderPosY;
	ofxIntSlider sliderRotation;

	float largeurForme;
	float hauteurForme;
	ofxLabel labelLargeur;
	ofxLabel labelHauteur;

	ofxLabel choixDeForme;

	ofxButton boutonLigne;
	ofxButton boutonCercle;
	ofxButton boutonRectangle;
	ofxButton boutonTriangle;
	ofxButton boutonTriangleRectangle;
    ofxButton boutonSelection;

	ofxLabel labelPropriteteDuDessin;
	ofxFloatSlider sliderEpaisseurLigneContour;

	ofxButton boutonUndo;
	ofxButton boutonRedo;


	//interface pour modèle 3D
	ofxPanel guiModel3D;

	ofxLabel labelRotation3D;
	ofxFloatSlider sliderRotation3DX;
	ofxFloatSlider sliderRotation3DY;
	ofxFloatSlider sliderRotation3DZ;
	ofxLabel labelProportion3D;
	ofxFloatSlider sliderProportion3DX;
	ofxFloatSlider sliderProportion3DY;
	ofxFloatSlider sliderProportion3DZ;
    ofxFloatSlider sliderPorteeLumiere;
    ofxFloatSlider sliderBrightness;
    ofxFloatSlider sliderBrume;

	ofxLabel labelGenerationPrimitiveGeometrique;
	ofxButton boutonOctaedre;
	ofxButton boutonTetraedre;

	ofxLabel labelGenerationModel3D;
	ofxButton boutonLapin;
	ofxButton boutonDragon;
	ofxToggle animer;
	ofxToggle dessierBoite;
	ofxLabel labelEffet;

	ofxButton boutonLumiere;
	ofxButton boutonAmbient;
	ofxButton boutonDiffuse;
	ofxButton boutonSpeculaire;
	ofxButton boutonShader;

	//interface camera
	ofxLabel labelCamera;
	ofxLabel labelBougerCamera;
	ofxLabel labelZoomCamera;
	ofParameter<bool> toggleFrontCamera;
	ofParameter<bool> toggleBackCamera;
	ofParameter<bool> toggleLeftCamera;
	ofParameter<bool> toggleRightCamera;
	ofParameter<bool> toggleTopCamera;
	ofParameter<bool> toggleBottomCamera;
	ofxLabel labelModeDeProjection;
	ofParameter<bool> toggleProjectionPerspective;
	ofParameter<bool> toggleProjectionOrthogonale;

	//interface courbe parametrique
	ofxLabel labelCourbeParametrique;
	ofxLabel labelPointDeControle;
	ofParameter<string> labelChoixPointDeControle;
	ofParameter<string> labelDeplacerPointDeControle;
	ofParameter<bool> toggleCourbeDeBezier;
	ofParameter<bool> toggleCourbeDeHermite;
	ofParameter<bool> toggleCourbeSplineDeBezier;

	//interface surface parametrique 
	ofxLabel labelSurfaceParametrique;
	ofxLabel labelPointDeControleSurface;
	ofParameter<string> labelChoixPointDeControleSurface;
	ofParameter<string> labelDeplacerPointDeControleSurface;
	ofParameter<bool> toggleSurfaceDeCoons;

	//interface triangulation
	ofParameter<bool> toogleTriangulation;

	//interface brume
	ofParameter<bool> toogleActiverBrume;

	//interface tesselation
	ofxSlider<int> sliderNiveauTesselation;

	//Camera
	Camera camera_active;

	ofCamera camera_front;
	ofCamera camera_back;
	ofCamera camera_left;
	ofCamera camera_right;
	ofCamera camera_top;
	ofCamera camera_down;

	ofCamera* camera;

	ofQuaternion camera_orientation;

	ofVec3f camera_position;
	ofVec3f camera_target;

	float camera_near;
	float camera_far;

	float offset_camera;
	float offset_scene;

    ShaderType shader_active;

    ofShader shader_color_fill;
    ofShader shader_lambert;
    ofShader shader_gouraud;
    ofShader shader_phong;
    ofShader shader_blinn_phong;
    ofShader shader_2_light_blinn_phong;
	ofShader shader_3_light_blinn_phong;

    ofShader shader_lights;

    ofShader* shader;

    ofMaterial material_basic;

    ofColor primitive_color;

    string shader_name;

	float speed_delta;
	float speed_translation;

	float time_current;
	float time_last;
	float time_elapsed;

	int index_x;
	int index_y;
	int index_z;

	bool is_camera_move_left;
	bool is_camera_move_right;
	bool is_camera_move_up;
	bool is_camera_move_down;
	bool is_camera_move_forward;
	bool is_camera_move_backward;

	//Courbe parametrique
	bool isCourbeParametriqueActive;
	CurveType curveId;
	ofPolyline lineRenderer;
	ofVec3f* pointDeControleSelectionne;

	ofVec3f pointDeControle1;
	ofVec3f pointDeControle2;
	ofVec3f pointDeControle3;
	ofVec3f pointDeControle4;
	ofVec3f pointDeControle5;
	ofVec3f pointDeControle6;

	ofVec3f positionDansCourbe;

	int resolutionCourbe;
	int index;

	float vitessePointDeControle;

	ofVec3f tangente1;
	ofVec3f tangente2;

	int rayonPointDeControle;
	//Surface parametrique
	bool isSurfaceParametriqueActive;

	ofVec3f* pointDeControleSelectionneSurface;

	ofVec3f pointDeControleSurface1;
	ofVec3f pointDeControleSurface2;
	ofVec3f pointDeControleSurface3;
	ofVec3f pointDeControleSurface4;
	ofVec3f pointDeControleSurface5;
	ofVec3f pointDeControleSurface6;
	ofVec3f pointDeControleSurface7;
	ofVec3f pointDeControleSurface8;

	ofVec3f positionDansCourbeU;
	ofVec3f positionDansCourbeV;
	int resolutionSurface;

	ofPolyline lineRendererC1;
	ofPolyline lineRendererC2;
	ofPolyline lineRendererC3;
	ofPolyline lineRendererC4;

	vector<ofPolyline> lignesDeSurfaceV;
	vector<ofPolyline> lignesDeSurfaceU;
	ofVec3f lerpu;
	ofVec3f lerpv;
	ofVec3f blerp;
	ofVec3f surfaceCoons;

	//Triangulation
	bool isTriangulation;
	ofMesh maillageDeTriangulation;

	//brume
	bool brumeActiver;

	//Tesselation
	int saut;

	//Ecouteur de l'interface
	void boutonLignePressed();
	void boutonCerclePressed();
	void boutonRectanglePressed();
	void boutonTrianglePressed();
	void boutonTriangleRectanglePressed();
	void boutonSelectionPressed();
	void boutonMode2DToggled(bool &mode2D);
	void boutonMode3DToggled(bool &mode3D);
	void boutonImporterImagePressed();
	void boutonExporterImagePressed();
	void boutonUndoPressed();
	void boutonRedoPressed();

	void boutonTetraedrePressed();
	void boutonOctaedrePressed();
	void boutonLapinPressed();
	void boutonDragonPressed();
	void boutonLumierePressed();
	void boutonAmbientPressed();
	void boutonDiffusePressed();
	void boutonSpeculairePressed();
	void boutonShaderPressed();

	//Curseur
	void dessinerCurseur(float x, float y) const;
	void dessinerCurseurEtoile(float x, float y) const;
	void dessinerCurseurTriforce(float x, float y) const;
	void dessinerCurseurPoint(float x, float y) const;
	void dessinerCurseurVise(float x, float y) const;
	void dessinerCurseurFleche(float x, float y) const;
	void dessinerCurseurPeace(float x, float y) const;

	//Camera
	void toggleFrontCameraPressed(bool &front);
	void toggleBackCameraPressed(bool &back);
	void toggleLeftCameraPressed(bool &left);
	void toggleRightCameraPressed(bool &right);
	void toggleTopCameraPressed(bool &top);
	void toggleBottomCameraPressed(bool &bottom);

	void toggleProjectionPerspectivePressed(bool &perspective);
	void toggleProjectionOrthogonalePressed(bool &orthogonale);

	//Courbe parametrique
	void toggleCourbeDeBezierPressed(bool &bezier);
	void toggleCourbeDeHermitePressed(bool &hermite);
	void toggleCourbeSplineDeBezierPressed(bool &splineDeBezier);
	void resetCourbeParametrique();

	//Surface parametrique
	void toggleSurfaceDeCoonsPressed(bool &coons);
	void resetSurfaceParametrique();

	//triangulation
	void toogleTriangulationPressed(bool &triangulation);

	//brume
	void toggleActiverBrumePressed(bool &brume);

	void setup();
	void draw();
    void draw2D();
    void draw3D();
    void drawLightSources();
    void drawGui();
	void update();
	void updateSelection();
    void updateShader();

	void create_preview();
	void preview_form();
	void addForm();
	void selectObject();
    void releaseSelection();
    void releaseWithNoChange();
    void deleteSelection();
    void setColor(float r, float g, float b, float a=255);

	//Geometrie
	void genererModele3D();
	void genererTetraedre();
	void genererOctaedre();

	//Color picker
	void rgbModeSwitched(bool &rgbMode);
    void fillingModeSwitched(bool &fillingMode);

	void undo();
	void redo();
	//primitive geo 3D	//gui 3d
	ofxLabel labelPrimitiveGeo;
	ofxButton buttonRotation3DX;
	ofxButton buttonRotation3DY;
	ofxButton buttonRotation3DZ;
	ofxFloatSlider sliderRotationPrimitiveGeo;
	bool isRotation3DXPrimitiveGeo;
	bool isRotation3DYPrimitiveGeo;
	bool isRotation3DZPrimitiveGeo;
	//listener
	void buttonRotation3DXPressed();
	void buttonRotation3DYPressed();
	void buttonRotation3DZPressed();

	//Generer boite
	void genererBoite();

	//animation
	void animerMaillage();

	//Image composition functions
	void load_image(const std::string path, ofImage &image, bool &imageLoadSuccess);
	void boutonImporterImageTexture1();
	void boutonImporterImageTexture2();

	//Convolution filter functions
	void aiguiserToggled(bool &aiguiser);
	void detectionBordureToggled(bool &detectionBordure);
	void bosselerToggled(bool &bosseler);
	void flouToggled(bool &flou);
	void filter();

	//Procedural texture functions
	void binaryTreeToggled(bool &binaryTree);
	void ternaryTreeToggled(bool & ternaryTree);
	void drawBinaryTree(int length);
	void drawTernaryTree(int length);

	//Ray tracing functions
	void boutonRenderRayCastImagePressed();
	void viewLastRayCastImageToggled(bool &viewRayTraceResult);
	double clamp(double x);
	int format_color_component(double value);
	bool raycast(const RayCastRay& ray, double& distance, int& id);
	Vector compute_radiance(const RayCastRay& ray, int depth);
	void setupRaycastScene();
	void render();
	void save_image_file(int width, int height, int ray_per_pixel, const Vector* pixel);

	//Traitement d'image
	void compositionToggled(bool &composition);
	void convolutionToggled(bool &convolution);
	void proceduralToggled(bool &procedural);
	void raycastToggled(bool &raycast);	

	void image_export(const std::string name, const std::string extension) const;

	void mouseReleased(ofMouseEventArgs & mouse);
	
	//Camera
	void setup_camera();
	void resetCamera();

	~Renderer();

private:
    void generate_modified_primitive();

};
