#ifndef HUECYCLER_H
#define HUECYCLER_H

#include <vector>
#include <stdexcept>
#include <cstdio>

class ColorHolder {
public:
    float r, g, b, a;

    ColorHolder();
    ColorHolder(float red, float green, float blue, float alpha = 1.0f);
    int toRGBInt() const;
};

class HueCycler {
private:
    float hueMultiplier;
    std::vector<int> colorCycle;  // Assuming a max of 256 cycles for simplicity
    int index;

    static int HSBtoRGB(float hue, float saturation, float brightness);

public:
    HueCycler(int cycles);
    void reset();
    void set(int indexIn);
    ColorHolder setCurrent();
    void setNext();
    void setNext(float alpha);
    int currentInt() const;
    ColorHolder currentRgba(int alpha);
    ColorHolder currentRgb();
    void operator+=(int plus);
    HueCycler& operator++();
    HueCycler& inc(); // Explicitly declare inc method

private:
    ColorHolder hexToRgb(int hex);
    void setColor(float red, float green, float blue, float alpha);
};

#endif // HUECYCLER_H
