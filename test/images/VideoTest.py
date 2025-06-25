import mediapipe as mp
import cv2
import json
import sys
import os
import time

def extract_keypoints(image_path, output_json_path="output_keypoints.json"):
    # Inicializar MediaPipe
    mp_pose = mp.solutions.pose
    pose = mp_pose.Pose(static_image_mode=True)
    mp_drawing = mp.solutions.drawing_utils

    # VERIFICACIÓN MEJORADA
    print(f"[DEBUG] Ruta recibida: {image_path}")
    
    if not os.path.exists(image_path):
        print("❌ ERROR: La ruta no existe.")
        print("👉 Verifica si hay errores tipográficos, espacios sin comillas, o si el archivo fue movido.")
        raise FileNotFoundError(f"Ruta inválida: {image_path}")

    if not os.path.isfile(image_path):
        print("❌ ERROR: La ruta no es un archivo.")
        raise FileNotFoundError(f"Se esperaba un archivo de imagen: {image_path}")

    image = cv2.imread(image_path)
    if image is None:
        print("❌ ERROR: OpenCV no pudo cargar la imagen.")
        print("🔎 Posibles causas:")
        print("   - Extensión incorrecta (ej. .png cuando es .jpg)")
        print("   - Imagen corrupta")
        print("   - Codificación no soportada por OpenCV")
        raise FileNotFoundError(f"No se puede cargar la imagen con OpenCV: {image_path}")

    # Convertir BGR a RGB
    image_rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

    # Procesar imagen
    results = pose.process(image_rgb)

    if not results.pose_landmarks:
        print("⚠️ Advertencia: No se detectaron landmarks en la imagen.")
        return

    # Crear salida en formato JSON
    output = {
        "timestamp": int(time.time() * 1000),
        "keypoints": {}
    }

    for idx, landmark in enumerate(results.pose_landmarks.landmark):
        output["keypoints"][str(idx)] = {
            "x": landmark.x,
            "y": landmark.y,
            "z": landmark.z,
            "visibility": landmark.visibility
        }

    # Guardar archivo JSON
    with open(output_json_path, 'w') as f:
        json.dump(output, f, indent=2)

    print(f"✅ Keypoints guardados en: {output_json_path}")


# Uso desde línea de comandos
if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Uso: python VideoTest.py ruta_imagen [ruta_salida_json]")
        sys.exit(1)

    image_path = sys.argv[1]

    # Si no se especifica salida, usar el mismo nombre de archivo con .json en el mismo directorio
    if len(sys.argv) > 2:
        output_path = sys.argv[2]
    else:
        image_basename = os.path.basename(image_path)                # "1.png"
        name_wo_ext = os.path.splitext(image_basename)[0]            # "1"
        dir_name = os.path.dirname(image_path)                       # "/ruta/a/carpeta"
        output_path = os.path.join(dir_name, f"{name_wo_ext}.json")  # "/ruta/a/carpeta/1.json"

    extract_keypoints(image_path, output_path)