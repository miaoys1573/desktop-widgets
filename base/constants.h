#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <QFont>
#include <qcolor.h>

class Constants
{
public:
    static QString MAIN_COLOR;
    static QString MAIN_BACKGROUND_1;
    static QString MAIN_BACKGROUND_2;
    static QString MAIN_BACKGROUND_3;
    static QString MAIN_BACKGROUND_4;
    static int TITLE_FONT_SIZE;
    static int CONTENT_FONT_SIZE;
    static int BUTTON_RADIUS;
    static int BUTTON_PADDING;

    static int WIDGET_WIDTH;
    static int WIDGET_HEIGHT;
    static int WIDGET_RADIUS;
    static int WIDGET_TRANSPARENCE;

    static int MAIN_SETTING_WIDGET_WIDTH;
    static int MAIN_SETTING_WIDGET_HEIGHT;

    static int CHECK_BUTTON_HEIGHT;
    static int CHECK_BUTTON_BORDER_WIDTH;

    static int CLOSE_BUTTON_HEIGHT;

    static QString RESET_BUTTON_COLOR;
    static QString RESET_BUTTON_HOVER_BACKGROUND;

    static int SETTING_ITEM_RADIUS;
    static int SETTING_ITEM_PADDING;
    static int SETTING_ITEM_MARGIN_TOP;
    static int SETTING_ITEM_TITLE_MARGIN_TOP;

    static int SLIDER_HEIGHT;

    static int PROGRESS_HEIGHT;

    static QString SYSTEM_TRAY_TOOLTIP;
};

#endif // CONSTANTS_H
