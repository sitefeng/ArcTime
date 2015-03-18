#include <pebble.h>

static Window *window;
static Layer *drawLayer;
static GBitmap *bitmap2;
static GBitmap *bitmap6;
static GBitmap *bitmap7;
static GBitmap *bitmap9;
struct tm *tick_time;

static void drawPixel(GContext *ctx, GPoint center, int32_t r, int32_t angle) {
    int32_t x = r * cos_lookup(angle)/TRIG_MAX_RATIO;
    int32_t y = r * sin_lookup(angle)/TRIG_MAX_RATIO;

    GPoint point = GPoint(center.x + x, center.y + y);
    graphics_draw_pixel(ctx, point);
}


static void drawArc(GContext *ctx, GPoint center, int32_t r, int32_t startAngle,
        int32_t endAngle) {
    
    if (endAngle >= startAngle) {
      for (int32_t angle=startAngle; angle<=endAngle; angle++) {
        drawPixel(ctx, center, r, angle);
      }
    } else {
      for (int32_t angle=startAngle; angle<=TRIG_MAX_ANGLE; angle++) {
        drawPixel(ctx, center, r, angle);
      }
      for (int32_t angle=0; angle<=endAngle; angle++) {
        drawPixel(ctx, center, r, angle);
      }
    }
}


static void drawThickArc(GContext *ctx, GPoint center, int32_t r, int32_t startAngle,
        int32_t endAngle, int32_t thickness) {
    for (int32_t i = r; i < r+thickness; ++i) {
        drawArc(ctx, center, i, startAngle, endAngle);
    }
}


static void drawBallEndArc(GContext *ctx, GPoint center, int32_t r, int32_t startAngle,
        int32_t endAngle) {
    drawThickArc(ctx, center, r, startAngle, endAngle, 2);

    int32_t x1 = r * cos_lookup(startAngle)/TRIG_MAX_RATIO + center.x;
    int32_t y1 = r * sin_lookup(startAngle)/TRIG_MAX_RATIO + center.y;
    graphics_fill_circle(ctx, GPoint(x1,y1), 3);

    int32_t x2 = r * cos_lookup(endAngle)/TRIG_MAX_RATIO + center.x;
    int32_t y2 = r * sin_lookup(endAngle)/TRIG_MAX_RATIO + center.y;
    graphics_fill_circle(ctx, GPoint(x2,y2), 3);
}


static void drawThickLine(GContext *ctx, GPoint start, GPoint end, int32_t thickness) {
    for (int i=0; i<thickness; i++) {
      graphics_draw_line(ctx, GPoint(start.x + i, start.y + i), GPoint(end.x + i, end.y + i));
    }
}

static void drawDot(GContext *ctx, GPoint p) {
  graphics_fill_circle(ctx, p, 3);
}

static int32_t randAngle() {
  int32_t angle = rand() % TRIG_MAX_ANGLE;
  return angle;
}

