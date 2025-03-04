import cv2
import mediapipe as mp
import numpy as np
import multiprocessing.shared_memory as shm
import json
import threading

# Configuración de MediaPipe Pose
mp_pose = mp.solutions.pose
pose = mp_pose.Pose()

# Dimensiones de la imagen
WIDTH, HEIGHT = 640, 480
FRAME_SIZE = WIDTH * HEIGHT * 3  # Total de bytes en una imagen RGB

# Crear memoria compartida para imágenes de las dos cámaras
shm_cam1 = shm.SharedMemory(create=True, size=FRAME_SIZE, name="shm_cam1")
shm_cam2 = shm.SharedMemory(create=True, size=FRAME_SIZE, name="shm_cam2")

# Crear memoria compartida para los keypoints (formato JSON)
shm_kp = shm.SharedMemory(create=True, size=2048, name="shm_keypoints")  # 2KB para los datos de keypoints

def capture_camera(camera_index, shm_name):
    """Captura imágenes de la cámara y almacena en memoria compartida."""
    cap = cv2.VideoCapture(camera_index)
    if not cap.isOpened():
        print(f"Error: No se pudo abrir la cámara {camera_index}")
        return

    memory = shm.SharedMemory(name=shm_name)

    while True:
        ret, frame = cap.read()
        if not ret:
            continue

        frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        np_frame = np.array(frame_rgb, dtype=np.uint8).flatten()
        memory.buf[:FRAME_SIZE] = np_frame.tobytes()

        cv2.waitKey(1)  # Pequeña pausa para sincronizar

def process_pose():
    """Captura imágenes de ambas cámaras y obtiene keypoints con MediaPipe Pose."""
    cap1 = cv2.VideoCapture(0)  # Primera cámara
    cap2 = cv2.VideoCapture(1)  # Segunda cámara

    if not cap1.isOpened() or not cap2.isOpened():
        print("Error: No se pudieron abrir las cámaras.")
        return

    while True:
        ret1, frame1 = cap1.read()
        ret2, frame2 = cap2.read()
        if not ret1 or not ret2:
            continue

        rgb_frame1 = cv2.cvtColor(frame1, cv2.COLOR_BGR2RGB)
        rgb_frame2 = cv2.cvtColor(frame2, cv2.COLOR_BGR2RGB)

        results1 = pose.process(rgb_frame1)
        results2 = pose.process(rgb_frame2)

        keypoints = {"cam1": {}, "cam2": {}}

        # Extraer keypoints de la primera cámara
        if results1.pose_landmarks:
            for idx, lm in enumerate(results1.pose_landmarks.landmark):
                keypoints["cam1"][idx] = [lm.x * WIDTH, lm.y * HEIGHT]  # Escalar a la imagen

        # Extraer keypoints de la segunda cámara
        if results2.pose_landmarks:
            for idx, lm in enumerate(results2.pose_landmarks.landmark):
                keypoints["cam2"][idx] = [lm.x * WIDTH, lm.y * HEIGHT]  # Escalar a la imagen

        # Guardar keypoints en memoria compartida
        json_data = json.dumps(keypoints)
        shm_kp.buf[:len(json_data)] = json_data.encode('utf-8')

        cv2.waitKey(1)  # Espera pequeña para sincronización

# Lanzar hilos para capturar imágenes en paralelo
threading.Thread(target=capture_camera, args=(0, "shm_cam1"), daemon=True).start()
threading.Thread(target=capture_camera, args=(1, "shm_cam2"), daemon=True).start()
threading.Thread(target=process_pose, daemon=True).start()

# Mantener el proceso en ejecución
while True:
    pass
