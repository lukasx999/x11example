#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>

#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>


#define HEIGHT 1080
#define WIDTH 1920

#define COLOR_DARK_GRAY (CustomColor) { 0xfff,  0xfff,  0xfff  }
#define COLOR_BLUE      (CustomColor) { 0x0,    0x0,    0xffff }
#define COLOR_RED       (CustomColor) { 0xffff, 0x0,    0x0    }



typedef struct {
    Display *dpy;
    Window root;
    int scr;
    Colormap cmap;

    Window win;
    GC gc;
} App;



typedef struct {
    uint16_t red;
    uint16_t green;
    uint16_t blue;
} CustomColor;



static Window _new_window(Display *dpy, Window root, int scr, int width, int height) {

    int x = 0;
    int y = 0;
    int border_width = 0;

    Window win = XCreateSimpleWindow(dpy, root, x, y, width, height,
                                     border_width,
                                     BlackPixel(dpy, scr),
                                     BlackPixel(dpy, scr));
    assert(win != None);

    return win;

}


static GC _new_gc(Display *dpy, Window root, int scr) {
    GC gc = XCreateGC(dpy, root, 0, NULL);
    XSetBackground(dpy, gc, WhitePixel(dpy, scr));
    XSetForeground(dpy, gc, WhitePixel(dpy, scr));
    return gc;

}



App app_new(int width, int height) {

    Display *dpy = XOpenDisplay(NULL);
    assert(dpy != NULL);

    Window root = XDefaultRootWindow(dpy);
    assert(root != None);

    Colormap cmap = XDefaultColormap(dpy, 0);
    int scr = DefaultScreen(dpy);

    Window win = _new_window(dpy, root, scr, width, height);
    GC gc      = _new_gc(dpy, root, scr);

    XMapWindow(dpy, win); // show the window
    XSelectInput(dpy, win, ExposureMask | ButtonPressMask);

    App app = {
        .dpy  = dpy,
        .cmap = cmap,
        .root = root,
        .scr  = scr,
        .win  = win,
        .gc   = gc,
    };

    return app;

}

void app_destroy(App *app) {
    XFreeGC(app->dpy, app->gc);
    XDestroyWindow(app->dpy, app->root);
    XCloseDisplay(app->dpy);
}


static void change_color(App *app, CustomColor cc) {
    XColor color = { 0 };
    color.red   = cc.red;
    color.green = cc.green;
    color.blue  = cc.blue;
    color.flags = DoRed | DoGreen | DoBlue;
    XAllocColor(app->dpy, app->cmap, &color);
    XSetForeground(app->dpy, app->gc, color.pixel);
}


void app_eventloop(App *app) {

    bool quit = false;
    XEvent event = { 0 };

    while (!quit) {
        XNextEvent(app->dpy, &event);

        change_color(app, COLOR_DARK_GRAY);
        int width  = 500;
        int height = 500;
        int x1     = WIDTH/2  - width/2;
        int y1     = HEIGHT/2 - height/2;
        XFillRectangle(app->dpy, app->win, app->gc, x1, y1, width, height);

        change_color(app, COLOR_RED);
        XDrawLine(app->dpy, app->win, app->gc, x1, y1, 500, 500);

        change_color(app, COLOR_BLUE);
        XDrawString(app->dpy, app->win, app->gc, 50, 50, "foo", 3);


        switch (event.type) {
        }

    }

}



int main(void) {

    App app = app_new(WIDTH, HEIGHT);
    app_eventloop(&app);
    app_destroy(&app);

    return EXIT_SUCCESS;

}
