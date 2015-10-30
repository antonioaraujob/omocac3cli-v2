#ifndef MUTATION_H
#define MUTATION_H

#include <QList>
#include <QHash>

#include "individual.h"
#include "normativegrid.h"
#include "ctable.h"

#include "scan.h"

/**
 * @brief Clase que modela la variacion de la poblacion para cada generacion
 */
class Mutation
{

private:
    /**
     * @brief Lista que mantiene la poblacion original mas los hijos generados
     */
    QList<Individual *> newPopulation;

    /**
     * @brief Diccionario para mantener registro de los canales que ya se han utilizado
     * en un proceso de mutacion.
     *
     * Se utiliza para evitar que en la mutacion de parametros de un individuo se asignen
     * canales que ya se utilizaron previamente
     */
    QHash<int, bool> channelsUsedForMutation;

    /**
     * @brief Desviacion estandar para la mutacion
     */
    double stdDeviation;

    /**
     * @brief Objeto que abstrae la campana de scanning
     */
    //ScanningCampaing * scanningCampaing;
public:

    /**
     * @brief Constructor de la clase Mutation
     */
    Mutation();

    /**
     * @brief Destructor de la clase Mutation
     */
    ~Mutation();

    /**
     * @brief Ejecuta el proceso de mutacion sobre la poblacion pasada como argumento.
     *
     * La nueva poblacion de tamano 2P se almacenara en @see newPopulation
     *
     * @param population poblacion sobre la cual se ejecuta el proceso de mutacion
     * @param std desviacion estandar de la variable aleatoria normal
     * @param std desviacion estandar de la variable aleatoria normal MinChannelTime
     * @param std desviacion estandar de la variable aleatoria normal MaxChannelTime
     */
    void doMutation(QList<Individual *> population, double std, double stdMin, double stdMax);

    /**
     * @brief Ejecuta el proceso de mutacion dirigida sobre la base del conocimiento normativo
     * del espacio de creencias.
     *
     * @param population poblacion sobre la cual se ejecuta el proceso de mutacion
     * @param std desviacion estandar de la variable aleatoria normal
     * @param std desviacion estandar de la variable aleatoria normal MinChannelTime
     * @param std desviacion estandar de la variable aleatoria normal MaxChannelTime
     * @param dMutationProbability probabilidad de ejecutar la mutacion dirigida
     * @param grid Puntero a la rejilla
     */
    void doDirectedMutation(QList<Individual *> population, double std, double stdMin, double stdMax,
                            double dMutationProbability, NormativeGrid * grid);

    /**
     * @brief Ejecuta el proceso de mutacion dirigida sobre la base de la tabla C
     * del espacio de creencias.
     *
     * @param population poblacion sobre la cual se ejecuta el proceso de mutacion
     * @param std desviacion estandar de la variable aleatoria normal
     * @param std desviacion estandar de la variable aleatoria normal MinChannelTime
     * @param std desviacion estandar de la variable aleatoria normal MaxChannelTime
     * @param dMutationProbability probabilidad de ejecutar la mutacion dirigida
     * @param ctable tabla C del espacio de creencias
     */
    void doDirectedMutation(QList<Individual *> population, double std, double stdMin, double stdMax,
                            double dMutationProbability, CTable * ctable);

    /**
     * @brief Retorna la nueva poblacion luego de la variacion.
     *
     * Esta poblacion es de tamano 2P
     * @return Lista de tamano 2P con la nueva poblacion luego de ejecutada la mutacion
     */
    QList<Individual *> getNewPopulation();

    /**
     * Retorna un numero aletorio entre a y b
     *
     * @param a limite inferior
     * @param b limite superior
     * @return numero aleatorio entre a y b
     */
    int getRandom(int low, int high);

    /**
     * @brief Retorna un número aleatorio uniforme entre 0 y 1
     * @return número aleatorio uniforme entre 0 y 1
     */
    double getRandomUniform();

