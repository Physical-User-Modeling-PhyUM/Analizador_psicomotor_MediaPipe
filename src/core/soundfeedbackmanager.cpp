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
    //effect->setSource(QUrl(path));
    effect->setVolume(0.8);

    connect(effect, &QSoundEffect::statusChanged, this, [=]() {
        onSoundStatusChanged(effect, path);
    });

    soundMap.insert(type, effect);
}

void SoundFeedbackManager::onSoundStatusChanged(QSoundEffect* effect, const QString& path) {
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
}



/**
 * @brief Carga los sonidos predeterminados para condiciones específicas.
 */
void SoundFeedbackManager::loadSounds()
{

    addSound(ConditionType::OptimalForm, soundPath + "good_job.wav");
    addSound(ConditionType::FastMovement, soundPath +"fast.wav");
    addSound(ConditionType::SlowMovement, soundPath +"slow.wav");
    addSound(ConditionType::JointOverload, soundPath +"overload.wav");
    addSound(ConditionType::EndOfRepetition, soundPath +"rep.wav");
    addSound(ConditionType::EndOfSet, soundPath +"set.wav");
    addSound(ConditionType::ExerciseOvertime, soundPath +"time.wav");
    addSound(ConditionType::MaxStateTimeout, soundPath +"time.wav");
    addSound(ConditionType::MinStateTimeout, soundPath +"time.wav");
    addSound(ConditionType::RestOverTime, soundPath +"rest.wav");
    addSound(ConditionType::Has_Stopped, soundPath +"No_stop.wav");
    addSound(ConditionType::IncorrectExecution, soundPath +"Wrong_exec.wav");
    addSound(ConditionType::Not_Steady, soundPath +"not_steady.wav");
    addSound(ConditionType::RestTime, soundPath +"time.wav");
    addSound(ConditionType::symmetryDeviation, soundPath +"symetry.wav");
    addSound(ConditionType::exerciseInit, soundPath +"go.wav");
    //addSound(ConditionType::FastMovement, soundPath + "camera_qt.wav");

   // addSound(ConditionType::OptimalForm, "qrc:/sounds/tadaa_qt.wav");
    //addSound(ConditionType::OptimalForm, "qrc:/sounds/good_job.wav");
    // addSound(ConditionType::FastMovement, "qrc:/sounds/fast.wav");
    // addSound(ConditionType::SlowMovement, "qrc:/sounds/slow.wav");
    // addSound(ConditionType::JointOverload, "qrc:/sounds/overload.wav");
    // addSound(ConditionType::EndOfRepetition, "qrc:/sounds/rep.wav");
    // addSound(ConditionType::EndOfSet, "qrc:/sounds/set.wav");
    // addSound(ConditionType::ExerciseOvertime, "qrc:/sounds/time.wav");
    // addSound(ConditionType::MaxStateTimeout, "qrc:/sounds/time.wav");
    // addSound(ConditionType::MinStateTimeout, "qrc:/sounds/time.wav");
    // addSound(ConditionType::RestOverTime, "qrc:/sounds/rest.wav");
    // addSound(ConditionType::Has_Stopped, "qrc:/sounds/No_stop.wav");
    // addSound(ConditionType::IncorrectExecution, "qrc:/sounds/Wrong_exec.wav");
    // addSound(ConditionType::Not_Steady, "qrc:/sounds/not_steady.wav");
    // //addSound(ConditionType::OpositeDirection, "qrc:/sounds/.wav");
    // addSound(ConditionType::RestTime, "qrc:/sounds/time.wav");
    // addSound(ConditionType::symmetryDeviation, "qrc:/sounds/symetry.wav");
    // addSound(ConditionType::exerciseInit, "qrc:/sounds/go.wav");
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
    for (ConditionType type : feedback.getEntriesList()) {
        addtoSoundList(feedback, type);
    }
    if (!isPlaying)
        playNext();
}

