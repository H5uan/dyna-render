#include "GLCore.h"
#include "EditorLayer.h"

using namespace GLCore;

class App final : public Application {
public:
    explicit App(const std::string&obj_file_path = "") : Application("Dyna-Render") {
        PushLayer(new EditorLayer(obj_file_path));
    }
};

int main(int argc, char** argv) {
    std::string obj_file_path = "../../../Assets/Models/teapot.obj";
    if (argc > 1) {
        obj_file_path = argv[1];
    }
    const auto app = std::make_unique<App>(obj_file_path);
    app->Run();
}
