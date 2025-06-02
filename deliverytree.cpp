#include "deliverytree.h"
#include "deliveryinfo.h"

DeliveryInfo* DeliveryTree::insert(DeliveryInfo* node, DeliveryInfo* newInfo) {
    if (!node) return newInfo;

    if (newInfo->nomeProduto < node->nomeProduto)
        node->left = insert(node->left, newInfo);
    else
        node->right = insert(node->right, newInfo);

    return node;
}

void DeliveryTree::insert(const QString& nomeProduto, double peso,
                          const QString& origem, const QString& destino,
                          double distancia, double custo, double tempo) {
    DeliveryInfo* newInfo = new DeliveryInfo(nomeProduto, peso, origem, destino, distancia, custo, tempo);
    root = insert(root, newInfo);
}

void DeliveryTree::inOrderTraversal(DeliveryInfo* node, QListWidget* listWidget) {
    if (!node) return;

    inOrderTraversal(node->left, listWidget);

    QString text = "Produto: " + node->nomeProduto + " (" + QString::number(node->peso) + " kg)\n" +
                   node->origem + " -> " + node->destino + "\n" +
                   "Distância: " + QString::number(node->distancia) + " km\n" +
                   "Custo Final: R$ " + QString::number(node->custo, 'f', 2) + "\n" +
                   "Tempo Estimado: " + QString::number(node->tempo, 'f', 1) + " h";

    listWidget->addItem(text);

    inOrderTraversal(node->right, listWidget);
}

void DeliveryTree::fillListWidget(QListWidget* listWidget) {
    listWidget->clear();
    inOrderTraversal(root, listWidget);
}

void DeliveryTree::clear(DeliveryInfo* node) {
    if (!node) return;
    clear(node->left);
    clear(node->right);
    delete node;
}

void DeliveryTree::clearTree() {
    clear(root);
    root = nullptr;
}
