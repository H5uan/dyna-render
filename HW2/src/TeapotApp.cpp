#include "GLCore.h"
#include "TeapotLayer.h"

using namespace GLCore;

class TeapotApp final : public Application {
public:
    explicit TeapotApp(const std::string&obj_file_path = "") : Application("HW2") {
        PushLayer(new TeapotLayer(obj_file_path));
    }

};

int main(int argc, char** argv) {
    std::string obj_file_path;
    if (argc > 1) {
        obj_file_path = argv[1];
    }
    const auto app = std::make_unique<TeapotApp>(obj_file_path);
    app->Run();
}
