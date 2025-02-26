package com.lso.client;

import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.stereotype.Component;

import com.lso.client.service.RequestService;
import com.lso.client.service.ResponseService;
import com.lso.client.service.RequestService.Request;
import com.lso.client.service.ResponseService.Response;
import com.lso.client.service.SocketClient;
import com.lso.client.types.Method;

public class Main {

    public static void main(String[] args) {

        try {
            ResponseService responseService = new ResponseService();
            SocketClient userSocket = new SocketClient();
            RequestService requestService = new RequestService(userSocket, responseService);

            // Response response = requestService
            // .sendRequest(
            // requestService.createRequest()
            // .setMethod(Method.GET)
            // .setPath("/user")
            // .setPayload("{\\\"random\\\":\\\"paylaod\\\"}"));

            // System.out.println("response status-code: " + response.getStatusCode());
            // System.out.println("response status-phrase: " + response.getPhrase());
            // System.out.println("response payload: " + response.getPayload());

            // System.out.println(responseService.parseResponse(userSocket.sendRequest(requestService.createRequest()
            //                                                                                         .setMethod(Method.GET)
            //                                                                                         .setPath("/user")
            //                                                                                         .setPayload("{\\\"random\\\":\\\"paylaod\\\"}")
            //                                                                                         .toString())));


            System.out.println(requestService.sendRequest(requestService.createRequest()
                                                        .setMethod(Method.GET)
                                                        .setPath("/user")
                                                        .setPayload("{\\\"random\\\":\\\"paylaod\\\"}")));

        } catch (Exception e) {
            System.out.println("error");
        }
    }
}
