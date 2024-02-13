#include "GLCore.h"
#include "TextureLayer.h"

using namespace GLCore;

class BlinnPhongApp final : public Application {
public:
    explicit BlinnPhongApp(const std::string&obj_file_path = "") : Application("HW4") {
        PushLayer(new TextureLayer(obj_file_path));
    }
};

int main(int argc, char** argv) {
    std::string obj_file_path = "../../../HW4/assets/models/teapot.obj";
    if (argc > 1) {
        obj_file_path = argv[1];
    }
    const auto app = std::make_unique<BlinnPhongApp>(obj_file_path);
    app->Run();
}
