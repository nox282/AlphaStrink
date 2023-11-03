#include "simpleTests.h"
#include <Core/World.h>
#include <OpenGL/Shader/ShaderUtils.h>
#include <filesystem>
#include <Core/Application.h>
#include <Utils/StringUtils.h>

using namespace ECSEngine;

ST_SECTION_BEGIN(OpenGL, "OpenGL")
{
    ST_TEST(ParseShaderFile, "Should parse a shader file")
    {
        Application app;
        auto path = std::filesystem::path(std::filesystem::current_path().string() + "/OpenGL/shader.glsl");
        std::string fileName, vertexSource, fragmentSource;
        const bool bResult = parseShaderSourceFileFromPath(path.string(), fileName, vertexSource, fragmentSource);

        ST_ASSERT(bResult, "Should have succeeded");
        ST_ASSERT(fileName == "shader.glsl", "Filename should be equal");

        // Expected vertex and fragment shader sources
        const std::string expectedVertexSource = removeWhiteSpace("#version 330 corelayout(location = 0) in vec3 aPos;void main(){gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);}");
        const std::string expectedFragmentSource = removeWhiteSpace("#version 330 coreout vec4 FragColor;void main(){FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);}");

        ST_ASSERT(removeWhiteSpace(vertexSource) == expectedVertexSource, "Vertex source should match expected");
        ST_ASSERT(removeWhiteSpace(fragmentSource) == expectedFragmentSource, "Fragment source should match expected");
    }
}
ST_SECTION_END(OpenGL)
