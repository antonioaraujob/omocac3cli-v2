#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <QObject>
#include <QList>
#include <QSet>

/**
 * @brief Clase que modela un individuo del espacio de poblacion.
 *
 * Un individuo tiene la siguiente estructura:
 *
 * C_i|MinChannelTime_i|MaxChannelTime_i|AP_i|F1|F2
 *
 * con i= 1,...11. Donde:
 *
 * C_i: Canal i
 * MinChannelTime_i: parametro MinChannelTime para el canal i
 * MaxChannelTime_i: parametro MaxChannelTime para el canal i
 * AP_i: numero de APs en el canal i
 * F1: valor de la funcion objetivo 1 (Descubierta) para los parametros del individuo
 * F2: valor de la funcion objetivo 2 (Latencia) para los parametros del individuo
 *
 */
class Individual
{

private:
    /**
     * @brief Identificador unico del individuo
     */
    unsigned int individualId;

    /**
     *  @brief Lista de parametros de un individuo
     */
    QList<double> parametersList;

    /**
     * @brief Lista de indices de la funcion objetivo 1 para cada canal del individuo.
     *
     * Se almacena el valor de APmin/min + APmax/max para cada canal.
     */
    QList<double> of1IndexList;


    /**
     * @brief Valor de desempeno del individuo
     */
    double performanceValue;

    /**
     * @brief Valor de desempeno del individuo para la funcion objetivo 1 tasa de descubrimiento
     * de APs
     */
    double performanceDiscovery;

    /**
     * @brief Valor de desempeno del individuo para la funcion objetivo 2 latencia de full scanning
     *
     */
    double performanceLatency;

    /**
     * @brief Conjunto utilizado para generar secuencias de scanning aleatorias
     */
    QSet<int> channelSequenceSet;

    /**
     * @brief Contador de encuentros ganados en un torneo
     */
    int wonMatchesCounter;

    /**
     * @brief Tamano del individuo con respecto a la estructura:
     *
     * C_i|MinChannelTime_i|MaxChannelTime_i|AP_i|F1|F2
     *
     * con i= 1,...11.
     *
     * Esto representará una cadena de scanning de un tamano definido por el valor. Por
     * ejemplo se tendrán individuos con un canal o hasta con 11 (secuencia completa de 11
     * canales).
     *
     */
    int individualSize;

    /**
     * @brief Verdadero si se debe emular el scanning de acuerdo a la campana de medicion.
     * En otro caso se usa el modelo matematico del articulo de PIMRC 13
     */
    bool emulateScanning;

    /**
     * @brief Representa el numero de full scanning que se tomaran como referencia para
     * ejecutar el proceso de mutacion del numero de APs
     */
    int nscansForMutation;

public:
    /**
     * @brief Constructor de la clase
     *
     */
    Individual();

    /**
     * @brief Constructor de un individuo pseudointeligentes por la influencia de
     * condicionales iniciales.
     *
     * Se usa en el caso de pruebas de individuos que corresponderian a secuencias
     * que se conocen como buenas
     */
    //Individual(bool smart);

    /**
     * @brief Constructor de la clase
     * @param sequence secuencia de scanning de la forma <ch,m,M,ch,m,...ch,m,M>
     * ch: canal
     * m: MinChannelTime
     * M: MaxChannelTime
     * En total la secuencia deberia tener 33 parametros, 3 por cada posicion de canal
     */
    Individual(bool smart, QString sequence);

    /**
     * @brief Destructor de la clase
     */
    ~Individual();

    /**
     * @brief Constructor copia
     */
    Individual(const Individual &p);

    /**
     * @brief Retorna el Id del individuo
     * @return Retorna el Id del individuo
     */
     int getIndividualId() const;

    /**
     * @brief Retorna el tamano del individuo con respecto al numero de bloques en la cadena de escaneo
     * @return el tamano del individuo con respecto al numero de bloques en la cadena de escaneo
     */
    int getIndividualSize() const;

    /**
     * @brief Retorna un numero de canal aleatorio.
     *
     * Esta funcion se utiliza para inicializar los individuos de manera que los canales
     * aleatorios seleccionados no se repitan en la secuencia de scanning.
     *
     * @return Numero de canal aleatorio
     */
    int getRandomChannel();

    /**
     * @brief Retorna un numero de minChannelTime aleatorio
     * @return valor de minChannelTime aleatorio
     */
    double getRandomMinChannelTime();