    /**
     * @brief Ejecuta una mutacion gausiana de un parametro de un individuo
     * @param index indice del parametro a mutar
     * @param mean media de la distribucion normal
     * @param std desviacion estandar de la distribucion normal
     * @param std desviacion estandar de la distribucion normal MinChannelTime
     * @param std desviacion estandar de la distribucion normal MaxChannelTime
     * @param currentParameterValue valor actual del parametro a mutar
     * @param offspring Individuo hijo sobre el cual se mutan parametros
     *
     * @return valor del parametro mutado
     */
    double mutateIndividualParameter(int index, int mean, double std, double stdMin, double stdMax, double currentParameterValue, Individual *offspring);

    /**
     * @brief Ejecuta la mutacion de parametros restantes de los IndividualSize-w genes para la mutacion dirigida
     * @param index
     * @param mean
     * @param std
     * @param stdMin
     * @param stdMax
     * @param currentParameterValue
     * @param offspring
     * @param busyChannels
     * @return
     */
    double mutateIndividualParameterModified(int index, int mean, double std, double stdMin, double stdMax,
                                             double currentParameterValue, Individual *offspring, QList<int> busyChannels);

    /**
     * @brief Retorna verdadero si el parametro a revisar de un individuo corresponde a un canal
     * @param index indice del parametro a revisar del individuo
     * @return Verdadero si el indice corresponde a un parametro de numero de canal
     */
    bool isThisParameterAChannel(int index);

    /**
     * @brief Retorna verdadero si el parametro a revisar de un individuo corresponde a un canal
     * @param index indice del parametro a revisar del individuo
     * @param individualSize tamano del individuo
     * @return Verdadero si el indice corresponde a un parametro de numero de canal
     */
    bool isThisParameterAChannel(int index, int individualSize);

    /**
     * @brief Retorna verdadero si el parametro a revisar de un individuo corresponde a un MinChannelTime
     * @param index indice del parametro a revisar del individuo
     * @return Verdadero si el indice corresponde a un parametro de MinChannelTime
     */
    bool isThisParameterAMinChannelTime(int index);

    /**
     * @brief Retorna verdadero si el parametro a revisar de un individuo corresponde a un MinChannelTime
     * @param index indice del parametro a revisar del individuo
     * @param individualSize tamano del individuo
     * @return Verdadero si el indice corresponde a un parametro de MinChannelTime
     */
    bool isThisParameterAMinChannelTime(int index, int individualSize);

    /**
     * @brief Retorna verdadero si el parametro a revisar de un individuo corresponde a un MaxChannelTime
     * @param index indice del parametro a revisar del individuo
     * @return Verdadero si el indice corresponde a un parametro de MaxChannelTime
     */
    bool isThisParameterAMaxChannelTime(int index);

    /**
     * @brief Retorna verdadero si el parametro a revisar de un individuo corresponde a un MaxChannelTime
     * @param index indice del parametro a revisar del individuo
     * @param individualSize tamano del individuo
     * @return Verdadero si el indice corresponde a un parametro de MaxChannelTime
     */
    bool isThisParameterAMaxChannelTime(int index, int individualSize);

    /**
     * @brief Retorna verdadero si el parametro a revisar de un individuo corresponde a numero de APs
     * @param index indice del parametro a revisar del individuo
     * @return Verdadero si el indice corresponde a un parametro de APs
     */
    bool isThisParameterAPs(int index);

    /**
     * @brief Retorna verdadero si el parametro a revisar de un individuo corresponde a numero de APs
     * @param index indice del parametro a revisar del individuo
     * @param individualSize tamano del individuo
     * @return Verdadero si el indice corresponde a un parametro de APs
     */
    bool isThisParameterAPs(int index, int individualSize);

    /**
     * @brief Imprime en una cadena legible al humano la nueva poblacion de tamano 2p
     */
    void printNewPopulation();

