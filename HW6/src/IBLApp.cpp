#include "GLCore.h"
#include "IBLLayer.h"

using namespace GLCore;

class IBLApp final : public Application {
public:
    explicit IBLApp(const std::string&obj_file_path = "") : Application("HW4") {
        PushLayer(new IBLLayer(obj_file_path));
    }
};

int main(int argc, char** argv) {
    std::string obj_file_path = "../../../HW6/assets/models/sphere.obj";
    if (argc > 1) {
        obj_file_path = argv[1];
    }
    const auto app = std::make_unique<IBLApp>(obj_file_path);
    app->Run();
}
