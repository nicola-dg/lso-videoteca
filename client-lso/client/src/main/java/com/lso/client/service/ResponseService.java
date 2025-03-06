package com.lso.client.service;

import java.io.IOException;
import java.util.List;

import org.springframework.stereotype.Service;

import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.lso.client.DTO.FilmDTO;

@Service
public class ResponseService {

    private final ObjectMapper objectMapper = new ObjectMapper();

    public class Response {
        String statusCode;
        String phrase;
        String payload;

        public String getStatusCode() {
            return this.statusCode;
        }

        public void setStatusCode(String statusCode) {
            this.statusCode = statusCode;
        }

        public String getPhrase() {
            return this.phrase;
        }

        public void setPhrase(String phrase) {
            this.phrase = phrase;
        }

        public String getPayload() {
            return this.payload;
        }

        public void setPayload(String payload) {
            this.payload = payload;
        }

        @Override
        public String toString() {
            return "Status Code: " + getStatusCode() +"\nPhrase: " + getPhrase() + "\nPayload: " + getPayload();
        }

    }

    public Response parseResponse(String json) {
        try {
            JsonNode root = objectMapper.readTree(json);
            ResponseService responseService = new ResponseService();
            Response response = new Response();
            
            response.setStatusCode(root.path("Status Code").asText());
            response.setPhrase(root.path("Phrase").asText());
            response.setPayload(root.path("Payload").asText());

            return response;
        } catch (Exception e) {
            throw new RuntimeException("Errore nel parsing della risposta JSON", e);
        }
    }

    public List<FilmDTO> parseFilms(String jsonPayload) throws IOException {
            ObjectMapper objectMapper = new ObjectMapper();
            return objectMapper.readValue(jsonPayload, new TypeReference<List<FilmDTO>>() {});
    }


}
