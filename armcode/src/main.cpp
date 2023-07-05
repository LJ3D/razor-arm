#include <Braccio++.h>
#include "razor_robotics.c"
#include "RobotArmController.h"
#include <CommandParser.h>

const int baudRate = 115200;
lv_obj_t *statusLabel;

RobotArmController *controller;

void showBranding()
{
    Braccio.lvgl_lock();
    lv_obj_t *img = lv_img_create(lv_scr_act());
    lv_img_set_src(img, &razor_robotics);
    lv_img_set_zoom(img, 200);
    lv_obj_align(img, NULL, LV_ALIGN_TOP_LEFT, 0);

    statusLabel = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(statusLabel, LV_LABEL_LONG_WRAP);
    lv_label_set_recolor(statusLabel, true);
    lv_label_set_text(statusLabel, "#E3E858 Not Connected#");
    lv_obj_set_width(statusLabel, 150);
    lv_obj_set_style_text_align(statusLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(statusLabel, LV_ALIGN_CENTER, 0, -80);

    Braccio.lvgl_unlock();
}

void setConnectionStatus(bool status)
{
    Braccio.lvgl_lock();
    if (!status)
    {
        lv_label_set_text(statusLabel, "#FF0000 Not Ready#");
    }
    else
    {
        lv_label_set_text(statusLabel, "#AE64D9 Ready#");
    }

    Braccio.lvgl_unlock();
}

void setup()
{
    Serial.begin(baudRate);
    Braccio.begin(&showBranding);

    while (!Serial)
        ;

    setConnectionStatus(false);

    controller = RobotArmController::Instance();

    if (!controller->Setup())
    {
        lv_label_set_text(statusLabel, "#FF0000 Setup failed#");
        while (1)
        {
            delay(1000);
        }
    }

    Serial.println("Setup successful");
}

void loop()
{
    setConnectionStatus(true);
    controller->Loop();
}