    /**
     *  @brief Retorna un numero de maxChannelTime aleatorio
     * @return canal aleatorio
     */
    double getRandomMaxChannelTime();

    /**
     * @brief Retorna un numero de APs descubiertos en el canal i
     * dado un numero de AP desplegados.
     *
     * El valor se obtiene de la proporcion promedio de APs que operan en
     * cada canal.
     *
     * @param
     * @return Numero de APs descubiertos en el canal i
     */
    double getAPNumberOnChannel(int numberOfApsDeployed, int channel);

    /**
     *  @brief Imprime el individuo en una cadena legible al humano
     */
    void printIndividual();

    /**
     * @brief Retorna el individuo como una cadena
     * @return Cadena legible al humano de los parametros del individuo
     */
    QString getIndividualAsQString();

    /**
     *  @brief Establece el valor de desempeno del individuo
     * @param performance valor de desempeno
     */
    void setPerformanceValue(double performance);

    /**
     *  @brief Calcula el valor de desempeno del individuo
     */
    void calculatePerformanceValue();

    /**
     * @brief Retorna el valor de desempeno del individuo
     * @return valor de desempeno del individuo
     */
    double getPerformanceValue();


    /**
     *  @brief Calcula el valor de desempeno del descubrimiento
     */
    void calculateDiscoveryValue();

    /**
     *  @brief Calcula el valor de desempeno de la latencia
     */
    void calculateLatencyValue();

    /**
     *  @brief Establece el valor de desempeno de la funcion objetivo 1 tasa de descubrimiento
     * @param performance valor de desempeno
     */
    void setPerformanceDiscovery(double performance);

    /**
     * @brief Retorna el valor de desempeno del individuo para la funcion objetivo 1 tasa de descubrimiento
     * @return valor de desempeno de la tasa de descubrimiento del individuo
     */
    double getPerformanceDiscovery() const;

    /**
     *  @brief Establece el valor de desempeno de la funcion objetivo 2 latencia
     * @param performance valor de desempeno de latencia
     */
    void setPerformanceLatency(double performance);

    /**
     * @brief Retorna el valor de desempeno del individuo para la funcion objetivo 2 latencia
     * @return valor de desempeno de la latencia
     */
    double getPerformanceLatency() const;

    /**
     * @brief Asigna el valor del parametro en la posicion i
     * @param i indice del parametro a asignar
     * @param value valor a asignar en el parametro
     */
    void setParameter(int i, double value);

    /**
     * @brief Retorna el valor del parametro en la posicion i
     * @param i indice del parametro a retornar
     */
    double getParameter(int i) const;

    /**
     * @brief Retorna el numero de parametros del individuo
     * @return Numero de parametros del individuo
     */
    int getNumberOfParameters() const;

    /**
     * @brief Asigna el valor del contador de encuentros ganados en torneos
     * @param value numero de encuentros ganados por el individuo en un torneo
     */
    void setWonMatchesCounter(int value);

    /**
     * @brief Retorna el contador de encuentros ganados por el individuo en un torneo
     * @return El contador de encuentros ganados por el individuo en un torneo
     */
    int getWonMatchesCounter() const;

    /**
     * @brief Incrementa el contador de encuentros ganados del individuo en un torneo
     */
    void incrementWonMatchesCounter();

    /**
     * @brief Sobrecarga del operador = para asignar un individuo
     * @return nuevo individuo
     */
    //Individual& operator = (const Individual &ind);

    /**
     * @brief Retorna la probabilidad de encontrar al menos un AP en el canal pasado como argumento.
     *
     * La distribucion de probabilidad es aproximada de acuerdo al articulo que se esta estudiando
     *
     * @param channel Canal que se escanea
     * @return Valor de probabilidad de encontrar al menos un AP en el canal pasado como argumento.
     */
    double probabilityExistAtLeastOneAp(int channel);

    /**
     * @brief Retorna el porcentaje de APs en el canal pasado como argumento
     *
     *  El valor retornado  es aproximado de acuerdo al articulo que se esta estudiando
     *
     * @param channel canal para determinar el porcentaje de APs
     * @return el porcentaje de APs en el canal pasado como argumento
     */
    double percentageOfAps(int channel);

    /**
     * @brief Retorna la probabilidad de que el retardo del primer Probe Response sea menor que el
     * parametro MinChannelTime
     *
     * La distribucion de probabilidad es aproximada de acuerdo al articulo que se esta estudiando
     *
     * @param delay retardo del primer Probe Response recibido
     * @return probabilidad de que el retardo del primer Probe Response sea menor que el
     * parametro MinChannelTime
     */
    double probabilityDelayLessThanMinCT(double delay);

