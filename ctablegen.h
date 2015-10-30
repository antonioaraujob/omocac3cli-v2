#ifndef CTABLEGEN_H
#define CTABLEGEN_H

/**
 * @brief Clase que abstrae un gen de la Tabla C del espacio de creencias para
 * mutacion dirigida.
 *
 * La estructura del un CTableGen es la siguiente:
 *
 * <Channel, MinChannelTime, MaxChannel, APs, indexA, indexB, indexC>
 *
 */
class CTableGen
{

private:

    /**
     * @brief Canal del gen
     */
    int channel;

    /**
     * @brief MinChannelTime del gen
     */
    double minChannelTime;

    /**
     * @brief MaxChannelTime del gen
     */
    double maxChannelTime;

    /**
     * @brief Numero de APs del gen
     */
    double aps;

    /**
     * @brief indice A del gen
     */
    double indexA;

    /**
     * @brief indice B del gen
     */
    double indexB;

    /**
     * @brief indice C del gen
     */
    double indexC;

public:

    /**
     * @brief Constructor del CTableGen
     *
     * @param ch canal del gen
     * @param min minChannelTime del gen
     * @param max maxChannelTime del gen
     * @param ap numero de aps del gen
     * @param indA indiceA asociado al gen
     * @param indB indiceB asociado al gen
     * @param indC indiceC asociado al gen
     */
    CTableGen(int ch, double min, double max, double ap, double indA, double indB, double indC);

    /**
     * @brief Constructor copia
     * @param g gen a copiar
     */
    CTableGen(const CTableGen &g);

    /**
     * @brief Destructor del gen
     */
    ~CTableGen();

    /**
     * @brief Retorna el canal del gen
     * @return el canal del gen
     */
    double getChannel() const;

    /**
     * @brief Asigna el canal al gen
     * @param ch
     */
    void setChannel(int ch);

    /**
     * @brief Retorna el minChannelTime del gen
     * @return el minChannelTime del gen
     */
    double getMinChannelTime() const;

    /**
     * @brief Asigna el minChannelTime del gen
     */
    void setMinChannelTime(double min);

    /**
     * @brief Retorna el maxChannelTime del gen
     * @return el maxChannelTime del gen
     */
    double getMaxChannelTime() const;

    /**
     * @brief Asigna el maxChannelTime del gen
     */
    void setMaxChannelTime(double max);

    /**
     * @brief Retorna el numero de APs del gen
     * @return el numero de APs del gen
     */
    double getAPs() const;

    /**
     * @brief Asigna el numero de APs del gen
     */
    void setAPs(double ap);

    /**
     * @brief Retorna el valor asociado al indexA del gen
     * @return el valor asociado al indexA del gen
     */
    double getIndexA() const;

    /**
     * @brief Asigna el indice A del gen
     */
    void setIndexA(double indA);

    /**
     * @brief Calcula el valor del indice A y lo asigna
     */
    void calculateIndexA();

    /**
     * @brief Retorna el valor asociado al indexB del gen
     * @return el valor asociado al indexB del gen
     */
    double getIndexB() const;

    /**
     * @brief Asigna el indice B del gen
     */
    void setIndexB(double indB);

    /**
     * @brief Calcula el valor del indice B y lo asigna
     */
    void calculateIndexB();

    /**
     * @brief Retorna el valor asociado al indexC del gen
     * @return el valor asociado al indexC del gen
     */
    double getIndexC() const;

    /**
     * @brief Asigna el indice C del gen
     */
    void setIndexC(double indC);

    /**
     * @brief Calcula el valor del indice C y lo asigna
     */
    void calculateIndexC();

    /**
     * @brief retorna el FONC del gen
     * @return retorna el FONC del gen
     */
    double getFONC();

    /**
     * @brief Obtiene el numero de APs consultando la topologia
     * @return numero de APs consultando la topologia
     */
    double calculateAPs();

};

#endif // CTABLEGEN_H
