#ifndef HOTBAR_UI_H
#define HOTBAR_UI_H

#include "../../core/types.h"
#include "../../gui/text/text_renderer.h"
#include "../../gui/image/image_renderer.h"

class HotbarUI {
public:
    Vec2<f32> pos;

    u8 slot;
    ImageRendererUI hotbarRenderer;
    ImageRendererUI hotbarSelectionRenderer;
    f32 scale;

    HotbarUI() {}

    HotbarUI(Vec2<f32> _pos, u8 _slot, f32 _scale, const Shader& shader) :
    pos(_pos),
    slot(_slot),
    hotbarRenderer(ImageRendererUI::empty()),
    hotbarSelectionRenderer(ImageRendererUI::empty()),
    scale(_scale) {
        hotbarRenderer.shader = shader;
        hotbarRenderer.loadImage("assets/textures/gui/hotbar.png");

        hotbarSelectionRenderer.shader = shader;
        hotbarSelectionRenderer.loadImage("assets/textures/gui/hotbar_selection.png");

        update();
    }

    // call after changing pos
    void update() {
        hotbarRenderer.pos = pos;
        hotbarRenderer.size = Vec2<f32>(9.0f, 1.0f) * scale;
        hotbarRenderer.update();

        hotbarSelectionRenderer.size = Vec2<f32>(1.0f, 1.0f) * scale;
        updateSelection();
    }

    void setSlot(u8 _slot) {
        slot = _slot;
        updateSelection();
    }

    void updateSelection() {
        hotbarSelectionRenderer.pos = pos + Vec2<f32>(
            (slot - 4) * scale,
            0.0f
        );
        hotbarSelectionRenderer.update();
    }

    void render(u16 WINDOW_WIDTH, u16 WINDOW_HEIGHT) {
        hotbarRenderer.render(WINDOW_WIDTH, WINDOW_HEIGHT);
        hotbarSelectionRenderer.render(WINDOW_WIDTH, WINDOW_HEIGHT);
    }
};

#endif