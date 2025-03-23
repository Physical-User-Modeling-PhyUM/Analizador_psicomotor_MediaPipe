#include <iostream>
#include "../../src/pose/posemanager.h"

class TestPoseFunctional {
public:
    void runTests() {
        std::cout << "Ejecutando prueba funcional del sistema..." << std::endl;

        PoseManager poseManager(nullptr, false);
        poseManager.init();

        std::cout << "TestPoseFunctional PASSED" << std::endl;
    }
};

