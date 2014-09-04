#ifndef VIDEO_CONFIG_LOOP_H
#define VIDEO_CONFIG_LOOP_H

#include <allegro5/allegro.h>

#include <tgui2_widgets.hpp>

#include <vector>

#include "loop.h"
#include "wrap.h"
#include "general.h"
#include "widgets.h"

class Video_Config_Loop : public Loop {
public:
	// Loop interface
	bool init();
	void top();
	bool handle_event(ALLEGRO_EVENT *event);
	bool logic();
	void draw();

	Video_Config_Loop();
	virtual ~Video_Config_Loop();

private:

	W_Scrolling_List *mode_list;
	TGUI_Checkbox *checkbox;
	W_Button *save_button;
	W_Button *cancel_button;
	std::vector< std::pair<int, int> > modes;
	bool list_was_activated;
};

#endif // VIDEO_CONFIG_LOOP_H