    /**
     * @brief Retorna un nuevo valor de descubrimiento de AP de los experimentos
     * @param channel canal en el que se esta escuchando
     * @param minChannelTime minChannelTime
     * @param maxChannelTime maxChannelTime
     * @param nscan numero de scans a repetir para obtener los APs
     * @return
     */
    double getNewParameterAPs(int channel, double minChannelTime, double maxChannelTime, int nscans);

    /**
     * @brief Ejecuta la mutacion original del articulo utilizando como patron el individuo
     * pasado como argumento
     * @param father individuo a tomar como patron para la mutacion original
     * @param std desviacion estandar de la distribucion normal (parametro del algoritmo)
     * @param deployedAp numero de APs desplegados
     */
    void originalMutation(Individual * father, double std, double stdMin, double stdMax);

    /**
     * @brief Ejecuta la mutacion dirigida
     *
     * @param grid Puntero a la rejilla para observar las Cell a utilizar en la mutacion
     * @param father Individuo padre para la mutacion
     */
    void directedMutation(NormativeGrid * grid, Individual * father, double stdMin, double stdMax);

    /**
     * @brief Ejecuta el proceso de mutacion dirigida
     *
     * @param ct tabla C del espacio de creencias
     */
    void directedMutation(CTable * ct, Individual *father);

    /**
     * @brief Ejecuta el proceso de mutacion dirigida
     *
     * @param ct tabla C del espacio de creencias
     * @param father individuo padre
     * @param stdMin desviacion estandar de min
     * @param stdMax desviacion estandar de max
     */
    void directedMutation(CTable * ct, Individual * father, double stdMin, double stdMax);

    /**
     * @brief Asigna la desviacion estandar para la mutacion
     * @param std desviacion estandar para la mutacion
     */
    void setStdDeviation(double std);

    /**
     * @brief Retorna la desviacion estandar de la mutacion
     * @return desviacion estandar de la mutacion
     */
    double getStdDeviation();

    /**
     * @brief Calcula la media y la deviación estandar del bloque/gen de un individuo
     * pasado como argumento
     *
     * list[0]->media de minChannelTime
     * list[1]->desviación estándar de minChannelTime
     * list[2]->media de maxChannelTime
     * list[3]->desviación estándar de maxChannelTime
     *
     * @param minChannelTimeList lista de valores de minChannelTime
     * @param maxChannelTimeList lista de valores de maxChannelTime
     * @param genIndex indice del gen del individuo
     * @return Lista con media y desviación estándar de minChannelTime y media y desviación estándar de minChannelTime
     */
    QHash<QString, double> calculateMeanAndStdDev(QList<double> minChannelTimeList, QList<double> maxChannelTimeList, int genIndex);


    int getPatternSequence(QList<int> channelList);


    /**
     * @brief Retorna el numero de APs encontrados al emular la repeticion de nscans full scanning y
     * seleccionar el mayor valor de APs
     * @param nscans numero de scanning a realizar para luego hacer el calculo
     * @param channel canal en el cual se realiza el scanning
     * @param min valor de MinChannelTime
     * @param max valor de MaxChannelTime
     * @return numero de APs encontrados al emular la repeticion de nscans full scanning y
     * seleccionar el mayor valor de APs
     */
    double getAPsFromMaxNumberApproach(int channel, double min, double max, int nscans);


    /**
     * @brief retorn un individuo con el numero de APs actualizados luego de ejecutar un full scanning
     * @param ind individuo con los valores del full scanning en
     * @return individuo con el numero de APs actualizados luego de ejecutar un full scanning
     */
    Individual * scanSequence(Individual * ind);

    /**
     * @brief Ejecuta 3 full scanning del individuo y actualiza los valores de AP encontrados
     * por canal y el valor de la funcion objetivo de descubierta.
     * @param offspring
     * @return individuo con valores actualizados de APs por canal lugeo de 3 full scanning
     */
    Individual *updateOffspring(Individual * offspring);

};

#endif // MUTATION_H
