#ifndef DELIVERYTREE_H
#define DELIVERYTREE_H

#include "deliveryinfo.h"
#include <QListWidget>

class DeliveryTree {
private:
    DeliveryInfo* root = nullptr;

    DeliveryInfo* insert(DeliveryInfo* node, DeliveryInfo* newInfo);
    void inOrderTraversal(DeliveryInfo* node, QListWidget* listWidget);
    void clear(DeliveryInfo* node);

public:
    void insert(const QString& nomeProduto, double peso,
                const QString& origem, const QString& destino,
                double distancia, double custo, double tempo);

    void fillListWidget(QListWidget* listWidget);
    void clearTree();
};

#endif // DELIVERYTREE_H
