import cv2
import mediapipe as mp
import numpy as np
import json
import time
import os
import atexit
import struct
import mmap
import posix_ipc

class VideoCapture:


    def __init__(self, camera_index=0):
        #incializamos la clase


        self.camera_index = camera_index
        self.running = False

        # Cargar configuración
        config = self.load_config()
        if(camera_index==0):
            self.shm_name = config.get("CAM1", "/cam1")
        else:
            self.shm_name = config.get("CAM2", "/cam2")

        # Definir ruta del archivo .ready
        self.APP_DIR = os.path.dirname(os.path.abspath(__file__))
        self.ready_path = os.path.join(self.APP_DIR, self.shm_name[1:] + ".ready")


        self.WIDTH = config.get("WIDTH", 640)
        self.HEIGHT = config.get("HEIGHT", 480)
        self.JSON_SIZE = config.get("JSON_SIZE", 4096)
        self.FLAG_SIZE = 4
        self.FRAME_SIZE = self.WIDTH * self.HEIGHT * 3  # Tamaño de la imagen en RGB
        self.TOTAL_SIZE = self.FRAME_SIZE + self.JSON_SIZE
        #self.TOTAL_SIZE = 512
        if camera_index == 0:
            self.SEM_SHM = config.get("SEM_SHM1", "/semShm1")
        else:
            self.SEM_SHM = config.get("SEM_SHM2", "/semShm2")
        #self.sem = posix_ipc.Semaphore( self.SEM_SHM)
        # Crear semáforo
        print("Escritor> Creando el semáforo...")
        self.sem = posix_ipc.Semaphore(self.SEM_SHM , posix_ipc.O_CREAT, initial_value=1)

        # Imprimir configuración cargada
        print( "=== Configuración Cargada ===")
        print ("SHM: " + self.shm_name)
        print ("WIDTH: " + str(self.WIDTH))
        print ("HEIGHT: " + str(self.HEIGHT))
        print ("JSON_SIZE: " + str(self.JSON_SIZE))
        print ("FLAG_SIZE: " + str(self.FLAG_SIZE))
        print ("FRAME_SIZE: " + str(self.FRAME_SIZE))
        print ("TOTAL_SIZE: " + str(self.TOTAL_SIZE))
        print ("SEM_SHM: " + self.SEM_SHM)
        print ("=============================")


        # Configurar MediaPipe Pose
        self.pose = mp.solutions.pose.Pose()

        # Crear memoria compartida
        self.mem = self.openMem(self.shm_name)

        # Asegurar limpieza al finalizar
        #atexit.register(self.cleanup)

    # Cargar la configuración desde el archivo JSON
    def load_config(self):
        # Obtener la ruta del directorio donde está el script
        script_dir = os.path.dirname(os.path.abspath(__file__))

        # Crear la ruta del archivo dentro de ese directorio
        json_path = os.path.join( script_dir , "config/poseConfig.json")
        print( "ruta del archivo ce configuración"+json_path  )

        try:
            with open(json_path, "r") as file:
                config = json.load(file)
                return config


        except FileNotFoundError:
            print(" Python>Error: No se encontró el archivo config.json en "+json_path)
            exit(1)
        except json.JSONDecodeError:
            print("Python>Error: El archivo config.json tiene un formato inválido")
            exit(1)

    #función que inicializa la memoria compartida
    def openMem(self,shm_name):
        while True:
                try:
                    #shm_path = f"/tmp{shm_name}"
                    #fd = os.open(shm_path, os.O_CREAT | os.O_RDWR)
                    #os.ftruncate(fd, self.TOTAL_SIZE)  # Ajustar tamaño de la memoria
                    # Mapear la memoria compartida
                    #mem_map = mmap.mmap(fd, self.TOTAL_SIZE, mmap.MAP_SHARED, mmap.PROT_WRITE)

                    # Crear memoria compartida POSIX correctamente
                    #self.mem = posix_ipc.SharedMemory(self.shm_name, posix_ipc.O_CREAT, size=self.TOTAL_SIZE)
                    try:
                        self.mem = posix_ipc.SharedMemory(self.shm_name, posix_ipc.O_CREAT | posix_ipc.O_EXCL, size=self.TOTAL_SIZE)
                    except posix_ipc.ExistentialError:
                        self.mem = posix_ipc.SharedMemory(self.shm_name)  # Abrir sin size si ya existe

                    mem_map = mmap.mmap( self.mem.fd, self.TOTAL_SIZE, mmap.MAP_SHARED, mmap.PROT_WRITE)
                    self.mem.close_fd()

                    print(f"Python> Memoria compartida {shm_name} conectada correctamente.")

                    return mem_map
                except FileNotFoundError:
                    print("Python> Esperando a que C++ cree la memoria compartida...")
                    time.sleep(1)


    #función que lee la imagende la la cámara
    def start(self):

        cap = cv2.VideoCapture(self.camera_index)
        created_ready = False
        if not cap.isOpened():
            print(f"Error: No se pudo abrir la cámara {self.camera_index}")
            cap.release()  # Asegura que la cámara se libere
            return


        try:
            while True:
                ret, frame = cap.read()
                if not ret:
                    continue  # Si falla ,se intenta nuevamente

                # Convertir la imagen a RGB y escribirla en memoria compartida
                fullframe_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
                frame_rgb = cv2.resize(fullframe_rgb, (self.WIDTH, self.HEIGHT))
                np_frame = np.array(frame_rgb, dtype=np.uint8).flatten()

                frameRawData=np_frame.tobytes()


                # Obtenemos los timestamp en milisegundos para poder sincronizar los keypoints
                timestamp = int(time.time() * 1000)

                # Procesamos los keypoints con Mediapipe
                results = self.pose.process(frame_rgb)

                keypoints = {
                        "timestamp": timestamp,
                        "keypoints": {}
                }

                #if results.pose_landmarks:
                #    for idx, lm in enumerate(results.pose_landmarks.landmark):
                #        keypoints["keypoints"][idx] = [lm.x * self.WIDTH, lm.y * self.HEIGHT]

                if results.pose_landmarks:
                    for idx, lm in enumerate(results.pose_landmarks.landmark):
                        keypoints["keypoints"][idx] = {
                            "x": lm.x, #* self.WIDTH,
                            "y": lm.y #* self.HEIGHT
                        }


                #creamos el JSON
                json_data = json.dumps(keypoints).encode('utf-8')


                #comprobamos que el los datos no son mas grandes que la memoria compartida
                if len(frameRawData) > self.FRAME_SIZE:
                    print(f"Python>Error: La imagen '{len(frameRawData)} es demasiado grande para la memoria compartida '{self.frame_size} ")
                    continue
                if len(json_data) > self.JSON_SIZE:
                    print("Python>Error: Los keypoints son demasiado grandes para la memoria compartida")
                    continue

                # Se guardan los keypoints en memoria compartida con formato JSON
                # Asegurar que el JSON ocupa exactamente self.JSON_SIZE bytes
                json_padded = json_data.ljust(self.JSON_SIZE, b'\x00')[:self.JSON_SIZE]
                # Asegurar que el JSON ocupa exactamente self.JSON_SIZE bytes
                json_padded = json_data.ljust(self.JSON_SIZE, b'\x00')[:self.JSON_SIZE]

                # Se guardan los datos en memoria compartida
                self.sem.acquire()

                #mensaje = f"Hola desde el proceso escritor! Mensaje en {int(time.time())}".encode('utf-8')
                #self.mem.seek(0)  # Mover al inicio de la memoria compartida
                #self.mem.write(mensaje.ljust( self.TOTAL_SIZE, b'\x00'))  # Escribir y rellenar con '\0'
                #print(f"Escritor> Mensaje escrito: {mensaje.decode('utf-8')}")

                # Escribir imagen en memoria compartida
                self.mem.seek(0)
                self.mem.write(frameRawData)
                # Escribir JSON en memoria compartida
                self.mem.seek(self.FRAME_SIZE)
                self.mem.write(json_padded)

                #vamos a ller los datos  escrita en la memoria compartida
                #self.mem.seek(0)
                #readFrameRawData = self.mem.read(self.FRAME_SIZE)

                #self.mem.seek(self.FRAME_SIZE)
                #json_raw = self.mem.read(self.JSON_SIZE)
                # Eliminamos los bytes nulos
                #json_str = json_raw.split(b'\x00', 1)[0].decode('utf-8')  # Eliminar bytes nulos


                #try:
                #    json_data = json.loads(json_str)
                #except json.JSONDecodeError:
                #    json_data = {"error": "No se pudo decodificar el JSON"}

                self.sem.release()

                #creamos unarchoivo para indicar que la alicaciones esta totalmente lista
                if not created_ready:
                    try:
                        with open(self.ready_path, "w") as f_ready:
                            f_ready.write("ready")
                        print(f"[Python] Archivo de señal creado: {self.ready_path}")
                        created_ready = True
                    except Exception as e:
                        print(f"[Python] Error al crear archivo .ready: {e}")

                #comprobamos los datos escritos
                print(f"Python> Escribiendo imagen de {len(frameRawData)} bytes y JSON de {len(json_padded)} bytes en memoria.")

               # Imprimir JSON en consola
               # print("JSON leído desde memoria compartida:", json.dumps(json_data, indent=4))

               # np_frame = np.frombuffer(readFrameRawData, dtype=np.uint8).reshape((self.HEIGHT, self.WIDTH, 3))
               # cv2.imshow("Imagen desde Memoria Compartida", np_frame)
               # cv2.waitKey(1)

        except KeyboardInterrupt:
            print("\nPython> Detenido manualmente.")
        finally:
            print("VideoCapture> Liberando recursos...")
            cap.release()
            self.cleanup()
            print("VideoCapture> Memoria cerrada correctamente (NO eliminada).")

    def cleanup(self):
        print("Liberando memoria compartida...")
        self.mem.close()
        self.sem.close()
        try:
            os.remove(self.ready_path)
            print(f"[Python] Archivo de señal eliminado: {self.ready_path}")
        except Exception as e:
            print(f"[Python] Error al eliminar archivo .ready: {e}")




if __name__ == "__main__":
    import sys
    cam_index = int(sys.argv[1]) if len(sys.argv) > 1 else 0
    video_capture = VideoCapture(camera_index=cam_index)
    video_capture.start()

