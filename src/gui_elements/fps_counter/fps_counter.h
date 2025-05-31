#ifndef FPS_COUNTER_UI_H
#define FPS_COUNTER_UI_H

#include "../../core/types.h"
#include "../../gui/text/text_renderer.h"

class FpsCounterUI {
public:
    u16 renderFramesCount;
    TextRenderer textRenderer;

    u16 populatedRenderFrames;
    f64* renderTimes;

    FpsCounterUI() {}

    FpsCounterUI(u16 _renderFramesCount, TextRenderer _textRenderer) :
    renderFramesCount(_renderFramesCount),
    populatedRenderFrames(0),
    renderTimes(new f64[renderFramesCount]),
    textRenderer(_textRenderer) {}

    void addFrame(f64 frameTime) {
        // same thing
        // memmove(&renderTimes[index + 1], &renderTimes[index], (populatedRenderFrames - index) * sizeof(f64));
        memmove(&renderTimes[1], renderTimes, std::min(populatedRenderFrames, u16(renderFramesCount - 1)) * sizeof(f64));

        renderTimes[0] = frameTime;
        if (populatedRenderFrames < renderFramesCount) {
            populatedRenderFrames++;
        }
    }

    void render(u16 WINDOW_WIDTH, u16 WINDOW_HEIGHT) {
        f64 fps = 0;
        for (u16 i = 0; i < populatedRenderFrames; i++) {
            fps += renderTimes[i];
        }
        fps /= populatedRenderFrames;
        fps = 1 / fps;
        fps = std::floor(fps);

        f32 x = -f32(WINDOW_WIDTH) / 2 * 0.6;
        f32 y = -f32(WINDOW_HEIGHT) / 2 * 0.6;
        std::string fpsStr = "FPS: " + std::to_string(u32(fps));
        textRenderer.renderText(fpsStr.c_str(), x + 2.5f, y + 2.5f, 20.0f, Colors::createRGB4(8, 8, 8));
        textRenderer.renderText(fpsStr.c_str(), x, y, 20.0f, Colors::createRGB4(15, 15, 15));
    }
};

#endif