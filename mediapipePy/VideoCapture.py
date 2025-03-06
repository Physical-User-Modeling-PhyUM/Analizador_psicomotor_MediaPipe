import cv2
import mediapipe as mp
import numpy as np
import multiprocessing.shared_memory as shm
import json
import threading
import time

# Configuraciion de Mediapipe

mp_pose = mp.solutions.pose
pose = mp_pose.Pose()

# Se obtienen las dimensiones de la imagen para calcular su tamaño
WIDTH, HEIGHT = 640, 480
FRAME_SIZE = WIDTH * HEIGHT * 3  # Total de bytes en una imagen RGB

#Debemos pasar las imagenes y los keypoints a la aplicación. Por tanto creamos memorias compartidas para
#las imagenes y para los keypoints que luego recuperaremos.

# Crear memoria compartida para imágenes de las dos cámaras
mem_cam1 = shm.SharedMemory( name="mem_cam1",create=True, size=FRAME_SIZE,)
mem_cam2 = shm.SharedMemory( name="mem_cam2",create=True, size=FRAME_SIZE)

# Crear memoria compartida para los keypoints (formato JSON)
mem_kp = shm.SharedMemory(create=True, size=2048, name="shm_keypoints")  # 2KB para los datos de keypoints



#función que asigna una memoria a compartida a una camara y lee la imagende la misma
def capture_camera(camera_index, mem_name):

    cap = cv2.VideoCapture(camera_index)
    if not cap.isOpened():
        print(f"Error: No se pudo obtener la imagen de la camara {camera_index}")
        return

    memory = shm.SharedMemory(name=mem_name)

    while True:
        ret, frame = cap.read()
        if not ret:
            continue

        frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        #obtenemos el array con la imagen de la cámara
        #Escribimos la imagen en la memoria compartida
        np_frame = np.array(frame_rgb, dtype=np.uint8).flatten()
        # memory.buf[:FRAME_SIZE] = np_frame
        memory.buf[:FRAME_SIZE] = np_frame.tobytes()
        #  pausa de  sincronizacion
        cv2.waitKey(1)



# Funcion que captura las imágenes de las cámaras y obtienen los keypoints.
def process_pose():

    cap1 = cv2.VideoCapture(0)  # Primera cámara
    cap2 = cv2.VideoCapture(1)  # Segunda cámara

    if not cap1.isOpened() or not cap2.isOpened():
        print("Error: No se pudo obtener la imagen de la camara")
        return

    while True:
        #leemos las imagenes de las camaras
        ret1, frame1 = cap1.read()
        ret2, frame2 = cap2.read()

        # si falla la captura de alguna se intentara otra vez, así nos aseguramos que los keypoints esten sincronizados
        if not ret1 or not ret2:
            continue

        # Convertir imágenes a formato RGB
        img_frame1 = cv2.cvtColor(frame1, cv2.COLOR_BGR2RGB)
        img_frame2 = cv2.cvtColor(frame2, cv2.COLOR_BGR2RGB)

        results1 = pose.process(img_frame1)
        results2 = pose.process(img_frame2)

        #diccionario para almacenar los keypoints de las camaras tomados al mismo tiempo y la marca temporal
        keypoints = {
                 "timestamp": timestamp, #en milisegundos
                 "cam1": {},
                 "cam2": {}
        }

        # se obtinenen los keypoints de la primera cámara
        #se multiplica por la altura para obtener el tamaño real de pixeles
        if results1.pose_landmarks:
            for idx, lm in enumerate(results1.pose_landmarks.landmark):
                keypoints["cam1"][idx] = [lm.x * WIDTH, lm.y * HEIGHT]
        # se obtinenen los keypoints de la segunda cámara
        if results2.pose_landmarks:
            for idx, lm in enumerate(results2.pose_landmarks.landmark):
                keypoints["cam2"][idx] = [lm.x * WIDTH, lm.y * HEIGHT]

        # Se guardan los keypoints en memoria
        json_data = json.dumps(keypoints)
        mem_kp.buf[:len(json_data)] = json_data.encode('utf-8')

        cv2.waitKey(1)  # Espera pequeña para sincronización

# Lanzar hilos para capturar imágenes en paralelo
threading.Thread(target=capture_camera, args=(0, "mem_cam1"), daemon=True).start()
threading.Thread(target=capture_camera, args=(1, "mem_cam2"), daemon=True).start()
threading.Thread(target=process_pose, daemon=True).start()

# Mantener el proceso en ejecución
while True:
    pass
