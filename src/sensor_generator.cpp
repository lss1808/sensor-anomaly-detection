#include <random>
#include <chrono>

double generateSensorValue() {
    static std::default_random_engine generator(
        std::chrono::system_clock::now().time_since_epoch().count()
    );

    static std::normal_distribution<double> noise(0.0, 1.5);
    static int counter = 0;

    counter++;

    // Valor nominal do sensor
    double value = 100.0 + noise(generator);

    // Pico rápido a cada 50 ciclos
    if (counter % 50 == 0) {
        value = 200.0;
    }

    return value;
}

