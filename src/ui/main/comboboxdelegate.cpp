
#include "comboboxdelegate.h"
#include <QComboBox>

ComboBoxDelegate::ComboBoxDelegate(QObject *parent, const QStringList& items)
    : QStyledItemDelegate(parent), itemList(items) {}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const {

    QComboBox *editor = new QComboBox(parent);
    editor->addItems(itemList);
    return editor;
}

// QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const {
//     QComboBox *editor = new QComboBox(parent);

//     QSet<QString> usados;
//     const QAbstractItemModel* model = index.model();

//     // Escanea la columna actual y recoge los valores ya usados (excepto la fila en edición)
//     for (int r = 0; r < model->rowCount(); ++r) {
//         if (r == index.row()) continue; // Ignora la fila que se está editando
//         usados.insert(model->data(model->index(r, index.column()), Qt::EditRole).toString());
//     }

//     // Añade solo los elementos no usados al combo box
//     for (const QString& item : itemList) {
//         if (!usados.contains(item)) {
//             editor->addItem(item);
//         }
//     }

//     return editor;
// }


void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {

    QString value = index.model()->data(index, Qt::EditRole).toString();

    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    int idx = comboBox->findText(value);

    if (idx >= 0)
        comboBox->setCurrentIndex(idx);
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString value = comboBox->currentText();
    model->setData(index, value, Qt::EditRole);
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const {
    editor->setGeometry(option.rect);
}
