#ifndef SOUNDFEEDBACKMANAGER_H
#define SOUNDFEEDBACKMANAGER_H

#include <QObject>
#include <QHash>
#include <QSoundEffect>
#include "pose/feedback.h"
#include "pose/condition.h"
#include <QDir>
#include <QList>
#include <QTimer>

/**
 * @class SoundFeedbackManager
 * @brief Clase encargada de reproducir sonidos asociados a condiciones específicas del ejercicio.
 *
 * Esta clase permite la asociación entre `ConditionType` y archivos de sonido. Soporta reproducción priorizada
 * en tiempo real, control de volumen, enmudecimiento global y activación selectiva por severidad.
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
     * Aplica prioridad crítica > alerta > informativa y controla la cola de reproducción con un máximo de 5 sonidos.
     * @param feedback Objeto de tipo `FeedBack` con las condiciones generadas.
     */
    void playFeedback(const FeedBack& feedback);

    /**
     * @brief Reproduce directamente el sonido asociado a una condición específica si no hay otro en reproducción.
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

    void play(ConditionType cond, ConditionCategory cat);
    /**
     * @brief Reproduce el siguiente sonido en la cola si no hay ninguno en reproducción.
     *
     * Este método se llama de forma periódica mediante un QTimer. Si no hay ningún sonido
     * en reproducción actualmente y la cola contiene sonidos pendientes, se reproduce el siguiente.
     *
     * También establece una conexión temporal a la señal `playingChanged` del QSoundEffect
     * para detectar cuándo finaliza la reproducción, permitiendo continuar con el siguiente sonido.
     */
    void checkAndPlayNext();

    /**
     * @brief Añade un sonido a la cola con lógica de prioridad y reemplazo, según la categoría proporcionada por el feedback.
     * Si la cola ya contiene el sonido, no se añade. Si está llena, se aplica la siguiente política:
     * - ALERTA reemplaza INFO; si no hay, reemplaza ALERTA; si todos son CRÍTICOS, elimina el primero y añade al final.
     * - CRÍTICO reemplaza INFO o ALERTA; si todos son CRÍTICOS, elimina el primero y añade al final.
     * @param feedback Objeto `FeedBack` que proporciona la categoría del sonido.
     * @param type Tipo de condición cuyo sonido se desea añadir.
     */
    void addtoSoundList(ConditionType cond, ConditionCategory catCond);
private:
    QHash<ConditionType, QSoundEffect*> soundMap; ///< Mapa que asocia condiciones con efectos de sonido.
    QList<QPair<QSoundEffect*,ConditionCategory>> soundQueue;              ///< Cola de reproducción priorizada.
    bool isPlaying = false;                       ///< Indica si un sonido está siendo reproducido actualmente.

    bool infoSoundEnable = true;    ///< Bandera para sonidos informativos.
    bool alertSoundEnabled = true;  ///< Bandera para sonidos de alerta.
    bool criticaSoundEnable = true; ///< Bandera para sonidos críticos.
    qint64 lastPlayTimestamp = 0; ///< Tiempo (ms) del último sonido reproducido
    const int minDelayMs = 2000;
    QSoundEffect* currentPlayingEffect = nullptr;
    QString soundPath; ///< Ruta base a la carpeta de sonidos.
    QTimer* soundTimer;///<Temporizador para la emisión de los sonidos
    /**
     * @brief Carga los sonidos predeterminados desde la carpeta de configuración.
     */
    void loadSounds();

    /**
     * @brief Reproduce el siguiente sonido de la cola, si existe.
     */
    void playNext();

    /**
     * @brief Añade un nuevo sonido al mapa de condiciones.
     * @param type Tipo de condición.
     * @param path Ruta al archivo de sonido.
     */
    void addSound(ConditionType type, const QString& path);

    /**
     * @brief Gestiona eventos del sistema de sonido cuando se completa la carga o se produce un error.
     * @param effect Efecto de sonido implicado.
     * @param path Ruta del archivo de sonido.
     */
    void onSoundStatusChanged(QSoundEffect *effect, const QString &path);

    /**
     * @brief Añade un sonido a la cola con lógica de prioridad y reemplazo, según la categoría proporcionada por el feedback.
     * Si la cola ya contiene el sonido, no se añade. Si está llena, se aplica la siguiente política:
     * - ALERTA reemplaza INFO; si no hay, reemplaza ALERTA; si todos son CRÍTICOS, elimina el primero y añade al final.
     * - CRÍTICO reemplaza INFO o ALERTA; si todos son CRÍTICOS, elimina el primero y añade al final.
     * @param feedback Objeto `FeedBack` que proporciona la categoría del sonido.
     * @param type Tipo de condición cuyo sonido se desea añadir.
     */
    void addtoSoundList(const FeedBack& feedback, ConditionType type);

private slots:
    /**
     * @brief Slot llamado automáticamente al terminar un sonido, lanza el siguiente si existe.
     */
    void onSoundFinished();
};

#endif // SOUNDFEEDBACKMANAGER_H
