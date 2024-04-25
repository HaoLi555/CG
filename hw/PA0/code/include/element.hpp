#pragma once

#include <image.hpp>
#include <algorithm>
#include <queue>
#include <cstdio>
#include <stack>

class Element
{
public:
    virtual void draw(Image &img) = 0;
    virtual ~Element() = default;
};

class Line : public Element
{

public:
    int xA, yA;
    int xB, yB;
    Vector3f color;
    void draw(Image &img) override
    {
        // TODO: Implement Bresenham Algorithm

        int x, y, dx, dy, e;
        dx = xB - xA;
        dy = yB - yA;

        if (abs(dx) >= abs(dy))
        {
            if (xA <= xB)
            {
                x = xA;
                y = yA;
                dx = xB - xA;
                dy = yB - yA;
            }
            else
            {
                x = xB;
                y = yB;
                dx = xA - xB;
                dy = yA - yB;
            }

            if (dx * dy >= 0)
            {
                e = -dx;
                for (int i = 0; i <= dx; i++)
                {
                    img.SetPixel(x, y, color);
                    x++;
                    e = e + 2 * dy;
                    if (e >= 0)
                    {
                        y++;
                        e = e - 2 * dx;
                    }
                }
            }
            else
            {
                e = dx;
                for (int i = 0; i <= dx; i++)
                {
                    img.SetPixel(x, y, color);
                    x++;
                    e = e + 2 * dy;
                    if (e <= 0)
                    {
                        y--;
                        e = e + 2 * dx;
                    }
                }
            }
        }

        else
        {
            if (yA <= yB)
            {
                x = xA;
                y = yA;
                dx = xB - xA;
                dy = yB - yA;
            }
            else
            {
                x = xB;
                y = yB;
                dx = xA - xB;
                dy = yA - yB;
            }

            if (dx * dy >= 0)
            {
                e = -dy;
                for (int i = 0; i <= dy; i++)
                {
                    img.SetPixel(x, y, color);
                    y++;
                    e = e + 2 * dx;
                    if (e >= 0)
                    {
                        x++;
                        e = e - 2 * dy;
                    }
                }
            }
            else
            {
                e = dy;
                for (int i = 0; i <= dy; i++)
                {
                    img.SetPixel(x, y, color);
                    y++;
                    e = e + 2 * dx;
                    if (e <= 0)
                    {
                        x--;
                        e = e + 2 * dy;
                    }
                }
            }
        }

        printf("Draw a line from (%d, %d) to (%d, %d) using color (%f, %f, %f)\n", xA, yA, xB, yB,
               color.x(), color.y(), color.z());
    }
};

class Circle : public Element
{

public:
    int cx, cy;
    int radius;
    Vector3f color;
    void draw(Image &img) override
    {
        // TODO: Implement Algorithm to draw a Circle

        int x, y, d;
        x = 0;
        y = radius;
        d = 5 - 4 * radius;
        img.SetPixel(cx + x, cy + y, color);
        img.SetPixel(cx + x, cy - y, color);
        img.SetPixel(cx - x, cy + y, color);
        img.SetPixel(cx - x, cy - y, color);
        img.SetPixel(cx + y, cy + x, color);
        img.SetPixel(cx + y, cy - x, color);
        img.SetPixel(cx - y, cy + x, color);
        img.SetPixel(cx - y, cy - x, color);
        while (x <= y)
        {
            if (d < 0)
            {
                d += 8 * x + 12;
            }
            else
            {
                d += 8 * (x - y) + 20;
                y--;
            }
            x++;
            img.SetPixel(cx + x, cy + y, color);
            img.SetPixel(cx + x, cy - y, color);
            img.SetPixel(cx - x, cy + y, color);
            img.SetPixel(cx - x, cy - y, color);
            img.SetPixel(cx + y, cy + x, color);
            img.SetPixel(cx + y, cy - x, color);
            img.SetPixel(cx - y, cy + x, color);
            img.SetPixel(cx - y, cy - x, color);
        }

        printf("Draw a circle with center (%d, %d) and radius %d using color (%f, %f, %f)\n", cx, cy, radius,
               color.x(), color.y(), color.z());
    }
};

class Fill : public Element
{

public:
    int cx, cy;
    Vector3f color;
    void draw(Image &img) override
    {
        // // TODO: Flood fill

        int xl, xr, i;
        int x, y;
        bool needFill;
        struct Seed
        {
            int x;
            int y;
        };

        Seed pt;
        std::stack<Seed> pts;
        pt.x = cx;
        pt.y = cy;
        pts.push(pt);

        Vector3f oldColor = img.GetPixel(cx, cy);
        // printf("x %f, y %f, z %f\n", oldColor.x(), oldColor.y(), oldColor.z());
        if (oldColor==color)
            return;

        while (!pts.empty())
        {
            pt = pts.top();

            x = pt.x;
            y = pt.y;
            pts.pop();

            // printf("init: x %d, y %d\n", x, y);

            while (x < img.Width() && img.GetPixel(x, y) == oldColor)
            {
                img.SetPixel(x, y, color);
                // printf("right set x %d, y %d color\n", x, y);
                // printf("this color, x %f, y %f, z %f\n", img.GetPixel(x, y).x(), img.GetPixel(x, y).y(), img.GetPixel(x, y).z());
                x++;
            }
            xr = x - 1;
            // printf("finish painting right side\n");

            x = pt.x - 1;
            while (x >= 0 && img.GetPixel(x, y) == oldColor)
            {
                img.SetPixel(x, y, color);
                // printf("left: set x %d, y %d color\n", x, y);
                x--;
            }
            xl = x + 1;
            // printf("finish painting left side\n");

            // 上面的线
            x = xl;
            if (y + 1 < img.Height())
            {
                y = y + 1;
                // printf("start line above with x %d, y %d\n", x, y);

                while (x <= xr)
                {

                    needFill = false;
                    while (x < img.Width() && img.GetPixel(x, y) == oldColor)
                    {
                        needFill = true;
                        x++;
                    }
                    if (needFill)
                    {
                        pt.x = x - 1;
                        pt.y = y;
                        // printf("above: push: x %d, y%d\n", pt.x, pt.y);
                        pts.push(pt);
                        needFill = false;
                    }
                    while (x <= xr && img.GetPixel(x, y) != oldColor)
                    {
                        x++;
                    }
                }
                // printf("finish finding new points above\n");
            }
            if (y - 2 >= 0)
            {
                x = xl;
                y = y - 2;
                // printf("start line below with x %d, y %d\n", x, y);
                while (x <= xr)
                {
                    needFill = false;
                    while (x < img.Width() && img.GetPixel(x, y) == oldColor)
                    {
                        needFill = true;
                        x++;
                    }
                    if (needFill)
                    {
                        pt.x = x - 1;
                        pt.y = y;
                        // printf("below: push: x %d, y%d\n", pt.x, pt.y);
                        // printf("this color, x %f, y %f, z %f\n", img.GetPixel(x - 1, y).x(), img.GetPixel(x - 1, y).y(), img.GetPixel(x - 1, y).z());

                        assert(img.GetPixel(pt.x, y) == oldColor);
                        pts.push(pt);
                        needFill = false;
                    }
                    while (x <= xr && img.GetPixel(x, y) != oldColor)
                        x++;
                }
                // printf("finish finding new points below\n");
            }
        }

        printf("Flood fill source point = (%d, %d) using color (%f, %f, %f)\n", cx, cy,
               color.x(), color.y(), color.z());
    }
};