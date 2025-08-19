#include "can_sim.hpp"
#include "controller.hpp"
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

// usage: ./eae_firmware --temp 45 --run-seconds 20
int main(int argc, char** argv) {
    double setpoint = 40.0;
    int run_seconds = 30;

    // simple command line parsing
    for(int i=1;i<argc;i++){
        if(strcmp(argv[i],"--temp")==0 && i+1<argc) {
            setpoint = atof(argv[++i]);
        } else if(strcmp(argv[i],"--run-seconds")==0 && i+1<argc) {
            run_seconds = atoi(argv[++i]);
        } else if(strcmp(argv[i],"--help")==0) {
            std::cout << "Usage: " << argv[0] << " [--temp <C>] [--run-seconds <s>]\n";
            return 0;
        }
    }

    CANBusSim bus;
    CoolingController controller(bus);
    controller.set_temperature_setpoint(setpoint);

    // simulate ignition on
    controller.set_ignition(true);

    controller.start();

    // Run for specified seconds
    std::this_thread::sleep_for(std::chrono::seconds(run_seconds));

    controller.stop();
    return 0;
}
