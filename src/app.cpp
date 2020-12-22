#include "app.hpp"
#include "matrix.hpp"

namespace Application
{

    App::App(int w, int h) : Width(w), Height(h), window(w, h, "3D Projections. Essaulov Andrew")
    {
        window.SetTargetFPS(60);

        UpdatePoints();
    }

    App::~App()
    {
    }

    void App::Start()
    {
        while (!window.ShouldClose())
        {
            Input();
            Update();
            Render();
        }
    }

    void App::Input()
    {
        if (IsKeyDown(KEY_RIGHT))
        {
            angle += 0.01;
        }

        if (IsKeyDown(KEY_LEFT))
        {
            angle -= 0.01;
        }

        if (IsKeyReleased(KEY_O))
        {
            isAllEdges = !isAllEdges;
        }
    }

    void App::Update()
    {
        Width = window.GetScreenWidth();
        Height = window.GetScreenHeight();

        UpdatePoints();
        Rotate();
        UpdatePolygons();
        Projection();
    }

    void App::Render()
    {

        BeginDrawing();

        ClearBackground(BLACK);

        int shift = 10;

        for (auto &&pol : polygons)
        {
            pol.Draw(points, shift);
            
            shift += 40;
        }

        for (auto &&pol : polygons)
        {
            pol.DrawEdge(points, isAllEdges);
        }
        
        EndDrawing();
    }

    void App::UpdatePoints()
    {
        points.clear();

        int Radius = 100;
        points.push_back((Vector3){0, 0, 0}); // Center of pentagon

        // Pentagon
        for (int i = 0; i < 360; i += 72)
        {
            float x = Radius * cos(i * PI / 180);
            float z = Radius * sin(i * PI / 180);

            points.push_back((Vector3){x, 0, z});
        }

        float pentagon_side = Radius * 1.17557f;
        float pentagon_height = tan(72 * PI / 180) / 2 * pentagon_side;
        points.push_back((Vector3){0, pentagon_height, 0}); // Top of pentagon

        for (auto &&p : points)
        {
            p.z += 1;
        }
        
    }

    void App::Rotate()
    {
        float RotationZ[3][3] = {
            {cos(angle), -sin(angle), 0},
            {sin(angle), cos(angle), 0},
            {0, 0, 1}};

        float RotationX[3][3] = {
            {1, 0, 0},
            {0, cos(angle), -sin(angle)},
            {0, sin(angle), cos(angle)}};

        float RotationY[3][3] = {
            {cos(angle), 0, sin(angle)},
            {0, 1, 0},
            {-sin(angle), 0, cos(angle)}};

        for (auto &&point : points)
        {
            point = Application::Matrix::Multiply3x3(RotationZ, point);
            point = Application::Matrix::Multiply3x3(RotationX, point);
            // point = Application::Matrix::Multiply3x3(RotationY, point);
            // point = Application::Matrix::Multiply3x2(OrthoProjection, point);
        }
    }

    void App::UpdatePolygons()
    {
        polygons.clear();

        std::vector<raylib::Color> colors = {
            raylib::Color::White,
            raylib::Color::Red,
            raylib::Color::White,
            raylib::Color::White,
            raylib::Color::White,
            raylib::Color::White,
            raylib::Color::White,
            raylib::Color::White,
            raylib::Color::White,
            raylib::Color::White
        };
        int count = 0; // need to count id of polygons
        int j = 0;
        for (int i = 1; i < 6; i++)
        {
            if (i != 5)
            {
                polygons.push_back(Polygon(points.size() - 1, i, i + 1, count++, colors[j++]));
            }
            else
            {
                polygons.push_back(Polygon(points.size() - 1, i, 1, count++, colors[j++]));
            }
        }

        for (int i = 5; i > 0; i--)
        {
            if (i != 1)
            {
                polygons.push_back(Polygon(0, i, i - 1, count++, raylib::Color::Yellow));
            }
            else
            {
                polygons.push_back(Polygon(0, i, 5, count++, raylib::Color::Yellow));
            }
        }

        for (auto &&p : polygons)
        {
            p.GetCenter(points);
        }

        /*
        std::sort(polygons.begin(), polygons.end(), [&](Polygon pol1, Polygon pol2) {
            return (pol1.depth < pol2.depth);
        });
        */
        return;
    }

    void App::Projection()
    {
        float OrthoProjection[2][3] = {
            {1, 0, 0},
            {0, 1, 0}
        };

        for (auto &&point : points)
        {
            float distance = 1;
            float z = 1 / (distance - point.z);
            float PerspectiveProjection[2][3] = {
                {z, 0, 0},
                {0, z, 0}
            };

            point = Application::Matrix::Multiply3x3(PerspectiveProjection, point);
            point.x += Width / 2;
            point.y += Height / 2;
            DrawCircle(point.x, point.y, 2, raylib::Color::Red);
        }
    }

} // namespace Application
