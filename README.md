# 🔌 Reservas Hídricas - Módulo de Hardware & Gateway IoT

<p align="center">
  <img src="https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white" alt="Arduino">
  <img src="https://img.shields.io/badge/Python-3776AB?style=for-the-badge&logo=python&logoColor=white" alt="Python">
  <img src="https://img.shields.io/badge/MQTT-Eclipse%20Mosquitto-3C525C?style=for-the-badge&logo=eclipse-mosquitto&logoColor=white" alt="Mosquitto">
</p>

Este repositório contém o **Firmware do microcontrolador (Arduino Uno R3)** e o **Script de Gateway (Python)** desenvolvidos para o sistema de monitoramento e gestão de recursos hídricos da **Fazenda-Escola da UNIFEOB**. 

Esses componentes são responsáveis por realizar a leitura dos sensores em campo, processar os dados iniciais e transmiti-los via protocolo MQTT para que o aplicativo mobile em Flutter consuma as informações em tempo real.

---

## 👥 Integrantes (Grupo 07)
* **Felipe dos Santos Ferrari** — RA: 26000728
* **Isabelly Amaral Custódio da Silva** — RA: 26001064
* **Marco Gabriel Verotti dos Santos** — RA: 26000919
* **Sarah Alves de Souza Lima** — RA: 26001366
* **Victor Gabriel Tortelo da Silva** — RA: 26000953

---

## 🏗️ Arquitetura de Conexão Física

O hardware opera coletando variações elétricas no campo, empacotando os resultados e despachando-os localmente por comunicação serial (UART) via cabo USB até o Gateway de borda.

```text
 [Sensor Ultrassônico HC-SR04] ──┐
 [Higrômetro de Solo A0]       ──┼─> [Arduino Uno R3] ──(USB-Serial JSON)──> [Raspberry Pi / PC] ──(MQTT)──> Broker Mosquitto
 [Módulo Relé 5V]              ──┘
