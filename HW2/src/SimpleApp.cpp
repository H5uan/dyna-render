#include "GLCore.h"
#include "SimpleLayer.h"

using namespace GLCore;

class SimpleApp final : public Application {
public:
    SimpleApp() : Application("HW1") {
        PushLayer(new SimpleLayer());
    }
};

int main() {
    const auto app = std::make_unique<SimpleApp>();
    app->Run();
}
