#include <pebble.h>

Window *window;
TextLayer *text_layer;

static char s_time_buffer[16];
static long time_count = 25*60;

static bool running = false;
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  // Update things here
  running = true;
  if(time_count == 0){
    snprintf(s_time_buffer, sizeof(s_time_buffer), "%.2d : %.2d", 25, 0);
    text_layer_set_text(text_layer, s_time_buffer);
  }
  else{
  int minute = time_count/60;
  int second = time_count - minute*60;
  
  // Update the TextLayer
  snprintf(s_time_buffer, sizeof(s_time_buffer), "%.2d : %.2d", minute, second);
  text_layer_set_text(text_layer, s_time_buffer);
  time_count--;
  }
  
}

static void button_click(){
  vibes_double_pulse();
  if(running == false){
    tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
    running = true;
  }
  else{
    tick_timer_service_unsubscribe();
    time_count = 25*60;
    snprintf(s_time_buffer, sizeof(s_time_buffer), "%.2d : %.2d", 25, 0);
    text_layer_set_text(text_layer, s_time_buffer);
    running = false;
  }
}
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  button_click();
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  button_click();
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  button_click();
}

static void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}


void handle_init(void) {
	// Create a window and text layer
	window = window_create();
	text_layer = text_layer_create(GRect(0, 0, 144, 154));
	
	// Set the text, font, and text alignment
	text_layer_set_text(text_layer, "25:00");
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
	
	// Add the text layer to the window
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));

	// Push the window
	window_stack_push(window, true);
	
  //tick time
  
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  
  //add click event
  window_set_click_config_provider(window, click_config_provider);
  
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");
}

void handle_deinit(void) {
  
  //unsubscribe tick timer
  
  tick_timer_service_unsubscribe();
  
	// Destroy the text layer
	text_layer_destroy(text_layer);
	
	// Destroy the window
	window_destroy(window);
}



int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
