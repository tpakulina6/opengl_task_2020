set(SRC_FILES
    Main.h
    Main.cpp
    common/Application.hpp
    common/Application.cpp
    common/Camera.cpp
    common/Camera.hpp
    common/Common.h
    common/Mesh.hpp
    common/Mesh.cpp
    common/ShaderProgram.cpp
    common/ShaderProgram.hpp
    common/DebugOutput.h
    common/DebugOutput.cpp
    common/Texture.hpp
    common/Texture.cpp
    common/LightInfo.hpp
)
set(SHADER_FILES
        692PakulinaData2/shader.frag
        692PakulinaData2/shader.vert
        692PakulinaData2/marker.frag
        692PakulinaData2/marker.vert
)

set(TXT_FILES
        692PakulinaData2/coordinates.txt
)

source_group("Shaders" FILES ${SHADER_FILES}) 
source_group("TXT" FILES ${TXT_FILES})

include_directories(common)

MAKE_OPENGL_TASK(692Pakulina 2 "${SRC_FILES}")

if (UNIX)
    target_link_libraries(692Pakulina2 stdc++fs)
endif()


