#include <stdio.h>

#include "gui.h"

void slider_callback(Fl_Value_Slider* slider, void* data)
{
	printf("Slider value:%f\n", slider->value());
	//return 0;
}

int main()
{
	Fl_Double_Window* root = make_window();
	root->show();
	return Fl::run();
}
