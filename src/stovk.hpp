// © 2017 Loris Witschard

/////////////////////////////////////
//   STRING TO VIRTUAL KEY
/////////////////////////////////////

#include <string>
#include <algorithm>
#include <winuser.h>

WORD stovk(const std::string & input)
{
    std::string str;
    std::transform(input.begin(), input.end(), std::back_inserter(str), toupper);

    if(str.size() == 1)
    {
        auto value = str.front();
        if((value >= '0' && value <= '9') || (value >= 'A' && value <= 'Z'))
            return static_cast<WORD>(value);
    }

    if(str == "BACKSPACE" || str == "BACK")
        return VK_BACK;

    if(str == "TABULATOR" || str == "TAB")
        return VK_TAB;

    if(str == "ENTER" || str == "RETURN")
        return VK_RETURN;

    if(str == "SHIFT")
        return VK_SHIFT;

    if(str == "CTRL" || str == "CONTROL")
        return VK_CONTROL;

    if(str == "ALT")
        return VK_MENU;

    if(str == "PAUSE")
        return VK_PAUSE;

    if(str == "CAPSLOCK" || str == "CAPS LOCK" || str == "CAPS" || str == "CAPITAL")
        return VK_CAPITAL;

    if(str == "ESC" || str == "ESCAPE")
        return VK_ESCAPE;

    if(str == " " || str == "SPACEBAR" || str == "SPACE")
        return VK_SPACE;

    if(str == "PAGEUP" || str == "PAGE UP" || str == "PGUP" || str == "PRIOR")
        return VK_PRIOR;

    if(str == "PAGEDOWN" || str == "PAGE DOWN" || str == "PGDN" || str == "NEXT")
        return VK_NEXT;

    if(str == "END")
        return VK_END;

    if(str == "HOME")
        return VK_HOME;

    if(str == "LEFTARROW" || str == "LEFT ARROW" || str == "LEFT")
        return VK_LEFT;

    if(str == "UPARROW" || str == "UP ARROW" || str == "UP")
        return VK_UP;

    if(str == "RIGHTARROW" || str == "RIGHT ARROW" || str == "RIGHT")
        return VK_RIGHT;

    if(str == "DOWNARROW" || str == "DOWN ARROW" || str == "DOWN")
        return VK_DOWN;

    if(str == "PRINTSCREEN" || str == "PRINT SCREEN" || str == "PRTSCN"
        || str == "SCREENSHOT" || str == "SNAPSHOT")
            return VK_SNAPSHOT;

    if(str == "INS" || str == "INSERT")
        return VK_INSERT;

    if(str == "DEL" || str == "DELETE")
        return VK_DELETE;

    if(str == "WINDOWS" || str == "WIN")
        return VK_LWIN;

    if(str == "DEL" || str == "DELETE")
        return VK_DELETE;

    if(str == "DEL" || str == "DELETE")
        return VK_DELETE;

    if(str == "F1") return VK_F1;
    if(str == "F2") return VK_F2;
    if(str == "F3") return VK_F3;
    if(str == "F4") return VK_F4;
    if(str == "F5") return VK_F5;
    if(str == "F6") return VK_F6;
    if(str == "F7") return VK_F7;
    if(str == "F8") return VK_F8;
    if(str == "F9") return VK_F9;
    if(str == "F10") return VK_F10;
    if(str == "F11") return VK_F11;
    if(str == "F12") return VK_F12;

    if(str == "+" || str == "PLUS")
        return VK_OEM_PLUS;

    if(str == "," || str == "COMMA")
        return VK_OEM_COMMA;

    if(str == "-" || str == "MINUS")
        return VK_OEM_MINUS;

    if(str == "." || str == "DOT" || str == "FULLSTOP" || str == "FULL STOP" || str == "PERIOD")
        return VK_OEM_PERIOD;

    return 0;
}
