#pragma once
#include "headers.hpp"
#include <vector>
#include <algorithm>
#include <math.h>

namespace Application
{
    class Polygon
    {
    private:
        int indecies[3];
        int id;
        raylib::Color color;
    public:
        float depth;
        Polygon(int i1, int i2, int i3, int ID, raylib::Color c)
        {
            indecies[0] = i1;
            indecies[1] = i2;
            indecies[2] = i3;
            id = ID;
            depth = 0;
            color = c;
        }

        raylib::Vector3 GetCenter(std::vector<raylib::Vector3> &vec)
        {
            float x = (vec[indecies[0]].x + vec[indecies[1]].x + vec[indecies[2]].x) / 3;
            float y = (vec[indecies[0]].y + vec[indecies[1]].y + vec[indecies[2]].y) / 3;
            float z = (vec[indecies[0]].z + vec[indecies[1]].z + vec[indecies[2]].z) / 3;
            depth = z;
            return raylib::Vector3(x, y, z);
        }

        void DrawSurface(std::vector<raylib::Vector3> &vec, int shift)
        {
            std::vector<raylib::Vector3> buf = {vec[indecies[0]], vec[indecies[1]], vec[indecies[2]]};
            
            raylib::Vector3 first, second, third;
            first = buf[0];
            second = buf[1];
            third = buf[2];

            raylib::Vector3 line1, line2, normal;
            line1 = (raylib::Vector3){second.x - first.x, second.y - first.y, second.z - first.z};
            line2 = (raylib::Vector3){third.x - first.x, third.y - first.y, third.z - first.z};

            normal.x = line1.y * line2.z - line1.z * line2.y;
            normal.y = line1.z * line2.x - line1.x * line2.z;
            normal.z = line1.x * line2.y - line1.y * line2.x;
            float l = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
            normal.x /= l; normal.y /= l; normal.z /= l;

            // Debug info
            // DrawText(TextFormat("%02i f: (%03.0f, %03.0f, %03.0f) s: (%03.0f, %03.0f, %03.0f) t: (%03.0f, %03.0f, %03.0f)", 
            // id, first.x, first.y, first.z, second.x, second.y, second.z, third.x, third.y, third.z),
            // 25, shift, 18, color);

            DrawTriangle((raylib::Vector2){first.x, first.y},
                         (raylib::Vector2){second.x, second.y},
                         (raylib::Vector2){third.x, third.y},
                         color);
        }
    
        void DrawEdges(std::vector<raylib::Vector3> &vec, bool all)
        {
            std::vector<raylib::Vector3> buf = {vec[indecies[0]], vec[indecies[1]], vec[indecies[2]]};
            
            raylib::Vector3 first, second, third;
            first = buf[0];
            second = buf[1];
            third = buf[2];

            if (all)
            {
                DrawLine(first.x, first.y, second.x, second.y, raylib::Color::Red);
                DrawLine(second.x, second.y, third.x, third.y, raylib::Color::Red);
                DrawLine(third.x, third.y, first.x, first.y, raylib::Color::Red);
                return;
            }

            raylib::Vector3 line1, line2, normal;
            line1 = (raylib::Vector3){second.x - first.x, second.y - first.y, second.z - first.z};
            line2 = (raylib::Vector3){third.x - first.x, third.y - first.y, third.z - first.z};

            normal.x = line1.y * line2.z - line1.z * line2.y;
            normal.y = line1.z * line2.x - line1.x * line2.z;
            normal.z = line1.x * line2.y - line1.y * line2.x;
            float l = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
            normal.x /= l; normal.y /= l; normal.z /= l;

            if (normal.z < 0)
            {
                DrawLine(first.x, first.y, second.x, second.y, raylib::Color::Red);
                DrawLine(second.x, second.y, third.x, third.y, raylib::Color::Red);
                DrawLine(third.x, third.y, first.x, first.y, raylib::Color::Red);
            }
        }
    };

    class App
    {
    private:
        int Width;
        int Height;

        raylib::Window window;

        std::vector<raylib::Vector3> points;
        std::vector<Polygon> polygons;

        float angle = 0;
        float distance = 5;
        bool isAllEdges = true;
        bool PerspectiveMode = false;

    public:
        App(int w, int h);
        ~App();

        void Start();
        void Input();
        void Update();
        void Render();

        void UpdatePoints();
        void Rotate();
        void UpdatePolygons();
        void Projection();
    };
} // namespace Application
