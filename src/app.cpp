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

        if (IsKeyDown(KEY_UP))
        {
            distance += 0.01;
        }

        if (IsKeyDown(KEY_DOWN))
        {
            distance -= 0.01;
        }

        if (IsKeyReleased(KEY_O))
        {
            isAllEdges = !isAllEdges;
        }

        if (IsKeyReleased(KEY_P))
        {
            PerspectiveMode = !PerspectiveMode;
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
            pol.DrawSurface(points, shift);
            shift += 40;
        }

        for (auto &&pol : polygons)
        {
            pol.DrawEdges(points, isAllEdges);
        }
        
        // Control tips
        DrawText("Arrows LR - rotate. Arrow UD - change distance. O - toogle edges. P - Change perspective", 10, Height - 20, 18, raylib::Color::White);
        
        // State info
        DrawText(TextFormat("Distance: %03.1f", distance), Width - 300, 50, 20, raylib::Color::White);
        DrawText(TextFormat("Angle: %03.1f", angle), Width - 300, 80, 20, raylib::Color::White);
        if (PerspectiveMode)
        {
            DrawText("Show mode: Perspective", Width - 300, 110, 20, raylib::Color::Green);
        }
        else
        {
            DrawText("Show mode: Orthographic", Width - 300, 110, 20, raylib::Color::Yellow);
        }

        EndDrawing();
    }

    void App::UpdatePoints()
    {
        points.clear();

        int Radius = 1;
        if (!PerspectiveMode)
            Radius *= 100;

        float distance = 0;
        points.push_back((Vector3){0, 0, distance}); // Center of pentagon

        // Pentagon
        for (int i = 0; i < 360; i += 72)
        {
            float x = Radius * cos(i * PI / 180);
            float z = Radius * sin(i * PI / 180);

            points.push_back((Vector3){x, 0, distance + z});
        }

        float pentagon_side = Radius * 1.17557f;
        float pentagon_height = tan(72 * PI / 180) / 2 * pentagon_side;
        points.push_back((Vector3){0, pentagon_height, distance}); // Top of pentagon

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

        float fNear = 0.1f;
        float fFar = 1000.0f;
        float fFov = 90.0f;
        float fAspectRatio = (float)Height / (float)Width;
        float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * PI);
        float PerspectiveProjection[4][4] = { 0 };
        PerspectiveProjection[0][0] = fAspectRatio * fFovRad;
        PerspectiveProjection[1][1] = fFovRad;
        PerspectiveProjection[2][2] = fFar / (fFar / fNear);
        PerspectiveProjection[3][2] = (-fFar * fNear) / (fFar - fNear);
        PerspectiveProjection[2][3] = 1.0f;
        PerspectiveProjection[3][3] = 0.0f;

        for (auto &&point : points)
        {
            // Ortho projection
            if (!PerspectiveMode)
            {
                point = Application::Matrix::Multiply3x3(OrthoProjection, point);
                point.x += Width / 2;
                point.y += Height / 2; 
            }

            // Perspective projection
            if (PerspectiveMode)
            {
                point.z += this->distance;
                point = Application::Matrix::Multiply4x4(PerspectiveProjection, point);
                point.x += 1.0f; point.y += 1.0f;
                point.x *= 0.5f * (float)Width;
                point.y *= 0.5f * (float)Height;    
            }        

            DrawCircle(point.x, point.y, 2, raylib::Color::Red);
        }
    }

} // namespace Application
