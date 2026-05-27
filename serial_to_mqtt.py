import serial
import json
import time
import paho.mqtt.client as mqtt

# --- Configurações ---
BROKER = "localhost" 
PORTA_SERIAL = "/dev/ttyACM0" # Se não funcionar, tente /dev/ttyUSB0
BAUD = 115200

# Tópicos (Devem ser iguais aos que o Flutter está ouvindo)
TOPIC_NIVEL = "unifeob/fazenda/nivel"
TOPIC_UMIDADE = "unifeob/fazenda/umidade_solo"
TOPIC_STATUS = "unifeob/fazenda/status"

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("[MQTT] Conectado ao broker!")
    else:
        print(f"[MQTT] Falha na conexão. Código: {rc}")

# Inicializa Cliente MQTT - Usando API v1 para evitar erros de versão
client = mqtt.Client() 
client.on_connect = on_connect
client.connect(BROKER, 1883, 60)
client.loop_start()

print(f"[Serial] Tentando abrir {PORTA_SERIAL}...")

try:
    ser = serial.Serial(PORTA_SERIAL, BAUD, timeout=2)
    time.sleep(2) # Aguarda o Arduino resetar
    ser.reset_input_buffer()
    print(f"[Serial] Conectado com sucesso!")
except Exception as e:
    print(f"[Erro Serial] {e}")
    exit(1)

try:
    while True:
        if ser.in_waiting > 0:
            linha = ser.readline().decode('utf-8', errors='ignore').strip()
            
            if not linha:
                continue

            try:
                # O Arduino envia um JSON, o Python decodifica:
                dados = json.loads(linha)
                
                nivel = dados.get("nivel", 0)
                umidade = dados.get("umidade", 0)
                status_msg = dados.get("status", "")

                # Publica apenas o valor (String) para o Flutter processar
                client.publish(TOPIC_NIVEL, str(nivel), qos=1, retain=True)
                client.publish(TOPIC_UMIDADE, str(umidade), qos=1, retain=True)
                client.publish(TOPIC_STATUS, status_msg, qos=1, retain=True)

                print(f"[OK] Enviado -> Nível: {nivel}% | Umidade: {umidade}%")

            except json.JSONDecodeError:
                print(f"[Aviso] Dados inválidos: {linha}")
        
        time.sleep(0.1)

except KeyboardInterrupt:
    print("\nParando...")
    ser.close()
    client.loop_stop()
    client.disconnect()