#pragma once


class ViewportArea {
public:
    int x;
    int y;
    int width;
    int height;

public:
    ViewportArea() : x(0), y(0), width(1), height(1) {}
    ViewportArea(int x, int y, int width, int height)
        : x(x), y(y), width(width), height(height) {}

};