static void drawNumber(GContext *ctx, int number, int position) {
    GRect bounds = layer_get_bounds(drawLayer); // 144, 168
    GSize size = bounds.size;
    GPoint center = GPoint(size.w / 2, size.h / 2);

    switch (position) {
      case 0:
        center = GPoint(size.w / 4, size.h / 4);
        break;
      case 1:
        center = GPoint(size.w * 3 / 4, size.h / 4);
        break;
      case 2:
        center = GPoint(size.w / 4, size.h * 3 / 4);
        break;
      case 3:
        center = GPoint(size.w * 3 / 4, size.h * 3 / 4);
        break;
    }

    switch (number) {
      case 0: {
        int32_t randAngle1 = randAngle();
        int32_t randAngle2 = randAngle();
        drawBallEndArc(ctx, center, 20, randAngle1 , (int)(randAngle1 + TRIG_MAX_ANGLE*0.75) % TRIG_MAX_ANGLE);
        drawBallEndArc(ctx, center, 30, randAngle2, (int)(randAngle2 + TRIG_MAX_ANGLE*0.75) % TRIG_MAX_ANGLE);
        break;
      }
      case 1: {
        GPoint p1 = GPoint(center.x - 10, center.y - 10);
        GPoint p2 = GPoint(center.x - 20, center.y - 10);
        GPoint p3 = GPoint(center.x + 5, center.y - 35);
        GPoint p4 = GPoint(center.x + 5, center.y + 25);
        GPoint p5 = GPoint(center.x - 7, center.y + 25);

        drawDot(ctx, p1);
        drawThickLine(ctx, p1, p2, 2);
        drawThickLine(ctx, p2, p3, 2);
        drawThickLine(ctx, p3, p4, 2);
        drawThickLine(ctx, p4, p5, 2);
        drawDot(ctx, p5);

        break;
      }
      case 2: {
        if (!bitmap2) {
          bitmap2 = gbitmap_create_with_resource(RESOURCE_ID_TWO);
        }
        graphics_draw_bitmap_in_rect(ctx, bitmap2, GRect(center.x - size.w/4, center.y - size.h/4, size.w/2, size.h/2));
        drawBallEndArc(ctx, GPoint(center.x, center.y - 10), 25, TRIG_MAX_ANGLE * 0.51, TRIG_MAX_ANGLE *0.1);
        drawDot(ctx, GPoint(center.x + 19, center.y + 22));
        break;
      }
      case 3: {
        drawBallEndArc(ctx, GPoint(center.x, center.y - 15), 15, TRIG_MAX_ANGLE * 0.45, TRIG_MAX_ANGLE *0.25);
        drawBallEndArc(ctx, GPoint(center.x, center.y + 15), 15, TRIG_MAX_ANGLE * 0.75, TRIG_MAX_ANGLE *0.45);
        break;
      }
      case 4: {
        drawBallEndArc(ctx, GPoint(center.x, center.y - 10), 23, TRIG_MAX_ANGLE * 0.1, TRIG_MAX_ANGLE *0.65);
        GPoint p1 = GPoint(center.x, center.y - 15);
        GPoint p2 = GPoint(center.x, center.y + 30);
        GPoint p3 = GPoint(center.x - 10, center.y + 30);
        drawDot(ctx, p1);
        drawThickLine(ctx, p1, p2, 2);
        drawThickLine(ctx, p2, p3, 2);
        drawDot(ctx, p3);
        break;
      }
      case 5: {
        drawBallEndArc(ctx, GPoint(center.x, center.y + 11.5), 17, TRIG_MAX_ANGLE * 0.6, TRIG_MAX_ANGLE *0.4);
        GPoint p1 = GPoint(center.x + 17, center.y - 27);
        GPoint p2 = GPoint(center.x - 14, center.y - 27);
        GPoint p3 = GPoint(center.x - 14, center.y);
        drawDot(ctx, p1);
        drawThickLine(ctx, p1, p2, 2);
        drawThickLine(ctx, p2, p3, 2);
        break;
      }
      case 6: {
        if (!bitmap6) {
          bitmap6 = gbitmap_create_with_resource(RESOURCE_ID_SIX);
        }
        graphics_draw_bitmap_in_rect(ctx, bitmap6, GRect(center.x - size.w/4, center.y - size.h/4, size.w/2, size.h/2));
        drawBallEndArc(ctx, GPoint(center.x, center.y + 11), 22, TRIG_MAX_ANGLE * 0.7, TRIG_MAX_ANGLE *0.4);
        drawDot(ctx, GPoint(center.x + 3, center.y - 34));
      }
        break;
      case 7: {
        if (!bitmap7) {
          bitmap7 = gbitmap_create_with_resource(RESOURCE_ID_SEVEN);
        }
        graphics_draw_bitmap_in_rect(ctx, bitmap7, GRect(center.x - size.w/4, center.y - size.h/4, size.w/2, size.h/2));
        GPoint p1 = GPoint(center.x - 25, center.y - 22);
        GPoint p2 = GPoint(center.x - 15, center.y + 25);
        drawDot(ctx, p1);
        drawDot(ctx, p2);
        break;
      }
      case 8: {
        int32_t randAngle1 = randAngle();
        int32_t randAngle2 = randAngle();
        drawBallEndArc(ctx, GPoint(center.x, center.y - 8), 25, TRIG_MAX_ANGLE * 0.37, TRIG_MAX_ANGLE *0.13);
        drawBallEndArc(ctx, GPoint(center.x, center.y + 8), 25, TRIG_MAX_ANGLE * 0.87, TRIG_MAX_ANGLE *0.63);
        drawThickArc(ctx, GPoint(center.x, center.y - 10), 10, randAngle1, (int)(randAngle1 + TRIG_MAX_ANGLE*0.85) % TRIG_MAX_ANGLE, 2);
        drawThickArc(ctx, GPoint(center.x, center.y + 10), 10, randAngle2, (int)(randAngle2 + TRIG_MAX_ANGLE*0.85) % TRIG_MAX_ANGLE, 2);
        break;
      }
      case 9: {
        if (!bitmap9) {
          bitmap9 = gbitmap_create_with_resource(RESOURCE_ID_NINE);
        }
        graphics_draw_bitmap_in_rect(ctx, bitmap9, GRect(center.x - size.w/4, center.y - size.h/4, size.w/2, size.h/2));
        drawBallEndArc(ctx, GPoint(center.x - 3, center.y - 12), 23, TRIG_MAX_ANGLE * 0.25, TRIG_MAX_ANGLE *0.95);
        drawDot(ctx, GPoint(center.x - 5, center.y + 29));
        break;
      }
    }
}


static void draw_update_proc(Layer *this_layer, GContext *ctx) {

    // Get the time.
    time_t temp = time(NULL);
    tick_time = localtime(&temp);
    int32_t hours = tick_time->tm_hour;
    int32_t minutes = tick_time->tm_min;

    #ifdef PBL_COLOR
    graphics_context_set_stroke_color(ctx, GColorMelon);
    graphics_context_set_fill_color(ctx, GColorMelon);
    #else
    graphics_context_set_stroke_color(ctx, GColorWhite);
    graphics_context_set_fill_color(ctx, GColorWhite);
    #endif

    int32_t hr1 = hours / 10;
    int32_t hr2 = hours % 10;
    int32_t min1 = minutes/10;
    int32_t min2 = minutes % 10;
    drawNumber(ctx, hr1, 0);
    drawNumber(ctx, hr2, 1);
    drawNumber(ctx, min1, 2);
    drawNumber(ctx, min2, 3);
}


static void tick_handler(struct tm *tick_time, TimeUnits units_change) {
    layer_mark_dirty(drawLayer);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  window_set_background_color(window, GColorBlack);

  // Draw Layer
  drawLayer = layer_create(bounds);
  layer_set_update_proc(drawLayer, draw_update_proc);
  layer_add_child(window_layer, drawLayer);
}

static void window_unload(Window *window) {
  layer_destroy(drawLayer);
}

static void init(void) {
  window = window_create();

  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

  window_set_fullscreen(window, true);
  window_stack_push(window, true);

  // Register with TickTimerService.
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

