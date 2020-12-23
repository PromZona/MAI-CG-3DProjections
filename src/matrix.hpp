#pragma once

namespace Application
{
    namespace Matrix
    {
        raylib::Vector3 Multiply3x2(float matrix[2][3], raylib::Vector3 &a)
        {
            float x = (matrix[0][0] * a.x) + (matrix[0][1] * a.y) + (matrix[0][2] * a.z);
            float y = (matrix[1][0] * a.x) + (matrix[1][1] * a.y) + (matrix[1][2] * a.z);
            return raylib::Vector3(x, y, 0);
        }

        raylib::Vector3 Multiply3x3(float matrix[3][3], raylib::Vector3 &a)
        {
            float x = (matrix[0][0] * a.x) + (matrix[0][1] * a.y) + (matrix[0][2] * a.z);
            float y = (matrix[1][0] * a.x) + (matrix[1][1] * a.y) + (matrix[1][2] * a.z);
            float z = (matrix[2][0] * a.x) + (matrix[2][1] * a.y) + (matrix[2][2] * a.z);
            return raylib::Vector3(x, y, z);
        }

        raylib::Vector3 Multiply4x4(float matrix[4][4], raylib::Vector3 &vec)
        {
            float x = vec.x * matrix[0][0] + vec.y * matrix[1][0] + vec.z * matrix[2][0] + matrix[3][0];
            float y = vec.x * matrix[0][1] + vec.y * matrix[1][1] + vec.z * matrix[2][1] + matrix[3][1];
            float z = vec.x * matrix[0][2] + vec.y * matrix[1][2] + vec.z * matrix[2][2] + matrix[3][2];
            float w = vec.x * matrix[0][3] + vec.y * matrix[1][3] + vec.z * matrix[2][3] + matrix[3][3];
            
            if (w != 0)
            {
                x /= w; y /= w; z /= w; 
            }
            return (raylib::Vector3){x, y, z};
        }
        
    } // namespace Matrix

} // namespace Application

