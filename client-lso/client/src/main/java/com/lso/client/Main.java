package com.lso.client;

import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.stereotype.Component;

import com.lso.client.DTO.UserDTO;
import com.lso.client.service.RequestService;
import com.lso.client.service.ResponseService;
import com.lso.client.service.RequestService.Request;
import com.lso.client.types.Header;
import com.lso.client.service.ResponseService.Response;
import com.lso.client.service.SocketClient;
import com.lso.client.types.Method;

public class Main {

    public static void main(String[] args) {

        try {
            ResponseService responseService = new ResponseService();
            SocketClient userSocket = new SocketClient();
            RequestService requestService = new RequestService(userSocket, responseService);

            Response response = requestService
            .sendRequest(
            requestService.createRequest()
            .setMethod(Method.GET)
            .setPath("/user")
            .setPayload("{\\\"random\\\":\\\"paylaod\\\"}")
            .setHeader(new Header("Authorization", "jwt"))
            );

            

        } catch (Exception e) {
            System.out.println("error nel main");
        }
    }
}
