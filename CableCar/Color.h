#pragma once

#include <string>

struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char alpha;

    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char alpha);
    Color(unsigned char r, unsigned char g, unsigned char b, float alpha);
    Color(unsigned char r, unsigned char g, unsigned char b);
    Color(std::string hexColor);

    std::string html() const;

    std::string string() const;
};

std::ostream& operator<<(std::ostream& os, const Color& color);
