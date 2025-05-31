#ifndef HOTBAR_UI_H
#define HOTBAR_UI_H

#include "../../core/types.h"
#include "../../gui/text/text_renderer.h"
#include "../../gui/image/image_renderer.h"

class HotbarUI {
public:
    u8 slot;
    ImageRendererUI hotbarRenderer;
    ImageRendererUI hotbarSelectionRenderer;

    HotbarUI() {}

    HotbarUI(u8 _slot) :
    slot(_slot),
    hotbarRenderer(ImageRendererUI::empty()),
    hotbarSelectionRenderer(ImageRendererUI::empty()) {
        imageRendererUI.loadImage("assets/textures/gui/hotbar.png");
        hotbarSelectionRenderer.loadImage("assets/textures/gui/hotbar_selection.png");

        // each slot in hotbar is 20x20
    }

    void setSlot() {

    }

    void render(u16 WINDOW_WIDTH, u16 WINDOW_HEIGHT) {
        
    }
};

#endif