#include "GLCore.h"
#include "EditorLayer.h"

using namespace GLCore;

class App final : public Application {
public:
    explicit App(const std::string&normalmap_file_path = "",
                 const std::string&displacement_file_path = "") : Application("Dyna-Render") {
        PushLayer(new EditorLayer(normalmap_file_path));
    }
};

int main(int argc, char** argv) {
    std::string normalmap_file_path = "../../../Assets/Texture/teapot_normal.png";
    std::string displacement_file_path;
    if (argc > 1) {
        normalmap_file_path = argv[1];
    }
    if (argc > 2) {
        displacement_file_path = argv[2];
    }
    else {
        displacement_file_path = "";
    }
    const auto app = std::make_unique<App>(normalmap_file_path, displacement_file_path);
    app->Run();
}
