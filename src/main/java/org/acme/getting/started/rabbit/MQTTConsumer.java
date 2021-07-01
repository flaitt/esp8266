package org.acme.getting.started.rabbit;

import javax.enterprise.context.ApplicationScoped;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

import io.quarkus.runtime.Startup;

@Startup
@ApplicationScoped
public class MQTTConsumer {
    MQTTConsumer() {
        String topic        = "javaTopic"; //routing-key
        String content      = "Hello CloudAMQP";
        int qos             = 1;
        String broker       = "tcp://localhost:1884";

        //MQTT client id to use for the device. "" will generate a client id automatically
        String clientId     = "ClientId"; //para dar o nome para fila

        MemoryPersistence persistence = new MemoryPersistence();
        try {
            MqttClient mqttClient = new MqttClient(broker, clientId, persistence);
            mqttClient.setCallback(new MqttCallback() {
              public void messageArrived(String topic, MqttMessage msg)
                        throws Exception {
                            System.out.println("Recived:" + topic);
                            System.out.println("Recived:" + new String(msg.getPayload()));
                            MqttMessage message = new MqttMessage(new String(msg.getPayload()).getBytes());
                            message.setQos(qos);
                            System.out.println("Publish message: " + message);
                            mqttClient.publish("inTopic", message);
                      }
                  
                public void deliveryComplete(IMqttDeliveryToken arg0) {
                  System.out.println("Delivary complete");
                }

                public void connectionLost(Throwable arg0) {
                  // TODO Auto-generated method stub
                }
            });

            MqttConnectOptions connOpts = new MqttConnectOptions();
            connOpts.setCleanSession(true);
            connOpts.setUserName("rabbit");
            connOpts.setPassword(new char[]{'g', 'u', 'e', 's', 't'});
            mqttClient.connect(connOpts);
            MqttMessage message = new MqttMessage(content.getBytes());
            message.setQos(qos);
            System.out.println("Publish message: " + message);
            mqttClient.subscribe(topic, qos);
            mqttClient.publish(topic, message);
        } catch(MqttException me) {
            System.out.println("reason "+me.getReasonCode());
            System.out.println("msg "+me.getMessage());
            System.out.println("loc "+me.getLocalizedMessage());
            System.out.println("cause "+me.getCause());
            System.out.println("excep "+me);
            me.printStackTrace();
        }
    }
}
