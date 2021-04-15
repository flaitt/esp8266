package org.acme.getting.started;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;

import com.rabbitmq.client.Channel;
import com.rabbitmq.client.Connection;
import com.rabbitmq.client.ConnectionFactory;

@Path("/")
public class GreetingResource {
    private final static String QUEUE_NAME = "hello";

    @GET
    @Path("hello")
    @Produces(MediaType.TEXT_PLAIN)
    public String hello() {
        ConnectionFactory factory = new ConnectionFactory();
        
        factory.setHost("localhost");
        factory.setUsername("guest");
        factory.setPassword("guest");
        
        try {
            Connection connection = factory.newConnection();
            Channel channel = connection.createChannel();
            channel.queueDeclare(QUEUE_NAME, false, false, false, null);
            String message = "Hello World";
            channel.basicPublish("", QUEUE_NAME, null, message.getBytes());
            System.out.println(" [x] Sent '" + message + "'");

        } catch(Exception e) {
            System.out.println("deu ruim");
        }
        return "Hello RESTEasy";
    }

    @Path("publish")
    @GET
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.TEXT_PLAIN)
    public String publishESP8266(@QueryParam("message") String message) {
        ConnectionFactory factory = new ConnectionFactory();
        factory.setHost("localhost");
        factory.setUsername("guest");
        factory.setPassword("guest");

        try {
            Connection connection = factory.newConnection();
            Channel channel = connection.createChannel();
            channel.basicPublish("test-exchange", "inTopic", null, message.getBytes());
            return "[SUCESSO] Mensagem enviada para ESP8266";
        } catch (Exception e) {
            return "[ERRO] Mensagem não enviada para ESP8266";
        }
    }
}