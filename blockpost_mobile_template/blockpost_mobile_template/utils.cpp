#include "utils.hpp"

ImVec2 operator-( ImVec2 left, ImVec2 right )
{
    return ImVec2( left.x - right.x, left.y - right.y );
}
ImVec2 operator+( ImVec2 left, ImVec2 right )
{
    return ImVec2( left.x + right.x, left.y + right.y );
}
ImVec2 operator/( ImVec2 left, ImVec2 right )
{
    return ImVec2( left.x / right.x, left.y / right.y );
}
ImVec2 operator*( ImVec2 left, ImVec2 right )
{
    return ImVec2( left.x * right.x, left.y * right.y );
}
ImVec2 operator/( ImVec2 left, int right )
{
    return ImVec2( left.x / right, left.y / right );
}
ImVec2 operator*( ImVec2 left, int right )
{
    return ImVec2( left.x * right, left.y * right );
}