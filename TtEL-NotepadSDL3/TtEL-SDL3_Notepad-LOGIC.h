// 
/**
* @file TtEL-SDL3_Notepad-LOGIC.h
* @brief A header file of the TtEL SDL3 GUI Notepad, containing logic 
* It is based on the one for the TtEL SDL3 GUI Calcuator
* @author Team theEeveeLovers Sean
* @date May 27th, 2024
*/
#ifndef SDL3_CALC_LOGIC
#define SDL3_CALC_LOGIC

#include <SDL.h>

extern SDL_Renderer* main_renderer;

/**
* @brief A function that checks if the mouse is inside a given rectangle.
* @details If you really want, you could simplify it to: if any point is inside a given rectangle.
* However, this code was specifically written for the purpose of checking mice.
* @param mouseX - The X position of the mouse
* @param mouseY - The Y position of the mouse
* @param checkRect - A pointer to the rectangle for checking
*
* @return (true) if the mouse is inside the input rectangle.
*/
bool isMouseInRectangle(int mouseX, int mouseY, SDL_Rect* checkRect) {

    // if the mouse is in the horizontal confines of the rectangle
    // mouseX >= checkRect->x makes sure the mouse isn't left of the rectangle
    // and mouseX < checkRect->x + checkRect->w makes sure the mouse isn't right of the rectangle.
    if (mouseX >= checkRect->x && mouseX < checkRect->x + checkRect->w) {

        // if the mouse is in the vertical confines of the rectangle
        // mouseY >= checkRect->y makes sure the mouse isn't above the rectangle
        // and mouseY < checkRect->y + checkRect->h makes sure the mouse isn't below the rectangle.
        if (mouseY >= checkRect->y && mouseY < checkRect->y + checkRect->h) {

            return true; // the mouse is both in the horizontal and vertical confines of the rectangle.

        }
        else {

            return false;   // the mouse is in the horizontal confines of the rectangle, 
            // however it is not within the vertical confines.

        }
    }
    else {

        return false; // the mouse is not in the horizontal confines of the rectangle.

    }
}

/**
* @brief A function that checks if the mouse (in floating point precision) is inside a given float rectangle.
*
* @param mouseX - The X position of the mouse.
* @param mouseY - The Y position of the mouse.
* @param checkRect - A pointer to the floating-point rectangle for checking.
*
* @return (true) if the mouse is inside the input rectangle.
*/
bool isFMouseInFRectangle(float mouseX, float mouseY, SDL_FRect* checkRect) {
    float scaleX = 1.f; // X Axis Rendering Scale
    float scaleY = 1.f; // Y Axis Rendering Scale
    SDL_GetRenderScale(main_renderer, &scaleX, &scaleY);

    // Make a scaled rect
    SDL_FRect scaledRect = { checkRect->x * scaleX, checkRect->y * scaleY, checkRect->w * scaleX, checkRect->h * scaleY }; // Scaled rect

    // if the mouse is in the horizontal confines of the rectangle
    // mouseX >= checkRect->x makes sure the mouse isn't left of the rectangle
    // and mouseX < checkRect->x + checkRect->w makes sure the mouse isn't right of the rectangle.
    if (mouseX >= scaledRect.x && mouseX < scaledRect.x + scaledRect.w) {

        // if the mouse is in the vertical confines of the rectangle
        // mouseY >= checkRect->y makes sure the mouse isn't above the rectangle
        // and mouseY < checkRect->y + checkRect->h makes sure the mouse isn't below the rectangle.
        if (mouseY >= scaledRect.y && mouseY < scaledRect.y + scaledRect.h) {

            return true; // the mouse is both in the horizontal and vertical confines of the rectangle.

        }
        else {

            return false; // the mouse is in the horizontal confines of the rectangle, 
            // however it is not within the vertical confines.

        }
    }
    else {

        return false; // the mouse is not in the horizontal confines of the rectangle.

    }
}

/**
* @brief A function that checks if the mouse is inside a given float rectangle.
* @brief If you really want, you could simplify it to: if any point is inside a given rectangle.
*
* However, this code was specifically written for the purpose of checking mice.
* @deprecated Deprecated in SDL3 since Mouse Position is now stored as a float.
* This is now a wrapper for @code isFMouseInFRectangle() @endcode
* It essentially was always one as it used the same code
*
* @param mouseX - The X position of the mouse.
* @param mouseY - The Y position of the mouse.
* @param checkRect - A pointer to the floating-point rectangle for checking.
*
* @return (true) if the mouse is inside the input rectangle.
*/
bool isMouseInFRectangle(int mouseX, int mouseY, SDL_FRect* checkRect) {
    float mouseX_f = static_cast<float>(mouseX); // static cast mouse variable to float
    float mouseY_f = static_cast<float>(mouseY); // static cast mouse variable to float

    return isFMouseInFRectangle(mouseX_f, mouseY_f, checkRect);
}








#endif