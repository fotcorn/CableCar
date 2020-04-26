#pragma once

#include <stdexcept>
#include <string>
#include <sstream>

struct Color {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char alpha;

	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char alpha) : r(r), g(g), b(b), alpha(alpha) {}
	Color(unsigned char r, unsigned char g, unsigned char b, float alpha) : r(r), g(g), b(b) {
		if (alpha < 0.0f || alpha > 1.0f) {
			throw std::runtime_error("Color: Invalid alpha value");
		}
		alpha = static_cast<unsigned char>(255.0 * alpha);
	}
	Color(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b), alpha(255) {}
	Color(std::string hexColor) {
		// strtoul also accepts strings starting with 0x or 0X, we do not want that
		if (hexColor.size() >= 2 && (hexColor.substr(0, 2) == std::string("0x") || hexColor.substr(0, 2) == std::string("0X"))) {
			throw std::runtime_error(std::string("Color: Invalid html value, must not start with 0x or 0X") + hexColor);
		}

		unsigned long v;
		try {
			v = std::stoul(hexColor, 0, 16);
		} 
		catch (std::logic_error) {
			throw std::runtime_error(std::string("Color: Invalid html value: ") + hexColor);
		}

		std::cout << v << std::endl;

		if (hexColor.size() == 6) {
			r = static_cast<unsigned char>((v & 0xFF0000) >> 16);
			g = static_cast<unsigned char>((v & 0x00FF00) >> 8);
			b = static_cast<unsigned char>(v & 0x0000FF);
			alpha = 255;
		}
		// with alpha value
		else if (hexColor.size() == 8) {
			r = static_cast<unsigned char>((v & 0xFF000000) >> 24);
			g = static_cast<unsigned char>((v & 0x00FF0000) >> 16);
			b = static_cast<unsigned char>((v & 0x0000FF00) >> 8);
			alpha = static_cast<unsigned char>((v & 0x000000FF));
		}
		else {
			throw std::runtime_error(std::string("Color: Invalid html value, must be 6 or 8 chars long:") + hexColor);
		}
	}

	std::string html() const {
		std::stringstream ss;
		ss << "Color(#" << std::hex << +r << +g << +b << +alpha << ")";
		return ss.str();
	}

	std::string string() const {
		std::stringstream ss;
		ss << "Color(" << +r << ", " << +g << ", " << +b << ", " << +alpha << ")";
		return ss.str();
	}
};

std::ostream &operator<<(std::ostream &os, const Color& color) {
	return os << color.string();
}
