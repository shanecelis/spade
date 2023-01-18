#ifndef SPADE_GRAPHICS_BACKEND_H_
#define SPADE_GRAPHICS_BACKEND_H_

#include "pemsa/pemsa.hpp"

uint16_t color16(uint8_t r, uint8_t b, uint8_t g);

typedef uint16_t Color;

class SpadeGraphicsBackend : public PemsaGraphicsBackend {

    public:
        SpadeGraphicsBackend();
        ~SpadeGraphicsBackend();

        void createSurface() override;
        void flip() override;

        void render() override;

        int getFps() override;
        int fps;
        Color getPixel(int x, int y);
};
class SpadeInputBackend : public PemsaInputBackend {

    public:
        SpadeInputBackend();
        ~SpadeInputBackend();

        bool isButtonDown(int i, int p) override;
        bool isButtonPressed(int i, int p) override;
        void update() override;

        int getMouseX() override;
        int getMouseY() override;
        int getMouseMask() override;

        const char* readKey() override;
        bool hasKey() override;
        void reset() override;

        const char* getClipboardText() override;
        void callPress(int button);
    private:
        int buttonState;
        int lastButtonState;

};
class SpadeAudioBackend : public PemsaAudioBackend {

    public:
        ~SpadeAudioBackend();

        void setupBuffer() override;
        int fillSampleBuffer(int16_t *samples, int size);
};

#endif // SPADE_GRAPHICS_BACKEND_H_