    /**
     * @brief Retorna la probabilidad de encontrar todos los AP disponibles en el canal i
     *
     * La distribucion de probabilidad es aproximada de acuerdo al articulo que se esta estudiando
     *
     * @param delay retardo del primer Probe Response recibido
     * @return probabilidad de encontrar todos los AP disponibles en el canal i
     * parametro MinChannelTime
     */
    double probabilityOfFindingAllAps(double delay);

    /**
     * @brief Retorna si se debe emular el scanning de acuerdo a la campana de medicion.
     * En otro caso se utiliza el modelo matematico.
     * @return Retorna si se debe emular el scanning de acuerdo a la campana de medicion.
     */
    bool getEmulateScanning() const;

    /**
     * @brief Representa el numero de full scanning que se tomaran como referencia para
     * ejecutar el proceso de mutacion del numero de APs
     */
    int getNscanForMutation() const;

    /**
     * @brief establece un nuevo valor aleatorio para nscans entre 1 y 8
     */
    void setNewNscansForMutation();

    /**
     * @brief asigna los parametros del individuo de acuerdo a una cadena particular
     */
    void setSmartParameters();


    /**
     * @brief Emula la ejecucion de un full scanning con los parametros actuales del individuo y
     * actualiza su valore de Funcion objetivo 1.
     */
    void executeFullScanning();

    /**
     * @brief Ejecuta un full scanning del individuo
     * @return un full scanning del individuo
     */
    Individual * scanSequence();

    /**
     * @brief Repite 30 full scanning (consultas al emulador) y calcula el valor promedio de APs
     * encontrados por cada bloque <ch, min, max> del individuo
     */
    void getAverageOnFullScanning();

    /**
     * @brief Retorna el valor FO del canal pasado como argumento
     * @param channelIndex
     * @return el valor FO del canal pasado como argumento
     */
    double getBestIndexFoValue(int channelIndex);


    double getAPsForMin(int channel, double min, double max);


    double getAPsForMax(int channel, double min, double max);

    /**
     * @brief Retorna el valor de FO del canal pasado como argumento
     *
     * La formula utilizada para obtener el valor de la FO en el canal i es la siguiente:
     *
     * Fo_i = (AP_descubiertos_con_min)/min + (AP_descubiertos_con_max)/max
     *
     * @param channel canal
     * @param min minchanneltime
     * @param max maxchanneltime
     * @return valor de FO del canal pasado como argumento
     */
    double getAPsByChannel(int channel, double min, double max);

    /**
     * @brief Retorna el valor de FO del canal pasado como argumento
     *
     * La formula utilizada para obtener el valor de la FO en el canal i es la siguiente:
     *
     * Fo_i = (AP_descubiertos_con_min)/min + (AP_descubiertos_con_max)/max
     *
     *
     * @return valor de FO1
     */
    double getAPsByAllChannels();

    /**
     * @brief Retorna el valor de FO1 para el individuo.
     *
     * Tambien actualiza el valor de la latencia del individuo:
     * si no se descubren APs con MinChannelTime no se suma el MaxChannelTime a la latencia
     *
     * La formula utilizada para obtener el valor de la FO en el canal i es la siguiente:
     *
     * Fo_i = (AP_descubiertos_con_min)/min + (AP_descubiertos_con_max)/max
     *
     *
     * @return valor de FO1
     */
    double getAPsAndLatencyOfAllChannels();

    /**
     * @brief Retorna el valor de FO1 de acuerdo a la formula:
     * La formula utilizada para obtener el valor de la FO en el canal i es la siguiente:
     *
     * Fo_i = (AP_descubiertos_con_min)/min + (AP_descubiertos_con_max)/max
     *
     * @return valor de FO1
     */
    double getFONC();

    /**
     * @brief Retorna la suma simple de los APs encontrados en los canales del individuo de acuerdo a los
     * temporizadores min y max
     * @return suma simple de los APs encontrados en los canales del individuo
     */
    double getSimpleAPsum();

    /**
     * @brief Retorna el indice de descubrimiento APmin/min + APmax/max de un canal dado
     * @param channelIndex indice del canal para obtener el indice de descubrimiento
     * @return indice de descubrimiento APmin/min + APmax/max de un canal dado
     */
    double getDiscoveryIndexForChannel(int channelIndex);

};

#endif // INDIVIDUAL_H
