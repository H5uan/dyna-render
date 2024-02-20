#include "GLCore.h"
#include "FrameBufferLayer.h"

using namespace GLCore;

class BlinnPhongApp final : public Application {
public:
    explicit BlinnPhongApp(const std::string&obj_file_path = "") : Application("HW4") {
        PushLayer(new FrameBufferLayer(obj_file_path));
    }
};

int main(int argc, char** argv) {
    std::string obj_file_path = "../../../HW5/assets/models/teapot.obj";
    if (argc > 1) {
        obj_file_path = argv[1];
    }
    const auto app = std::make_unique<BlinnPhongApp>(obj_file_path);
    app->Run();
}
