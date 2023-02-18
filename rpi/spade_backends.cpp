#include "spade_backends.hpp"
#include "pemsa/memory/pemsa_memory_module.hpp"

uint16_t color16(uint8_t r, uint8_t b, uint8_t g) {
    r = (uint8_t)((float)((float)r / 255.0f) * 31.0f);
    g = (uint8_t)((float)((float)g / 255.0f) * 31.0f);
    b = (uint8_t)((float)((float)b / 255.0f) * 63.0f);
    // return ((r & 0xf8) << 8) + ((g & 0xfc) << 3) + (b >> 3);
    return ((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | (b >> 3);
}

static Color palette[] = {
color16(0, 0, 0),
color16(29, 43, 83),
color16(126, 37, 83),
color16(0, 135, 81),
color16(171, 82, 54),
color16(95, 87, 79),
color16(194, 195, 199),
color16(255, 241, 232),
color16(255, 0, 77),
color16(255, 163, 0),
color16(255, 236, 39),
color16(0, 228, 54),
color16(41, 173, 255),
color16(131, 118, 156),
color16(255, 119, 168),
color16(255, 204, 170),

color16(41, 24, 2),
color16(17, 29, 53),
color16(66, 33, 54),
color16(18, 83, 89),
color16(116, 47, 41),
color16(73, 51, 59),
color16(162, 136, 121),
color16(243, 239, 125),
color16(190, 18, 80),
color16(255, 108, 36),
color16(168, 231, 46),
color16(0, 181, 67),
color16(6, 90, 181),
color16(117, 70, 101),
color16(255, 110, 89),
color16(255, 157, 12)
};

SpadeGraphicsBackend::SpadeGraphicsBackend() {
    fps = 0;
}

SpadeGraphicsBackend::~SpadeGraphicsBackend() {
}

void SpadeGraphicsBackend::render() { }
void SpadeGraphicsBackend::createSurface() { }
void SpadeGraphicsBackend::flip() { }
int SpadeGraphicsBackend::getFps() {
    return fps;
}


/** Each byte has two pixels of values.
    getPixel(0, 0) -> pixel[0] & 0x0f
    getPixel(1, 0) -> pixel[0] >> 4
    getPixel(2, 0) -> pixel[1] & 0x0f
  */
Color SpadeGraphicsBackend::getPixel(int x, int y) {
    uint8_t* ram = this->emulator->getMemoryModule()->ram;
    // y * 64 + x / 2
    int i = (y << 6) | (x >> 1);
    uint8_t val = ram[i + PEMSA_RAM_SCREEN];
    return palette[1 & x ? val >> 4 : val & 0x0f];
}


SpadeInputBackend::SpadeInputBackend()
{
}


SpadeInputBackend::~SpadeInputBackend()
{
}


void SpadeInputBackend::callPress(int button) {
    this->buttonState |= 1 << button;
}


bool
SpadeInputBackend::isButtonDown(int i, int p)
{
    if (i < 0 || i > PEMSA_BUTTON_COUNT - 1 || p < 0 || p > PEMSA_PLAYER_COUNT - 1) {
        return false;
    }
    return this->buttonState & (1 << i);
}


bool
SpadeInputBackend::isButtonPressed(int i, int p)
{
    return this->buttonState & (1 << i);
}


void
SpadeInputBackend::update()
{
    this->lastButtonState = this->buttonState;
    this->buttonState = 0;
}


int
SpadeInputBackend::getMouseX()
{
    return -1;
}


int
SpadeInputBackend::getMouseY()
{
    return -1;
}


int
SpadeInputBackend::getMouseMask()
{
    return 0;
}


const char *
SpadeInputBackend::readKey()
{
    return "";
}


bool
SpadeInputBackend::hasKey()
{
    return false;
}


void
SpadeInputBackend::reset()
{
}


const char *
SpadeInputBackend::getClipboardText()
{
    return "";
}



SpadeAudioBackend::~SpadeAudioBackend()
{
}


void
SpadeAudioBackend::setupBuffer()
{
}

int SpadeAudioBackend::fillSampleBuffer(int16_t *samples, int size) {
    PemsaAudioModule* audioModule = ((PemsaEmulator*) emulator)->getAudioModule();

    for (int i = 0; i < size; i++) {
        samples[i] = (int16_t) (audioModule->sample() * INT16_MAX);
    }
    return size;
}
