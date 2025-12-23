# ==============================================
# 🚀 Rams Ultra Calculator & Unit Converter
# ==============================================
cmake_minimum_required(VERSION 3.10)
project(rams_ultra_calculator_and_unit_converter_portable)

# ==============================================
# ⚙️ Build Configuration
# ==============================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ==============================================
# 📦 Thirdparty Dependencies
# ==============================================
add_subdirectory(thirdparty/glfw-3.4)

include_directories(
    include
    thirdparty/imgui
    thirdparty/imgui/backends
    thirdparty/glfw-3.4/include
)

# ==============================================
# 🎨 ImGui Source Files
# ==============================================
set(IMGUI_SRC
    thirdparty/imgui/imgui.cpp
    thirdparty/imgui/imgui_demo.cpp
    thirdparty/imgui/imgui_draw.cpp
    thirdparty/imgui/imgui_tables.cpp
    thirdparty/imgui/imgui_widgets.cpp
    thirdparty/imgui/backends/imgui_impl_glfw.cpp
    thirdparty/imgui/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC ${IMGUI_SRC})

# ==============================================
# 🧠 Core Source Files
# ==============================================
file(GLOB SRC
    src/*.cpp
    src/engine/*.cpp
    src/themes/*.cpp
)

# ==============================================
# 🏗️ Main Executable
# ==============================================
add_executable(${PROJECT_NAME} ${SRC})

# ==============================================
# 🔗 Link Libraries (GLFW, ImGui, OpenGL)
# ==============================================
target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    imgui
    opengl32
)

# ==========================================
# ✅ Manual cURL Integration (supports DLL)
# ==========================================
include_directories("D:/curl_nospaces/include")
link_directories("D:/curl_nospaces/lib")

add_definitions(-DCURL_STATICLIB)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    imgui
    opengl32
    "D:/curl_nospaces/lib/libcurl.dll.a"
    "D:/curl_nospaces/lib/libssl.a"
    "D:/curl_nospaces/lib/libcrypto.a"
    "D:/curl_nospaces/lib/libz.a"
    "D:/curl_nospaces/lib/libzstd.a"
    "D:/curl_nospaces/lib/libssh2.a"
)



# ==============================================
# 📘 Optional: Doxygen Documentation (if installed)
# ==============================================
find_package(Doxygen)
if(DOXYGEN_FOUND)
    doxygen_add_docs(rams_docs include src)
endif()
