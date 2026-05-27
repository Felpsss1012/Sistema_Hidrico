// ============================================================================
// PROJETO DE EXTENSÃO: SISTEMA DE RESERVA HÍDRICA - FAZENDA UNIFEOB
// GRUPO: 07
// MATÉRIA: Projeto Integrado - P.I
// ============================================================================

// ── 1. DEFINIÇÕES DE HARDWARE E CONFIGURAÇÕES ────────────────────────────────
const int umidadePin = A0;
const int trigPin    = 9;
const int echoPin    = 10;

const float DIST_VAZIA = 30.0; // Nível 0%
const float DIST_CHEIA =  5.0; // Nível 100%

unsigned long lastMillis = 0;
const long INTERVALO = 2000;   // Delay de 2 segundos

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Controle de tempo para não travar o processamento
  if (millis() - lastMillis >= INTERVALO) {
    lastMillis = millis();

    // ── 2. MÓDULO DE ENTRADA: LEITURA DOS SENSORES ──────────────────────────
    
    // Processamento do Sensor Ultrassônico (Nível)
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duracao  = pulseIn(echoPin, HIGH, 30000);
    float distCm  = duracao * 0.034 / 2.0;

    float nivel = 0.0;
    if (duracao > 0) {
      nivel = (DIST_VAZIA - distCm) / (DIST_VAZIA - DIST_CHEIA) * 100.0;
      nivel = constrain(nivel, 0.0, 100.0);
    }

    // Processamento do Sensor de Umidade do Solo
    int rawUmidade = analogRead(umidadePin);
    float umidade = map(rawUmidade, 1023, 200, 0, 100);
    umidade = constrain(umidade, 0.0, 100.0);


    // ── 3. MÓDULO DE PROCESSAMENTO: REGRAS DE NEGÓCIO ───────────────────────
    
    String statusMsg;
    if (nivel < 20.0) {
      statusMsg = "CRITICO: Nivel da caixa muito baixo!";
    } else if (nivel < 40.0) {
      statusMsg = "ATENCAO: Nivel da caixa abaixo do ideal";
    } else if (umidade < 30.0) {
      statusMsg = "ATENCAO: Solo muito seco";
    } else {
      statusMsg = "Todos os niveis estao dentro do ideal";
    }


    // ── 4. MÓDULO DE SAÍDA: COMUNICAÇÃO EXTERNA ─────────────────────────────
    
    // Envia os dados formatados em JSON para o Python
    Serial.print("{");
    Serial.print("\"nivel\":");    Serial.print(nivel, 1);
    Serial.print(",\"umidade\":"); Serial.print(umidade, 1);
    Serial.print(",\"status\":\""); Serial.print(statusMsg);
    Serial.println("\"}");
  }
}