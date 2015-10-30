#include "emulator.h"


Emulator::Emulator()
{

    // base de datos sqlite
    QString database("test_18.1.db");
    //QString database("database.db");

    // tipo de experimento para extraer las muestras: full -> full scanning
    QString experiment("full");
    scanningCampaing = new ScanningCampaing(database.toStdString(),experiment.toStdString(), 0);
    scanningCampaing->init();
    scanningCampaing->prepareIRD();

}

Emulator::~Emulator()
{

}


double Emulator::getAPs(int channel, double min, double max)
{
    double aps = scanningCampaing->getAPs(channel, min, max);
    return aps;
}
