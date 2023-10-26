#include <pch.h>
#include <engine/rendering/shapes/box.h>

#include <maths/types/vertex.h>

using namespace PrioEngine;
using namespace DirectX;
using namespace PrioEngine::Maths;

namespace PrioEngine
{
    CBox::CBox()
    {
        m_Vertices = {
        Vertex(Vector3{-1.0f, -1.0f, -1.0f}, Colour{Colors::White}),
        Vertex(Vector3{-1.0f, +1.0f, -1.0f }, Colour{ Colors::Black }),
        Vertex(Vector3(+1.0f, +1.0f, -1.0f), Colour(Colors::Red)),
        Vertex(Vector3(+1.0f, -1.0f, -1.0f), Colour(Colors::Green)),
        Vertex(Vector3(-1.0f, -1.0f, +1.0f), Colour(Colors::Blue)),
        Vertex(Vector3(-1.0f, +1.0f, +1.0f), Colour(Colors::Yellow)),
        Vertex(Vector3(+1.0f, +1.0f, +1.0f), Colour(Colors::Cyan)),
        Vertex(Vector3(+1.0f, -1.0f, +1.0f), Colour(Colors::Magenta))
        };

        m_Indices = {
			// front face
            0, 1, 2,
            0, 2, 3,

            // back face
            4, 6, 5,
            4, 7, 6,

            // left face
            4, 5, 1,
            4, 1, 0,

            // right face
            3, 2, 6,
            3, 6, 7,

            // top face
            1, 5, 6,
            1, 6, 2,

            // bottom face
            4, 0, 3,
            4, 3, 7
        };

        // TODO:
        // Up to the "Build Geometry" method.
    }
}