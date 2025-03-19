#include "HueCycler.h"

ColorHolder::ColorHolder() : r(0), g(0), b(0), a(1) {}

ColorHolder::ColorHolder(float red, float green, float blue, float alpha)
    : r(red), g(green), b(blue), a(alpha) {}

int ColorHolder::toRGBInt() const {
    int ir = static_cast<int>(r * 255);
    int ig = static_cast<int>(g * 255);
    int ib = static_cast<int>(b * 255);
    return (ir << 16) | (ig << 8) | ib;
}

int HueCycler::HSBtoRGB(float hue, float saturation, float brightness) {
    int r, g, b;

    if (saturation == 0) {
        r = g = b = static_cast<int>(brightness * 255);
    } else {
        float h = hue * 6; // Convert to [0, 6)
        int i = static_cast<int>(h);
        float f = h - i; // Frac part of h
        float p = brightness * (1 - saturation);
        float q = brightness * (1 - f * saturation);
        float t = brightness * (1 - (1 - f) * saturation);

        switch (i % 6) {
            case 0: r = static_cast<int>(brightness * 255); g = static_cast<int>(t * 255); b = static_cast<int>(p * 255); break;
            case 1: r = static_cast<int>(q * 255); g = static_cast<int>(brightness * 255); b = static_cast<int>(p * 255); break;
            case 2: r = static_cast<int>(p * 255); g = static_cast<int>(brightness * 255); b = static_cast<int>(t * 255); break;
            case 3: r = static_cast<int>(p * 255); g = static_cast<int>(q * 255); b = static_cast<int>(brightness * 255); break;
            case 4: r = static_cast<int>(t * 255); g = static_cast<int>(p * 255); b = static_cast<int>(brightness * 255); break;
            case 5: r = static_cast<int>(brightness * 255); g = static_cast<int>(p * 255); b = static_cast<int>(q * 255); break;
        }
    }
    return (r << 16) | (g << 8) | b;
}

HueCycler::HueCycler(int cycles) : index(0) {
    if (cycles <= 0) {
        throw std::invalid_argument("cycles <= 0");
    }
    colorCycle.resize(cycles);
    hueMultiplier = 1.0f / static_cast<float>(cycles);
    for (int i = 0; i < cycles; ++i) {
        colorCycle[i] = HSBtoRGB(i * hueMultiplier, 1.0f, 1.0f);
    }
}

void HueCycler::reset() {
    set(0);
}

void HueCycler::set(int indexIn) {
    index = indexIn;
}

ColorHolder HueCycler::setCurrent() {
    int rgb = colorCycle[index];
    float red = ((rgb >> 16) & 0xFF) / 255.0f;
    float green = ((rgb >> 8) & 0xFF) / 255.0f;
    float blue = (rgb & 0xFF) / 255.0f;
    return ColorHolder(red, green, blue);
}

void HueCycler::setNext() {
    setNext(1.0f);
}

void HueCycler::setNext(float alpha) {
    inc();
    ColorHolder currentColor = currentRgb();
    setColor(currentColor.r, currentColor.g, currentColor.b, alpha);
}

int HueCycler::currentInt() const {
    return colorCycle[index];
}

ColorHolder HueCycler::currentRgba(int alpha) {
    ColorHolder color = currentRgb();
    color.a = static_cast<float>(alpha) / 255.0f; // Normalize alpha
    return color;
}

ColorHolder HueCycler::currentRgb() {
    return hexToRgb(currentInt());
}

void HueCycler::operator+=(int plus) {
    index += plus;
    if (index >= colorCycle.size()) index = 0;
}

HueCycler& HueCycler::operator++() {
    index++;
    if (index >= colorCycle.size()) index = 0;
    return *this;
}

// Explicit definition of inc method
HueCycler& HueCycler::inc() {
    index++;
    if (index >= colorCycle.size()) index = 0;
    return *this;
}

ColorHolder HueCycler::hexToRgb(int hex) {
    float red = ((hex >> 16) & 0xFF) / 255.0f;
    float green = ((hex >> 8) & 0xFF) / 255.0f;
    float blue = (hex & 0xFF) / 255.0f;
    return ColorHolder(red, green, blue);
}

void HueCycler::setColor(float red, float green, float blue, float alpha) {
    // Implementation for setting color in your application
    // For demonstration purposes, we can just print the values
    printf("Setting color: R: %f, G: %f, B: %f, A: %f\n", red, green, blue, alpha);
}
