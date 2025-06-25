/**
 * @file soundfeedbackmanager.h
 * @brief Declaración de la clase SoundFeedbackManager encargada del feedback sonoro en base a condiciones biomecánicas.
 *
 * Esta clase gestiona los efectos de sonido asociados a distintos tipos de condiciones detectadas durante un ejercicio.
 * Permite configurar qué sonidos se reproducen, ajustar el volumen y silenciar la salida.
 */

#ifndef SOUNDFEEDBACKMANAGER_H
#define SOUNDFEEDBACKMANAGER_H

#include <QObject>
#include <QHash>
#include <QSoundEffect>
#include "pose/feedback.h"
#include "pose/condition.h"
#include <QDir>

/**
 * @class SoundFeedbackManager
 * @brief Clase encargada de reproducir sonidos asociados a condiciones específicas del ejercicio.
 *
 * Esta clase permite la asociación entre `ConditionType` y archivos de sonido. Soporta reproducción de múltiples
 * sonidos simultáneos, control de volumen, y configuración de filtros según el nivel de criticidad (informativo, alerta, crítico).
 */
class SoundFeedbackManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructor por defecto del gestor de feedback sonoro.
     * @param parent Objeto Qt padre.
     */
    explicit SoundFeedbackManager(QObject *parent = nullptr);

    /**
     * @brief Reproduce todos los sonidos asociados a las condiciones contenidas en el feedback.
     * @param feedback Objeto de tipo `FeedBack` con las condiciones generadas.
     */
    void playFeedback(const FeedBack& feedback);

    /**
     * @brief Reproduce directamente el sonido asociado a una condición específica.
     * @param cond Condición a reproducir.
     */
    void play(ConditionType cond);

    /**
     * @brief Establece el volumen global de los sonidos.
     * @param volume Valor de volumen entre 0.0 y 1.0.
     */
    void setVolume(qreal volume);

    /**
     * @brief Silencia o reactiva todos los sonidos.
     * @param mute true para silenciar, false para restaurar volumen.
     */
    void mute(bool mute);

    /**
     * @brief Habilita o deshabilita sonidos informativos.
     * @param newInfoSoundEnable true para habilitar.
     */
    void setInfoSoundEnable(bool newInfoSoundEnable);

    /**
     * @brief Habilita o deshabilita sonidos de alerta.
     * @param newAlertSoundEnabled true para habilitar.
     */
    void setAlertSoundEnabled(bool newAlertSoundEnabled);

    /**
     * @brief Habilita o deshabilita sonidos críticos.
     * @param newCriticaSoundEnable true para habilitar.
     */
    void setCriticaSoundEnable(bool newCriticaSoundEnable);

private:
    QHash<ConditionType, QSoundEffect*> soundMap; ///< Mapa que asocia condiciones con efectos de sonido.

    bool infoSoundEnable = true;    ///< Bandera para sonidos informativos.
    bool alertSoundEnabled = true;  ///< Bandera para sonidos de alerta.
    bool criticaSoundEnable = true; ///< Bandera para sonidos críticos.

    QString soundPath; ///< Ruta base a la carpeta de sonidos.

    /**
     * @brief Carga los sonidos predeterminados desde la carpeta de configuración.
     */
    void loadSounds();

    /**
     * @brief Añade un nuevo sonido al mapa de condiciones.
     * @param type Tipo de condición.
     * @param path Ruta al archivo de sonido.
     */
    void addSound(ConditionType type, const QString& path);
};

#endif // SOUNDFEEDBACKMANAGER_H
