/*
* Copyright 2023 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
* mingw32-make run .\build\bin\simulator.exe
*/


/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lvgl.h"
#include "custom.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**
 * Create a demo application
 */
static int get_result(int old, int oper, int value)
{
    int result = 0;
    switch (oper) 
    {
    case 0:
        result = value;
        break;
    case 3:
        result = old / value;
        break;
    case 7:
        result = old * value;
        break;
    case 11:
        result = old - value;
        break;
    case 15:
        result = old + value;
        break;
    default:
        break;
    }
    return result;
}

static void my_event_handle(lv_event_t * e)
{
    lv_ui* ui = lv_event_get_user_data(e);
    uint32_t id = lv_btnmatrix_get_selected_btn(ui->screen_btnm_1);
    const char* txt = lv_btnmatrix_get_btn_text(ui->screen_btnm_1, id);
    static int old = 0;
    static int oper = 0;
    int value = 0;
    static char num[10]  = {0};

    switch(id)
    {
       
        case 3:
        case 7:
        case 11:
        case 15:
            sscanf(num, "%d", &value);
            old = get_result(old, oper, value);
            lv_label_set_text_fmt(ui->screen_label_1, "%d", old);
            oper = id;
            memset(num, 0, sizeof(num));
            break;
        case 19:
            sscanf(num, "%d", &value);
            old = get_result(old, oper, value);
            lv_label_set_text_fmt(ui->screen_label_1, "%d", old);
            oper = 0;
            memset(num, 0, sizeof(num));
            break;
        case 0:
            lv_label_set_text(ui->screen_label_1, "0");
            oper = 0;
            old = 0;
            memset(num, 0, sizeof(num));
            break;
        default:
            if (strlen(num) < sizeof(num) - 1) 
            {
                strcat(num, txt);
                lv_label_set_text(ui->screen_label_1, num);
            }
            break;
    }

}

void custom_init(lv_ui *ui)
{
    /* Add your codes here */
    lv_obj_add_event_cb(ui->screen_btnm_1, my_event_handle, LV_EVENT_PRESSED, ui);
}

