import cv2
import mediapipe as mp
import numpy as np
import json
import multiprocessing.shared_memory as shm
import sys
import time
import json

# Cargar la configuración desde el archivo JSON
def load_config():
    try:
        with open("config.json", "r") as file:
            config = json.load(file)
            return config
    except FileNotFoundError:
        print(" Python>Error: No se encontró el archivo config.json")
        exit(1)
    except json.JSONDecodeError:
        print("Python>Error: El archivo config.json tiene un formato inválido")
        exit(1)

#función que lee la imagende la la cámara
def capture_camera(camera_index):

    cap = cv2.VideoCapture(camera_index)

    if not cap.isOpened():
        print(f"Python>Error: No se pudo abrir la cámara {camera_index}")
        return

    while True:
        ret, frame = cap.read()
        if not ret:
            continue  # Si falla ,se intenta nuevamente

        # Convertir la imagen a RGB y escribirla en memoria compartida
        frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        np_frame = np.array(frame_rgb, dtype=np.uint8).flatten()


        frameRawData=np_frame.tobytes()
        #Guardamos la imagen en el buffer
        mem.buf[:FRAME_SIZE] = frameRawData

        # Obtenemos los timestamp en milisegundos para poder sincronizar los keypoints
        timestamp = int(time.time() * 1000)

        # Procesamos los keypoints con Mediapipe
        results = pose.process(frame_rgb)

        keypoints = {
                "timestamp": timestamp,
                "keypoints": {}
        }

        if results.pose_landmarks:
            for idx, lm in enumerate(results.pose_landmarks.landmark):
                keypoints["keypoints"][idx] = [lm.x * WIDTH, lm.y * HEIGHT]

        # Se guardan los keypoints en memoria compartida con formato JSON
        json_data = json.dumps(keypoints).encode('utf-8')
        mem.buf[FRAME_SIZE:FRAME_SIZE + len(json_data)] = json_data

        cv2.waitKey(1)


# Configuración de Mediapipe
mp_pose = mp.solutions.pose
pose = mp_pose.Pose()

#cargamos los datos desde el archivo JSON
config=load_config()

# Se obtiene el tama;o de la memoria compartida
WIDTH = config.get("WIDTH", 640)  # Valor por defecto si no existe la clave
HEIGHT = config.get("HEIGHT", 480)
FRAME_SIZE = config.get("FRAME_SIZE", WIDTH * HEIGHT * 3)
JSON_SIZE = config.get("JSON_SIZE", 4096)
TOTAL_SIZE = config.get("TOTAL_SIZE", FRAME_SIZE + JSON_SIZE)
CAM_1=config.get("CAM1","cam_1")
CAM_1=config.get("CAM2","cam_2")

# Se le pasa como argumento el índice de la camara.
if len(sys.argv) != 2:
    print("Python>Uso: python capture_instance.py <camera_index>")
    sys.exit(1)

camera_index = int(sys.argv[1])
camera_name = f"cam{camera_index}"

#Debemos pasar las imagenes y los keypoints a la aplicación. Por tanto creamos una memoria compartida para
#las imagenes y para los keypoints que luego recuperaremos.
mem = shm.SharedMemory( size=MEM_SIZE, name=f"mem_{camera_name}",create=True,)


#comenzamos a obtener poses
capture_camera(camera_index)
