#include "GLCore.h"
#include "PhongLayer.h"

using namespace GLCore;

class PhongApp final : public Application {
public:
    explicit PhongApp(const std::string&obj_file_path = "") : Application("HW3") {
        PushLayer(new PhongLayer(obj_file_path));
    }

};

int main(int argc, char** argv) {
    std::string obj_file_path;
    if (argc > 1) {
        obj_file_path = argv[1];
    }
    const auto app = std::make_unique<PhongApp>(obj_file_path);
    app->Run();
}
