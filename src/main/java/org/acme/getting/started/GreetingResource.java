package org.acme.getting.started;

import java.util.ArrayList;
import java.util.List;

import javax.inject.Inject;
import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;

import com.rabbitmq.client.Channel;
import com.rabbitmq.client.Connection;
import com.rabbitmq.client.ConnectionFactory;

import org.acme.getting.started.mongodb.Device;
import org.acme.getting.started.mongodb.Environment;
import org.acme.getting.started.mongodb.House;
import org.acme.getting.started.mongodb.SmarthomeService;

@Path("/")
public class GreetingResource {
    @Inject SmarthomeService service;

    private final static String QUEUE_NAME = "hello";

    // @GET
    // @Path("hello")
    // @Produces(MediaType.TEXT_PLAIN)
    // public String hello() {
    //     ConnectionFactory factory = new ConnectionFactory();
        
    //     factory.setHost("localhost");
    //     factory.setUsername("guest");
    //     factory.setPassword("guest");
        
    //     try {
    //         Connection connection = factory.newConnection();
    //         Channel channel = connection.createChannel();
    //         channel.queueDeclare(QUEUE_NAME, false, false, false, null);
    //         String message = "Hello World";
    //         channel.basicPublish("", QUEUE_NAME, null, message.getBytes());
    //         System.out.println(" [x] Sent '" + message + "'");

    //     } catch(Exception e) {
    //         System.out.println("deu ruim");
    //     }
    //     return "Hello RESTEasy";
    // }

    @Path("adddevice")
    @POST
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.TEXT_PLAIN)
    public String addDeviceToDatabase(@QueryParam("name") String name,
                                @QueryParam("status") String status,
                                @QueryParam("environment") String environment,
                                @QueryParam("user") String user) {
        Device device = new Device(name, status, environment, user);
        try {
            service.addDevice(device);
        } catch (Exception e) {
            System.out.println("Erro ao aidicionar objeto no mongo");
        }
        return "ok device";
    }

    @Path("addenvironment")
    @POST
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.TEXT_PLAIN)
    public String addEnvironemntToDatabase(@QueryParam("name") String name,
                                @QueryParam("house") String house,
                                @QueryParam("user") String user) {
        Environment environment = new Environment(name, house, user);
        try {
            service.addEnvironment(environment);
        } catch (Exception e) {
            System.out.println("Erro ao adicionar objeto no mongo");
        }
        return "ok environment";
    }

    @Path("addhouse")
    @POST
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.TEXT_PLAIN)
    public String addHouseToDatabase(@QueryParam("name") String name,
                                @QueryParam("user") String user) {
        House house = new House(name, user);
        try {
            service.addHouse(house);
        } catch (Exception e) {
            System.out.println("Erro ao adicionar objeto no mongo");
        }
        return "ok house";
    }

    // @Path("publish")
    // @GET
    // @Consumes(MediaType.APPLICATION_JSON)
    // @Produces(MediaType.TEXT_PLAIN)
    // public String publishESP8266(@QueryParam("message") String message) {
    //     ConnectionFactory factory = new ConnectionFactory();
    //     factory.setHost("localhost");
    //     factory.setUsername("guest");
    //     factory.setPassword("guest");

    //     try {
    //         Connection connection = factory.newConnection();
    //         Channel channel = connection.createChannel();
    //         channel.basicPublish("test-exchange", "inTopic", null, message.getBytes());
    //         return "[SUCESSO] Mensagem enviada para ESP8266";
    //     } catch (Exception e) {
    //         return "[ERRO] Mensagem não enviada para ESP8266";
    //     }
    // }

    @Path("device")
    @GET
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response devices(@QueryParam("user") String user, @QueryParam("environment") String environment) {
        List<Device> devices = new ArrayList<>();

        try {
            devices = service.getDevicesByUserAndEnvironment(user, environment);
            return Response.ok().header("Access-Control-Allow-Origin", "*").entity(devices).build();
        } catch (Exception e) {
            return Response.status(Status.INTERNAL_SERVER_ERROR).build();
        }
    }

    @Path("environment")
    @GET
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response environment(@QueryParam("user") String user, @QueryParam("house") String house) {
        List<Environment> environments = new ArrayList<>();

        try {
            environments = service.getEnvironmentByUserAndHouse(user, house);
            return Response.ok().entity(environments).build();
        } catch (Exception e) {
            return Response.status(Status.INTERNAL_SERVER_ERROR).build();
        }
    }

    @Path("house")
    @GET
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response houses(@QueryParam("user") String user) {
        List<House> houses = new ArrayList<>();

        try {
            houses = service.getHouseByUser(user);
            
            return Response.ok().entity(houses).build();
        } catch (Exception e) {
            return Response.status(Status.INTERNAL_SERVER_ERROR).build();
        }
    }


}