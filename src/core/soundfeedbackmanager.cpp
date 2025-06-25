/**
 * @file soundfeedbackmanager.cpp
 * @brief Implementación de la clase SoundFeedbackManager para gestión de sonidos de feedback.
 */
#include "soundfeedbackmanager.h"
#include <QCoreApplication>
#include <QDebug>
/**
 * @brief Constructor del SoundFeedbackManager. Establece la ruta de sonidos y los carga.
 */
SoundFeedbackManager::SoundFeedbackManager(QObject *parent)
    : QObject(parent)
{
    soundPath = QCoreApplication::applicationDirPath() + "/sounds/";
    loadSounds();
}
/**
 * @brief Asocia un archivo de sonido con un tipo de condición.
 * @param type Tipo de condición a asociar.
 * @param path Ruta completa al archivo de sonido.
 */
void SoundFeedbackManager::addSound(ConditionType type, const QString &path)
{
    // QSoundEffect effect;
    // effect.setSource(QUrl::fromLocalFile("engine.wav"));
    // effect.setLoopCount(QSoundEffect::Infinite);
    // effect.setVolume(0.25f);
    // effect.play();

    QSoundEffect* effect = new QSoundEffect(this);
    effect->setSource(QUrl::fromLocalFile(path));
    effect->setVolume(0.8);

    connect(effect, &QSoundEffect::statusChanged, this, [effect, path]() {
        switch (effect->status()) {
        case QSoundEffect::Ready:
            qDebug() << "Sonido cargado correctamente:" << path;
            break;
        case QSoundEffect::Error:
            qCritical() << "Error: No se pudo decodificar el archivo de sonido:" << path;
            break;
        default:
            break;
        }
    });

    soundMap.insert(type, effect);
}
/**
 * @brief Carga los sonidos predeterminados para condiciones específicas.
 */
void SoundFeedbackManager::loadSounds()
{
    //addSound(ConditionType::OptimalForm, "tadaa.wav");
    //addSound(ConditionType::OptimalForm, "tadaa_fixed.wav");
    //addSound(ConditionType::FastMovement, "camera.wav");
    //addSound(ConditionType::OptimalForm, "tadaa_qt.wav");
    //addSound(ConditionType::FastMovement, "camera_qt.wav");

    addSound(ConditionType::OptimalForm, soundPath + "tadaa.wav");
    addSound(ConditionType::FastMovement, soundPath + "camera_qt.wav");


    // addSound(ConditionType::SlowMovement, "slow.wav");
    // addSound(ConditionType::JointOverload, "overload.wav");
    // addSound(ConditionType::EndOfSet, "endset.wav");
    // addSound(ConditionType::EndOfRepetition, "endrep.wav");
    // addSound(ConditionType::ExerciseOvertime, "overtime.wav");
}
/**
 * @brief Reproduce los sonidos correspondientes a las condiciones contenidas en un feedback.
 * @param feedback Objeto que contiene las condiciones evaluadas.
 */
void SoundFeedbackManager::playFeedback(const FeedBack &feedback)
{
    for (auto it = soundMap.constBegin(); it != soundMap.constEnd(); ++it) {
        ConditionType type = it.key();

        if (feedback.contains(type)) {

            QSoundEffect* effect = it.value();
            if (effect!=nullptr && effect->isLoaded()) {
                effect->play();
            }
        }
    }
}
/**
 * @brief Reproduce el sonido asociado a una única condición.
 * @param cond Tipo de condición a reproducir.
 */
void SoundFeedbackManager::play(ConditionType cond) {
    if (soundMap.contains(cond)) {
        QSoundEffect* effect = soundMap.value(cond);
        if (effect && effect->isLoaded()) {
            effect->play();
        } else {
            qWarning() << "Sonido no cargado o nulo para:" << conditionTypeToString(cond);
        }
    }
}
/**
 * @brief Establece el volumen de todos los sonidos activos.
 * @param volume Nivel de volumen (0.0 a 1.0).
 */
void SoundFeedbackManager::setVolume(qreal volume)
{
    for (QSoundEffect* effect : soundMap.values()) {
        if (effect!=nullptr)
            effect->setVolume(volume);
    }
}
/**
 * @brief Silencia o restaura el volumen de todos los sonidos.
 * @param mute true para silenciar, false para restaurar.
 */
void SoundFeedbackManager::mute(bool mute)
{
    (mute)?setVolume(0.0):setVolume(0.8);

}
/**
 * @brief Activa o desactiva sonidos informativos.
 * @param newInfoSoundEnable Bandera para sonidos informativos.
 */
void SoundFeedbackManager::setInfoSoundEnable(bool newInfoSoundEnable)
{
    infoSoundEnable = newInfoSoundEnable;
}
/**
 * @brief Activa o desactiva sonidos de alerta.
 * @param newAlertSoundEnabled Bandera para sonidos de alerta.
 */
void SoundFeedbackManager::setAlertSoundEnabled(bool newAlertSoundEnabled)
{
    alertSoundEnabled = newAlertSoundEnabled;
}
/**
 * @brief Activa o desactiva sonidos críticos.
 * @param newCriticaSoundEnable Bandera para sonidos críticos.
 */
void SoundFeedbackManager::setCriticaSoundEnable(bool newCriticaSoundEnable)
{
    criticaSoundEnable = newCriticaSoundEnable;
}
