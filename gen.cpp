#include "gen.h"
#include "mainwindow.h"
#include "scan.h"

Gen::Gen()
{
    channel = 0;
    maxChannelTime = 0;
    minChannelTime = 0;
    APs = 0;
}

Gen::~Gen()
{
}

Gen::Gen(int ch, int min, int max, int aps)
{
    channel = ch;
    maxChannelTime = min;
    minChannelTime = max;
    APs = aps;
}

int Gen::getLatency()
{
    return minChannelTime+maxChannelTime;
}

double Gen::getValue(int index)
{
    if (index ==0)
        return channel;
    else if (index == 1)
        return minChannelTime;
    else if (index == 2)
        return maxChannelTime;
    else
        return APs;
}

void Gen::setChannel(int c)
{
    channel = c;
}

void Gen::setMinChannelTime(double min)
{
    minChannelTime = min;
}

void Gen::setMaxChannelTime(double max)
{
    maxChannelTime = max;
}

void Gen::setAPs(double aps)
{
    APs = aps;
}


double Gen::getFONC()
{

    // verificar el parametro indiceParaOrdenarTablaC que especifica el indice a utilizar
    int index = MainWindow::getIndexToSortCTable();

    QString database("test_18.1.db");

    // tipo de experimento para extraer las muestras: full -> full scanning
    QString experiment("full");

    //Scan scan(database.toStdString(),experiment.toStdString());
    ScanningCampaing scan(database.toStdString(),experiment.toStdString(), 0);
    scan.init();
    scan.prepareIRD();

    // sumatorio de APs en min
    double minAPsum = 0;

    // APs promedio encontrados con min
    double APmin = 0;

    // sumatorio de APs en min
    double maxAPsum = 0;

    // APs promedio encontrados con min
    double APmax = 0;

    // valor del indice para el gen
    double fonc = 0;

    for (int i=0; i<30; i++)
    {
        // (ch, min, 0) corresponde a los APs encontrados con minchanneltime
        minAPsum = minAPsum + scan.getAPs(channel, minChannelTime, 0);
    }
    APmin = minAPsum/30;

    for (int i=0; i<30; i++)
    {
        // (ch, min, max) corresponde a los APs encontrados con maxchanneltime
        maxAPsum = maxAPsum + scan.getAPs(channel, minChannelTime, maxChannelTime);
    }
    APmax = maxAPsum/30;

    if (index == 0)
    {
        // si maxChannelTime es cero no se suman los aps encontrados con max
        if (maxChannelTime ==0)
        {
            fonc = (APmin/minChannelTime);
        }
        else
        {
            fonc = (APmin/minChannelTime)*0.2 + (std::abs(APmax-APmin)/maxChannelTime)*0.8;
        }
    }
    else if (index == 1)
    {
        // si maxChannelTime es cero no se suman los aps encontrados con max
        if (maxChannelTime ==0)
        {
            fonc = (APmin/minChannelTime);
        }
        else
        {
            fonc = (APmin/minChannelTime)*0.4 + (std::abs(APmax-APmin)/maxChannelTime)*0.6;
        }
    }
    else // index == 2
    {
        // si maxChannelTime es cero no se suman los aps encontrados con max
        if (maxChannelTime ==0)
        {
            fonc = (APmin/minChannelTime);
        }
        else
        {
            //fonc = (APmin/minChannelTime)*0.6 + (std::abs(APmax-APmin)/maxChannelTime)*0.4;

            // para prueba de simulation200gFONC 0.7 y 0.3
            fonc = (APmin/minChannelTime)*0.7 + (std::abs(APmax-APmin)/maxChannelTime)*0.3;
        }
    }
    return fonc;
}
