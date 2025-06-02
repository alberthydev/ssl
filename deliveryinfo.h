#ifndef DELIVERYINFO_H
#define DELIVERYINFO_H

#include <QString>

struct DeliveryInfo {
    QString nomeProduto;
    double peso;
    QString origem;
    QString destino;
    double distancia;
    double custo;
    double tempo;

    DeliveryInfo* left = nullptr;
    DeliveryInfo* right = nullptr;

    DeliveryInfo(QString np, double p, QString o, QString d, double dist, double c, double t)
        : nomeProduto(np), peso(p), origem(o), destino(d), distancia(dist), custo(c), tempo(t),
        left(nullptr), right(nullptr) {}
};

#endif // DELIVERYINFO_H
