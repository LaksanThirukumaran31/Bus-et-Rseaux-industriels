import serial
import time
serialPort="/dev/ttyAMA0"
baudRate=115200
#ouverture connexion série
ser=serial.Serial(serialPort,baudRate,timeout=1)

def send_commandSTM32(command):
    ser.write(command.encode('utf-8'))
    time.sleep(0.1)  #attente réponse STM32 

def receive_response():
    response = ser.readline().decode('utf-8').strip()
    return response

send_command("GET_T")
response = receive_response()
print("Réponse du STM32:", response)

send_command("GET_P")
response = receive_response()
print("Réponse du STM32:", response)

send_command("SET_K=1234")  
response = receive_response()
print("Réponse du STM32:", response)

send_command("GET_K")
response = receive_response()
print("Réponse du STM32:", response)

#fermeture connexion série
ser.close()
