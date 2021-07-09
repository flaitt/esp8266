#include <ESP8266WiFi.h>       // Importa a Biblioteca ESP8266WiFi
#include <PubSubClient.h>      // Importa a Biblioteca PubSubClient
#include <ESP8266WebServer.h>  // Importa a Biblioteca ESP8266WebServer
 
//defines:
//defines de id mqtt e tópicos para publicação e subscribe
String TOPICO_SUBSCRIBE = "Sender";     //tópico MQTT de escuta
#define TOPICO_PUBLISH   "javaTopic"     //tópico MQTT de envio de informações para Broker
boolean state_changed = false;
//int ip_address_device = 0;

String ID_MQTT  ="";
 
//defines - mapeamento de pinos do NodeMCU

#define IO0    0
#define IO2    2
#define IO4    4
#define IO5    5
#define IO9    9
#define IO10   10
#define IO12   12
#define IO13   13
#define IO14   14
#define IO15   15
#define IO16   16

#define HTTP_REST_PORT 8080

int alreadySet = 0;

ESP8266WebServer httpRestServer(HTTP_REST_PORT);
 
 
// WIFI
const char* SSID = ""; // "VIVOFIBRA-019F"; // SSID / nome da rede WI-FI que deseja se conectar
const char* PASSWORD = ""; // "EAEAC6019F"; // Senha da rede WI-FI que deseja se conectar
  
// MQTT
const char* BROKER_MQTT = "snake.rmq2.cloudamqp.com"; //URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 1883; // Porta do Broker MQTT
const char* USERNAME = "zylppjqz:zylppjqz";
const char* PASSWORD2 = "bxe7Ktszi35KO9-4RHhP2pVhypiPWVk2";
 
 
//Variáveis e objetos globais
WiFiClient espClient; // Cria o objeto espClient
PubSubClient MQTT(espClient); // Instancia o Cliente MQTT passando o objeto espClient
char EstadoSaida = '0';  //variável que armazena o estado atual da saída
int ap_mode = 1;
  
//Prototypes
void initSerial();
void initWiFi();
void initMQTT();
void reconectWiFi(); 
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void VerificaMQTT(void);
void InitOutput(void);
void ICACHE_RAM_ATTR ISR(void);
 
/* 
 *  Implementações das funções
 */

 
void setup() 
{
    //inicializações:
    pinMode(D1,OUTPUT);
    WiFi.disconnect(true);
    InitOutput();
    initSerial();
    if(!alreadySet) {
        boolean result = WiFi.softAP("ESPsoftAP_01", "pass-to-soft-AP");
        if(result) {
          Serial.println("\nReady");
          }
        else {
          Serial.println("\nFailed!");
        }
    }
    // initWiFi();
    initMQTT();
    restServerRouting();
    httpRestServer.begin();
    httpRestServer.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
    attachInterrupt(digitalPinToInterrupt(IO14), ISR, CHANGE);
}

 
//Função: Rotina de interrupção
//Parâmetros: nenhum
//Retorno: nenhum
void ICACHE_RAM_ATTR ISR(void){
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  digitalWrite(IO16, !digitalRead(IO16));
  EstadoSaida = (EstadoSaida == '1')?'0':'1';
  state_changed = true;
  Serial.println((EstadoSaida == '1')?"L":"D");
}


void getHelloWord() {
    httpRestServer.send(200, "application/json", "{\"name\": \"Hello world\"}");
}
 
void restServerRouting() {
    httpRestServer.on("/", HTTP_GET, []() {
        httpRestServer.send(200, F("text/html"),
            F("Welcome to the REST Web Server"));
    });
    httpRestServer.on(F("/helloWorld"), HTTP_GET, getHelloWord);
    httpRestServer.on(F("/connect"), HTTP_GET, [](){

        SSID = httpRestServer.arg("rede").c_str();
        PASSWORD = httpRestServer.arg("senha").c_str();
        TOPICO_SUBSCRIBE = httpRestServer.arg("routingKey").c_str();
        ID_MQTT = TOPICO_SUBSCRIBE;
        String deviceName = httpRestServer.arg("deviceName").c_str();
        String status = httpRestServer.arg("status").c_str();
        String environment = httpRestServer.arg("environment").c_str();
        String user = httpRestServer.arg("user").c_str();
        //ip_address_device = httpRestServer.arg("endIp").c_str();
        //print(TOPICO_SUBSCRIBE);
        httpRestServer.send(200, F("application/json"),
        F("{\"status\": \"Conectado\"}"));
        delay(3000);
        WiFi.softAPdisconnect (true);
        initWiFi();
        VerificaMQTT();
        EnviaParaBancoDeDados(deviceName, status, environment, user, "newDevice");
        ap_mode = 0;
    });
}
//Função: inicializa comunicação serial com baudrate 115200 (para fins de monitorar no terminal serial 
//        o que está acontecendo.
//Parâmetros: nenhum
//Retorno: nenhum
void initSerial() 
{
    Serial.begin(115200);
}
 
//Função: inicializa e conecta-se na rede WI-FI desejada
//Parâmetros: nenhum
//Retorno: nenhum
void initWiFi() 
{
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
     
    reconectWiFi();
}
  