void SoundFeedbackManager::addtoSoundList(const FeedBack& feedback, ConditionType type)
{
    if (!soundMap.contains(type)) return;

    if (QDateTime::currentMSecsSinceEpoch() - lastPlayTimestamp < 2000)
        return;

    QSoundEffect* effect = soundMap.value(type);

    if (!effect || !effect->isLoaded()) return;
    //if (soundQueue.contains(effect)) return;
    for (const auto& pair : soundQueue) {
        if (pair.first == effect)
            return;
    }


    // Si hay espacio, se agrega al final
    if (soundQueue.size() < 5) {
        soundQueue.append(QPair(effect,feedback.getCategory(type)));
        return;
    }

    // Si no hay espacio, recorremos para ver las posiciones de los mensajes
    int firstInfo = -1;
    int firstAlert = -1;
    int firstCritical = -1;

    for (int i = 0; i < soundQueue.size(); ++i) {
        ConditionType t = soundMap.key(soundQueue[i].first);
        ConditionCategory cat = soundQueue[i].second;

        if (cat == ConditionCategory::info && firstInfo == -1)
            firstInfo = i;
        else if (cat == ConditionCategory::alert && firstAlert == -1)
            firstAlert = i;
        else if (cat == ConditionCategory::critical && firstCritical == -1)
            firstCritical = i;
    }

    ConditionCategory newCat = feedback.getCategory(type);
    // una vez vista la posicion reemplazamos el sonido por el nuevo
    if (newCat == ConditionCategory::alert) {
        if (firstInfo != -1)
            soundQueue[firstInfo] = QPair(effect,ConditionCategory::alert);
        else if (firstAlert != -1)
            soundQueue[firstAlert] = QPair(effect,ConditionCategory::alert);
        else if (firstCritical != -1) {
            soundQueue.removeAt(firstCritical);
            soundQueue.append(QPair(effect,ConditionCategory::alert));
        }
    } else if (newCat == ConditionCategory::critical) {
        if (firstInfo != -1)

            soundQueue[firstInfo] = QPair(effect,ConditionCategory::critical);
        else if (firstAlert != -1)
            soundQueue[firstAlert] = QPair(effect,ConditionCategory::critical);
        else if (firstCritical != -1) {

            soundQueue.removeAt(firstCritical);
            soundQueue.append(QPair(effect,ConditionCategory::critical));
        }
    }
}
void SoundFeedbackManager::play(ConditionType cond, ConditionCategory catCond) {

    int firstInfo = -1;
    int firstAlert = -1;
    int firstCritical = -1;
    QSoundEffect* effect = soundMap.value(cond);

    if (QDateTime::currentMSecsSinceEpoch() - lastPlayTimestamp < 2000)
        return;

    if (!effect || !effect->isLoaded()) return;
    for (const auto& pair : soundQueue) {
        if (pair.first == effect)
            return;
    }

    // Si hay espacio, se agrega al final
    if (soundQueue.size() < 5) {
        soundQueue.append(QPair(effect,catCond));
        return;
    }

    for (int i = 0; i < soundQueue.size(); ++i) {
        ConditionType t = soundMap.key(soundQueue[i].first);
        ConditionCategory cat = soundQueue[i].second;

        if (cat == ConditionCategory::info && firstInfo == -1)
            firstInfo = i;
        else if (cat == ConditionCategory::alert && firstAlert == -1)
            firstAlert = i;
        else if (cat == ConditionCategory::critical && firstCritical == -1)
            firstCritical = i;
    }


    // una vez vista la posicion reemplazamos el sonido por el nuevo
    if (catCond == ConditionCategory::alert) {
        if (firstInfo != -1)
            soundQueue[firstInfo] = QPair(effect,ConditionCategory::alert);
        else if (firstAlert != -1)
            soundQueue[firstAlert] = QPair(effect,ConditionCategory::alert);
        else if (firstCritical != -1) {
            soundQueue.removeAt(firstCritical);
            soundQueue.append(QPair(effect,ConditionCategory::alert));
        }
    } else if (catCond == ConditionCategory::critical) {
        if (firstInfo != -1)

        soundQueue[firstInfo] = QPair(effect,ConditionCategory::critical);
        else if (firstAlert != -1)
            soundQueue[firstAlert] = QPair(effect,ConditionCategory::critical);
        else if (firstCritical != -1) {

            soundQueue.removeAt(firstCritical);
            soundQueue.append(QPair(effect,ConditionCategory::critical));
        }
    }
    if (!isPlaying)
        playNext();

}


/**
 * @brief Reproduce el sonido asociado a una única condición.
 * @param cond Tipo de condición a reproducir.
 */
void SoundFeedbackManager::play(ConditionType cond) {
    if (soundMap.contains(cond)) {
        QSoundEffect* effect = soundMap.value(cond);
        if (effect && effect->isLoaded()) {
            //effect->play();
            //soundQueue.append(effect);

            if (!isPlaying)
                playNext();
        } else {
            qWarning() << "Sonido no cargado o nulo para:" << conditionTypeToString(cond);
        }
    }
}

void SoundFeedbackManager::playNext() {

    if (soundQueue.isEmpty() || isPlaying)
        return;

    auto pair = soundQueue.takeFirst();
    currentPlayingEffect = pair.first;

    isPlaying = true;
    connect(currentPlayingEffect, &QSoundEffect::playingChanged,
            this, &SoundFeedbackManager::onSoundFinished, Qt::UniqueConnection);
    currentPlayingEffect->play();
    lastPlayTimestamp = QDateTime::currentMSecsSinceEpoch();


}

void SoundFeedbackManager::onSoundFinished() {

    if (!currentPlayingEffect || currentPlayingEffect->isPlaying())
        return;  // Aún se está reproduciendo, no hacer nada

    disconnect(currentPlayingEffect, &QSoundEffect::playingChanged,
               this, &SoundFeedbackManager::onSoundFinished);

    isPlaying = false;
    currentPlayingEffect = nullptr;
    playNext();
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
