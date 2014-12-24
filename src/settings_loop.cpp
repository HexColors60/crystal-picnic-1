#include <tgui2.hpp>

#include "settings_loop.h"
#include "engine.h"
#include "widgets.h"
#include "input_config_loop.h"
#include "video_config_loop.h"
#include "language_config_loop.h"

bool Settings_Loop::init()
{
	if (inited) {
		return true;
	}
	Loop::init();

	video_button = new W_Translated_Button("CONFIG_VIDEO");
	keyboard_button = new W_Translated_Button("CONFIG_KEYBOARD");
	gamepad_button = new W_Translated_Button("CONFIG_GAMEPAD");
	language_button = new W_Translated_Button("CONFIG_LANGUAGE");
	return_button = new W_Translated_Button("RETURN");

	int maxw = video_button->getWidth();
	maxw = MAX(maxw, keyboard_button->getWidth());
	maxw = MAX(maxw, gamepad_button->getWidth());
	maxw = MAX(maxw, language_button->getWidth());
	maxw = MAX(maxw, return_button->getWidth());

#ifdef ALLEGRO_IPHONE
	tgui::TGUIWidget *w[] = {
		language_button,
		return_button
	};
	int nw = 2;
#elif defined ALLEGRO_ANDROID || defined ALLEGRO_RASPBERRYPI
	tgui::TGUIWidget *w[] = {
		gamepad_button,
		language_button,
		return_button
	};
	int nw = 3;
#else
	tgui::TGUIWidget *w[] = {
		video_button,
		keyboard_button,
		gamepad_button,
		language_button,
		return_button
	};
	int nw = 5;
#endif

	for (int i = 0; i < nw; i++) {
		w[i]->setX(cfg.screen_w/2-maxw/2);
		w[i]->setY(cfg.screen_h/2-((General::get_font_line_height(General::FONT_LIGHT)+4)*nw)/2+(General::get_font_line_height(General::FONT_LIGHT)+4)*i+2);
		tgui::addWidget(w[i]);
	}

	tgui::setFocus(return_button);

	return true;
}

void Settings_Loop::top()
{
}

bool Settings_Loop::handle_event(ALLEGRO_EVENT *event)
{
	if (event->type == ALLEGRO_EVENT_KEY_DOWN) {
		if (
			event->keyboard.keycode == ALLEGRO_KEY_ESCAPE
#if defined ALLEGRO_ANDROID
			|| event->keyboard.keycode == ALLEGRO_KEY_BUTTON_B
			|| event->keyboard.keycode == ALLEGRO_KEY_BACK
#endif
		) {
			std::vector<Loop *> loops;
			loops.push_back(this);
			engine->fade_out(loops);
			engine->unblock_mini_loop();
			return true;
		}
	}
	else if (event->type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN) {
		if (event->joystick.button == cfg.joy_ability[2]) {
			std::vector<Loop *> loops;
			loops.push_back(this);
			engine->fade_out(loops);
			engine->unblock_mini_loop();
			return true;
		}
	}

	return false;
}

bool Settings_Loop::logic()
{
	tgui::TGUIWidget *w = tgui::update();

	if (w == video_button) {
		std::vector<Loop *> this_loop;
		this_loop.push_back(this);
		engine->fade_out(this_loop);
		Video_Config_Loop *l = new Video_Config_Loop();
		tgui::hide();
		tgui::push(); // popped in ~Video_Config_Loop()
		std::vector<Loop *> loops;
		l->init();
		loops.push_back(l);
		engine->fade_in(loops);
		engine->do_blocking_mini_loop(loops, NULL);
		engine->fade_in(this_loop);
	}
	else if (w == keyboard_button) {
		std::vector<Loop *> this_loop;
		this_loop.push_back(this);
		engine->fade_out(this_loop);
		Input_Config_Loop *l = new Input_Config_Loop(true);
		tgui::hide();
		tgui::push(); // popped in ~Input_Config_Loop()
		std::vector<Loop *> loops;
		l->init();
		loops.push_back(l);
		engine->fade_in(loops);
		engine->do_blocking_mini_loop(loops, NULL);
		engine->fade_in(this_loop);
	}
	else if (w == gamepad_button) {
		std::vector<Loop *> this_loop;
		this_loop.push_back(this);
		engine->fade_out(this_loop);
		Input_Config_Loop *l = new Input_Config_Loop(false);
		tgui::hide();
		tgui::push(); // popped in ~Input_Config_Loop()
		std::vector<Loop *> loops;
		l->init();
		loops.push_back(l);
		engine->fade_in(loops);
		engine->do_blocking_mini_loop(loops, NULL);
		engine->fade_in(this_loop);
	}
	else if (w == language_button) {
		std::vector<Loop *> this_loop;
		this_loop.push_back(this);
		engine->fade_out(this_loop);
		Language_Config_Loop *l = new Language_Config_Loop();
		tgui::hide();
		tgui::push(); // popped in ~Language_Config_Loop()
		std::vector<Loop *> loops;
		l->init();
		loops.push_back(l);
		engine->fade_in(loops);
		engine->do_blocking_mini_loop(loops, NULL);
		engine->fade_in(this_loop);
	}
	else if (w == return_button) {
		std::vector<Loop *> loops;
		loops.push_back(this);
		engine->fade_out(loops);
		engine->unblock_mini_loop();
		return true;
	}

	return false;
}

void Settings_Loop::draw()
{
	al_clear_to_color(General::UI_GREEN);

	tgui::draw();
}

Settings_Loop::Settings_Loop()
{
}

Settings_Loop::~Settings_Loop()
{
	video_button->remove();
	delete video_button;
	keyboard_button->remove();
	delete keyboard_button;
	gamepad_button->remove();
	delete gamepad_button;
	language_button->remove();
	delete language_button;
	return_button->remove();
	delete return_button;

	tgui::pop(); // pushed beforehand
	tgui::unhide();
}