//Função: inicializa parâmetros de conexão MQTT(endereço do 
//        broker, porta e seta função de callback)
//Parâmetros: nenhum
//Retorno: nenhum
void initMQTT() 
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);   //informa qual broker e porta deve ser conectado
    MQTT.setCallback(mqtt_callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

//Função: função de callback 
//        esta função é chamada toda vez que uma informação de 
//        um dos tópicos subescritos chega)
//Parâmetros: nenhum
//Retorno: nenhum
void mqtt_callback(char* topic, byte* payload, unsigned int length) 
{
    String msg;
 
    //obtem a string do payload recebido
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
    }
    Serial.println("Recebi mensagem");
   
    //toma ação dependendo da string recebida:
    //verifica se deve colocar nivel alto de tensão na saída D0:
    //IMPORTANTE: o Led já contido na placa é acionado com lógica invertida (ou seja,
    //enviar HIGH para o output faz o Led apagar / enviar LOW faz o Led acender)
    if (msg.equals("L"))
    {
        digitalWrite(D1,LOW);
        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(IO16, HIGH);
        EstadoSaida = '1';
        Serial.println("L");
        String deviceName = getValue(ID_MQTT, '_', 0);
        String environment = getValue(ID_MQTT, '_', 1);
        String user = getValue(ID_MQTT, '_', 3);
        Serial.println("deviceName" + deviceName);
        Serial.println("env" + environment);
        Serial.println("user" + user);
        EnviaParaBancoDeDados(deviceName, "on", environment, user, "updateStatus");
    }
 
    //verifica se deve colocar nivel alto de tensão na saída D0:
    if (msg.equals("D"))
    {
        String deviceName = getValue(ID_MQTT, '_', 0);
        String environment = getValue(ID_MQTT, '_', 1);
        String user = getValue(ID_MQTT, '_', 3);
        EnviaParaBancoDeDados(deviceName, "off", environment, user, "updateStatus");
        digitalWrite(D1,HIGH);
        digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(IO16, LOW);
        EstadoSaida = '0';
        Serial.println("D");
    }
     
}
  
//Função: reconecta-se ao broker MQTT (caso ainda não esteja conectado ou em caso de a conexão cair)
//        em caso de sucesso na conexão ou reconexão, o subscribe dos tópicos é refeito.
//Parâmetros: nenhum
//Retorno: nenhum
void reconnectMQTT() 
{
    if(!MQTT.connected()) 
    {
        // Serial.print("* Tentando se conectar ao Broker MQTT: ");
        // Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT.c_str(), USERNAME, PASSWORD2)) 
        {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE.c_str()); 
        } 
        else
        {
            // Serial.println("Falha ao reconectar no broker.");
            // Serial.println("Havera nova tentatica de conexao em 2s");
            delay(2000);
        }
    }
}
  
//Função: reconecta-se ao WiFi
//Parâmetros: nenhum
//Retorno: nenhum
void reconectWiFi() 
{
    //se já está conectado a rede WI-FI, nada é feito. 
    //Caso contrário, são efetuadas tentativas de conexão
    if (WiFi.status() == WL_CONNECTED)
        return;
         
    WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI
    //IPAddress ip(192, 168, 0, ip_address_device);
    //IPAddress gateway(192, 168, 0, 254);
    //IPAddress subnet(255, 255, 255, 0);
    //WiFi.config(ip, gateway, subnet);
     
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(100);
        Serial.print(".");
    }
   
    Serial.println();
    Serial.print("Conectado com sucesso na rede ");
    Serial.print(SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());
}
 
//Função: verifica o estado das conexões WiFI e ao broker MQTT. 
//        Em caso de desconexão (qualquer uma das duas), a conexão
//        é refeita.
//Parâmetros: nenhum
//Retorno: nenhum
void VerificaMQTT(void)
{
    if (!MQTT.connected()) 
        reconnectMQTT(); //se não há conexão com o Broker, a conexão é refeita
     
     // reconectWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}
 
//Função: envia ao Broker o estado atual do output 
//Parâmetros: nenhum
//Retorno: nenhum
void EnviaEstadoOutputMQTT(void)
{
    if (EstadoSaida == '0')
      MQTT.publish(TOPICO_PUBLISH, ("{\"nome\": \""+ID_MQTT+"\",\"comando\": \"L\"}").c_str());
 
    if (EstadoSaida == '1')
      MQTT.publish(TOPICO_PUBLISH, ("{\"nome\": \""+ID_MQTT+"\",\"comando\": \"D\"}").c_str());

    state_changed = false;
    delay(300);
}

void EnviaParaBancoDeDados(String name, String status, String environment, String user, String action) {
    MQTT.publish(TOPICO_PUBLISH, ("{\"name\": \""+name+"\",\"status\": \""+status+"\",\"environment\": \""+environment+"\",\"user\": \""+user+"\",\"action\": \""+action+"\"}").c_str());
}


 
//Função: inicializa o output em nível lógico baixo
//Parâmetros: nenhum
//Retorno: nenhum
void InitOutput(void)
{
    //IMPORTANTE: o Led já contido na placa é acionado com lógica invertida (ou seja,
    //enviar HIGH para o output faz o Led apagar / enviar LOW faz o Led acender)
    pinMode(LED_BUILTIN, OUTPUT);         //LED do ESP
    pinMode(IO16, OUTPUT);                //Relé da placa
    pinMode(IO12, OUTPUT);                //Sinal Alto para detecção de borda do interruptor
    pinMode(IO14, INPUT_PULLUP);          //entrada que vai detectar a mudança de estado do interruptor
    
    digitalWrite(LED_BUILTIN, HIGH);  
    digitalWrite(IO14, LOW);  
    digitalWrite(IO16, LOW); 
    digitalWrite(IO12, LOW);           

}


//programa principal
void loop() 
{   
    //garante funcionamento das conexões WiFi e ao broker MQTT
    VerificaMQTT();

    if (!ap_mode) {
      reconectWiFi();
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("Ap Mode Active");
      Serial.println(WiFi.localIP());
      Serial.println(WiFi.softAPIP());
    }
 
    //envia o status de todos os outputs para o Broker no protocolo esperado
    if(state_changed)EnviaEstadoOutputMQTT();
 
    //keep-alive da comunicação com broker MQTT
    MQTT.loop();

    httpRestServer.handleClient();
}
