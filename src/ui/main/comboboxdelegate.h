/**
 * @file comboboxdelegate.h
 * @brief Declaración de la clase ComboBoxDelegate, un delegado personalizado para usar QComboBox en celdas de QTableView.
 *
 * Esta clase permite mostrar un combo box (QComboBox) en una celda de una tabla, facilitando la selección de valores
 * desde una lista predefinida. Está basada en QStyledItemDelegate y permite editar los datos de forma visual y controlada.
 */

#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H

#include <QStyledItemDelegate>
#include <QStringList>
#include <QComboBox>

/**
 * @class ComboBoxDelegate
 * @brief Delegado de edición que usa un QComboBox como editor de celdas en una tabla.
 *
 * Esta clase proporciona un combo desplegable para editar valores dentro de una celda en una vista basada en modelo,
 * como QTableView o QTableWidget. Se inicializa con una lista de opciones y gestiona la creación, visualización
 * y escritura de datos desde/hacia el modelo de datos.
 */
class ComboBoxDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    /**
     * @brief Constructor del delegado.
     * @param parent Objeto padre.
     * @param items Lista de opciones que aparecerán en el QComboBox.
     */
    ComboBoxDelegate(QObject *parent = nullptr, const QStringList& items = QStringList());

    /**
     * @brief Crea el editor de celda (QComboBox) cuando se activa la edición.
     * @param parent Widget padre del editor.
     * @param option Opciones de estilo y geometría.
     * @param index Índice del modelo correspondiente a la celda.
     * @return Puntero al editor creado (QComboBox).
     */
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    /**
     * @brief Inicializa el QComboBox con el valor actual del modelo.
     * @param editor Editor de celda (QComboBox).
     * @param index Índice del modelo que se está editando.
     */
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    /**
     * @brief Extrae el valor del QComboBox y lo guarda en el modelo.
     * @param editor Editor de celda (QComboBox).
     * @param model Modelo de datos asociado.
     * @param index Índice de la celda modificada.
     */
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    /**
     * @brief Ajusta la geometría del editor al área de la celda.
     * @param editor Editor de celda.
     * @param option Opción de estilo que contiene la geometría.
     * @param index Índice del modelo.
     */
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    QStringList itemList; ///< Lista de elementos que aparecerán en el QComboBox.
};

#endif // COMBOBOXDELEGATE_H
