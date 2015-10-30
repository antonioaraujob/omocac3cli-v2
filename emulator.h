#ifndef EMULATOR_H
#define EMULATOR_H


#include <QString>
#include "scan.h"

/**
 * @brief Clase que abstrae el emulador de la topologia
 */
class Emulator
{
private:

    ScanningCampaing * scanningCampaing;

public:
    /**
     * @brief Constructor de la clase
     */
    Emulator();

    /**
     * @brief Destructor de la clase
     */
    ~Emulator();

    /**
     * @brief Retorna el numero de APs al consultar la topologia
     * @param channel canal
     * @param min minchanneltime
     * @param max maxchanneltime
     * @return
     */
    double getAPs(int channel, double min, double max);

};

#endif // EMULATOR_H
