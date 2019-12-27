#include "plugin.hpp"
#include <string>
#include <componentlibrary.hpp>
#include <math.h>

#define SECTEUR (45) // 1/2 angle du vu metre
#define LONGUEUR (65) // longueur de l'aiguille
#define LONGUEUR_TRAIT (7) // longueur du trait sur le cadran 


struct WoltModule : Module {
	enum ParamIds {
		NUM_PARAMS
	};
	enum InputIds {
		INPUTONE_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};
	
	std::string lelabel  {
		"Hello"
	};
	
	TextField * label;
	 

	WoltModule() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	}
	void process(const ProcessArgs& args) override {
		// float courantV = inputs[INPUTONE_INPUT].getVoltage();
		
 

	}
};



struct WoltModuleDisplay : Widget {
	WoltModule *module;
	WoltModuleDisplay(){}
	void draw(const DrawArgs &args) override {
		//background noir 
		// nvgFillColor(args.vg, nvgRGB(20, 30, 33));
		// nvgBeginPath(args.vg);
		// nvgRect(args.vg, 10, 80, box.size.x-20, box.size.y-200);
		// nvgFill(args.vg);

		/// ligne médiane
		nvgSave(args.vg);
		nvgTranslate(args.vg, box.size.x * 0.5, box.size.y-150);
		nvgStrokeColor(args.vg, nvgRGB(95,95,95));
		nvgStrokeWidth(args.vg, 1);
		nvgBeginPath(args.vg);
		nvgMoveTo(args.vg, 0, -LONGUEUR+LONGUEUR_TRAIT);
		nvgLineTo(args.vg, 0, -LONGUEUR);
		nvgStroke(args.vg);
		nvgRestore(args.vg);

		// extrémités
		nvgSave(args.vg);
		nvgTranslate(args.vg, box.size.x * 0.5, box.size.y-150);
		nvgStrokeColor(args.vg, nvgRGB(95,95,95));
		nvgStrokeWidth(args.vg, 1);
		nvgBeginPath(args.vg);
		nvgRotate(args.vg, ( SECTEUR * M_PI ) / 180 );
		nvgMoveTo(args.vg, 0, -LONGUEUR+LONGUEUR_TRAIT);
		nvgLineTo(args.vg, 0, -LONGUEUR);
		nvgStroke(args.vg);
		nvgRestore(args.vg);

		nvgSave(args.vg);
		nvgTranslate(args.vg, box.size.x * 0.5, box.size.y-150);
		nvgStrokeColor(args.vg, nvgRGB(95,95,95));
		nvgStrokeWidth(args.vg, 1);
		nvgBeginPath(args.vg);
		nvgRotate(args.vg, ( -SECTEUR * M_PI ) / 180 );
		nvgMoveTo(args.vg, 0, -LONGUEUR+LONGUEUR_TRAIT);
		nvgLineTo(args.vg, 0, -LONGUEUR);
		nvgStroke(args.vg);
		nvgRestore(args.vg);

		// // moitié
		nvgSave(args.vg);
		nvgTranslate(args.vg, box.size.x * 0.5, box.size.y-150);
		nvgStrokeColor(args.vg, nvgRGB(95,95,95));
		nvgStrokeWidth(args.vg, 1);
		nvgBeginPath(args.vg);
		nvgRotate(args.vg, ( SECTEUR/2 * M_PI ) / 180 );
		nvgMoveTo(args.vg, 0, -LONGUEUR+LONGUEUR_TRAIT);
		nvgLineTo(args.vg, 0, -LONGUEUR);
		nvgStroke(args.vg);
		nvgRestore(args.vg);

		nvgSave(args.vg);
		nvgTranslate(args.vg, box.size.x * 0.5, box.size.y-150);
		nvgStrokeColor(args.vg, nvgRGB(95,95,95));
		nvgStrokeWidth(args.vg, 1);
		nvgBeginPath(args.vg);
		nvgRotate(args.vg, ( -SECTEUR/2 * M_PI ) / 180 );
		nvgMoveTo(args.vg, 0, -LONGUEUR+LONGUEUR_TRAIT);
		nvgLineTo(args.vg, 0, -LONGUEUR);
		nvgStroke(args.vg);
		nvgRestore(args.vg);
		 

	}

		
};


struct WoltModuleUpdateLine : Widget {
	WoltModule *module;
	WoltModuleUpdateLine(){}
	int dep = 0;
	void draw(const DrawArgs &args) override {
		if(module == NULL) return; // si je suis dans le clic droit pour ajouter des plugins, j'ai plus le module, ça plante ! 

		

		float voltage = module->inputs[WoltModule::INPUTONE_INPUT].getVoltage();

		if(voltage > 10){
			voltage = 10;
		} else if(voltage < -10) {
			voltage = -10;
		}
 		
		float degrees = dep + voltage/10   * SECTEUR ; // +-5V donc on parcours 2 secteurs
		float radians = ( degrees * M_PI ) / 180 ;


		nvgSave(args.vg);
		nvgTranslate(args.vg, box.size.x * 0.5, box.size.y-150);
		nvgStrokeColor(args.vg, nvgRGB(255, 0, 0));
		nvgStrokeWidth(args.vg, 1.5);
		nvgBeginPath(args.vg);
		nvgRotate(args.vg, radians);
		nvgMoveTo(args.vg, 0, -13);
		nvgLineTo(args.vg, 0, -LONGUEUR);
		nvgStroke(args.vg);
		nvgRestore(args.vg);

		


	}

		
};


struct WoltModuleWidget : ModuleWidget {
	TextField * label;
	SegmentDisplay * segment;
	MenuItem * item;
	WoltModuleWidget(WoltModule* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/WoltModule.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(20.32, 113)), module, WoltModule::INPUTONE_INPUT));

		//addChild(createWidgetCentered<Widget>(mm2px(Vec(20.32, 70.103))));

		// label = new TextField();
		// label->box.pos = Vec(5, 52 ); // position : x, y depuis haut gauche
		// label->box.size.x = box.size.x-10;
		// label->setText("Hello worldz");
		// addChild(label);

		// segment = new SegmentDisplay();
		// segment->box.pos = Vec(40, 42 );
		// segment->box.size.x = box.size.x-75;

		//segment->draw();
		// addChild(segment);


		// item = new MenuItem();
		// item->text = "Menuuu";
		// addChild(item);
		WoltModuleDisplay *display = new WoltModuleDisplay();
		

		display->module = module;
		display->box.pos = Vec(0, 0);
		display->box.size = Vec(box.size.x, RACK_GRID_HEIGHT);
		addChild(display);

		WoltModuleUpdateLine *display2 = new WoltModuleUpdateLine();

		display2->module = module;
		display2->box.pos = Vec(0, 0);
		display2->box.size = Vec(box.size.x, RACK_GRID_HEIGHT);
		addChild(display2);

		 
	}

	

	
};


Model* modelWoltModule = createModel<WoltModule, WoltModuleWidget>("WoltModule");