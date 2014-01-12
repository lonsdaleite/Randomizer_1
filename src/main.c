#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include <time.h>

#define MY_UUID {0xD0, 0xF7, 0xA8, 0x8D, 0xAD, 0x3F, 0x4F, 0x2C, 0x87, 0x30, 0x86, 0xC9, 0x18, 0x1F, 0x59, 0x3C}
PBL_APP_INFO_SIMPLE(MY_UUID, "Randomizer", "Lonsdaleite", 1 /* App version */);


Window window;

TextLayer textLayer;
TextLayer layer2;
Layer imglayer;

BmpContainer image;

char text_buffer [10];
char text_layer2[]="Gen. limit: ";
int num_gen=10;


char *intost(int num, char *string)
{
	int i = 0, temp_num = num, length = 0;
	
	if (num==0) {string[0]='0'; string[1]='\0'; return string;}
	
	if(num > 0) {
		while(temp_num) {
			temp_num /= 10;
			length++;
		}
		
		for(i = 0; i < length; i++) {
		 	string[(length-1)-i] = '0' + (num % 10);
			num /= 10;
		}
		string[i] = '\0'; 
	}
	else
	{string[0]='B'; string[1]='a'; string[2]='d'; string[3]='\0';}
	
	return string;
}

void layer_update_callback(Layer *me, GContext* ctx) {

  // We make sure the dimensions of the GRect to draw into
  // are equal to the size of the bitmap--otherwise the image
  // will automatically tile. Which might be what *you* want.

  GRect destination = layer_get_frame(&image.layer.layer);

  destination.origin.y = 0;
  destination.origin.x = 121;

  graphics_draw_bitmap_in_rect(ctx, &image.bmp, destination);


}


// Modify these common button handlers

void up_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
num_gen++;
	intost(num_gen,text_layer2+12);
	text_layer_set_text(&layer2, text_layer2);
}

void up_long_click_handler(ClickRecognizerRef recognizer, Window *window) {
num_gen+=10;
	intost(num_gen,text_layer2+12);
	text_layer_set_text(&layer2, text_layer2);
}


void down_single_click_handler(ClickRecognizerRef recognizer, Window *window) {

	if (num_gen>1) {
		num_gen--;
		intost(num_gen,text_layer2+12);
	text_layer_set_text(&layer2, text_layer2);
	}
}

void down_long_click_handler(ClickRecognizerRef recognizer, Window *window) {

	if (num_gen>10) {
		num_gen-=10;
		intost(num_gen,text_layer2+12);
	text_layer_set_text(&layer2, text_layer2);
	}
}


void select_single_click_handler(ClickRecognizerRef recognizer, Window *window) {

int k=rand()%(num_gen)+1;
 intost(k,text_buffer);
	text_layer_set_text(&textLayer, text_buffer);
}


void select_long_click_handler(ClickRecognizerRef recognizer, Window *window) {

num_gen=10;
	intost(num_gen,text_layer2+12);
	text_layer_set_text(&layer2, text_layer2);
	
}


// This usually won't need to be modified

void click_config_provider(ClickConfig **config, Window *window) {

  config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) select_single_click_handler;

  config[BUTTON_ID_SELECT]->long_click.handler = (ClickHandler) select_long_click_handler;

  config[BUTTON_ID_UP]->click.handler = (ClickHandler) up_single_click_handler;
  
  config[BUTTON_ID_UP]->long_click.handler = (ClickHandler) up_long_click_handler;

  config[BUTTON_ID_DOWN]->click.handler = (ClickHandler) down_single_click_handler;

  config[BUTTON_ID_DOWN]->long_click.handler = (ClickHandler) down_long_click_handler;
}


// Standard app initialisation

void handle_init(AppContextRef ctx) {

  window_init(&window, "Randomizer");
  window_stack_push(&window, true /* Animated */);


	 // text_layer_init(&layer2, window.layer.frame);
	text_layer_init(&layer2, GRect(0, 0, 121, 50));
	text_layer_set_text_alignment(&layer2, GTextAlignmentCenter);
	 text_layer_set_font(&layer2, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
	layer_add_child(&window.layer, &layer2.layer);
	text_layer_set_text(&layer2, "Gen. limit: 10");
	
		text_layer_init(&textLayer, GRect(0, 55, 121, 30));
	text_layer_set_text_alignment(&textLayer, GTextAlignmentCenter);
  text_layer_set_font(&textLayer, fonts_get_system_font(FONT_KEY_DROID_SERIF_28_BOLD));
  layer_add_child(&window.layer, &textLayer.layer);
		text_layer_set_text(&textLayer, "0");
	
	
  layer_init(&imglayer, window.layer.frame);
  imglayer.update_proc = &layer_update_callback;
  layer_add_child(&window.layer, &imglayer);

  resource_init_current_app(&RANDOMIZER_RESOURCES);
  bmp_init_container(RESOURCE_ID_IMAGE_NUM_1, &image);
	

  // Attach our desired button functionality
  window_set_click_config_provider(&window, (ClickConfigProvider) click_config_provider);
}

void handle_deinit(AppContextRef ctx) {

  bmp_deinit_container(&image);
}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
	.deinit_handler = &handle_deinit
  };
	srand(time(0));
  app_event_loop(params, &handlers);
}