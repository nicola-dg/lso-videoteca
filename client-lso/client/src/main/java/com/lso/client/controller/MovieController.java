package com.lso.client.controller;

import com.lso.client.DTO.FilmDTO;
import com.lso.client.DTO.LoanDTO;
import com.lso.client.DTO.MessageDTO;
import com.lso.client.service.RequestService;
import com.lso.client.service.ResponseService;
import com.lso.client.service.ResponseService.Response;
import com.lso.client.service.SocketClient;
import com.lso.client.types.Header;
import com.lso.client.types.Method;

import jakarta.servlet.http.HttpSession;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;

import java.io.IOException;
import java.util.List;

@Controller
@RequestMapping("/")
public class MovieController {

    private RequestService requestService;
    private SocketClient socket;
    private ResponseService responseService;

    public MovieController(RequestService requestService, SocketClient socket, ResponseService responseService) {
        this.requestService = requestService;
        this.socket = socket;
        this.responseService = responseService;
    }


    
    @GetMapping("/home")
    public String getAllMovies(Model model, HttpSession session) {
        String jwt = (String) session.getAttribute("jwt");
        Response res = requestService
                .sendRequest(requestService.createRequest().setMethod(Method.GET).setPath("/film")
                        .setHeader(new Header("Authorization", "Bearer " + jwt)));
        try {
            if (res.getStatusCode().equals("200")) {
                List<FilmDTO> films = responseService.parseFilms(res.getPayload());
                films.forEach(System.out::println);
            } else if (res.getStatusCode().equals("404")) {
                System.out.println(res.getPayload());
            } else if (res.getStatusCode().equals("500")) {
                System.out.println(res.getPayload());
            } else if (res.getStatusCode().equals("403")) {
                System.out.println(res.getPayload());
            }
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        return "main";
    }

    @GetMapping("/add-movie")
    public String showAddMovieForm() {
        return "add-movie";
    }

    @PostMapping("/add-movie")
    public String addMovie(
            @RequestParam String title,
            @RequestParam String genre,
            @RequestParam double price,
            HttpSession session) {

        System.out.println("route add-movie attivata");
        String jwt = (String) session.getAttribute("jwt");
        FilmDTO film = new FilmDTO();
        film.setTitle(title);
        film.setGenre(genre);
        film.setPrice(price);
        System.out.println(film);
        Response res = requestService.sendRequest(requestService.createRequest()
                .setMethod(Method.POST)
                .setPath("/film")
                .setPayload(film.toJSON())
                .setHeader(new Header("Authorization", "Bearer " + jwt)));
        return "user-profile-test";

    }

    @GetMapping("/cart/film")
    public String showAddFilmToCart() {
        return "add-to-cart";
    }

    @PostMapping("/cart/film")
    public String addFilmToCart(HttpSession session, @RequestParam String title) {
        String jwt = (String) session.getAttribute("jwt");
        FilmDTO film = new FilmDTO();
        film.setTitle(title);
        Response res = requestService.sendRequest(requestService.createRequest()
                .setMethod(Method.POST)
                .setPath("/cart/film")
                .setPayload(film.toJSON())
                .setHeader(new Header("Authorization", "Bearer " + jwt)));

        return "user-profile-test";
    }

    @GetMapping("/loan") // L'UTENTE VEDE I NOLEGGI ATTIVI
    public String getLoans(Model model, HttpSession session) {
        String jwt = (String) session.getAttribute("jwt");
        Response res = requestService
                .sendRequest(requestService.createRequest().setMethod(Method.GET).setPath("/loan")
                        .setHeader(new Header("Authorization", "Bearer " + jwt)));
        try {
            List<LoanDTO> loans = responseService.parseLoans(res.getPayload());
            loans.forEach(System.out::println);
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        return "loans-page";
    }

    @GetMapping("/loan/film")
    public String showAddFilmToLoan() {
        return "add-to-loan";
    }

    @PostMapping("/loan/film") // IL FILM VIENE SPOSTATO DA CARRELLO A LOAN
    public String addFilmToLoan(HttpSession session, @RequestParam String id) {
        String jwt = (String) session.getAttribute("jwt");
        FilmDTO film = new FilmDTO();
        film.setId(id);
        Response res = requestService
                .sendRequest(requestService.createRequest().setMethod(Method.POST).setPath("/loan/film")
                        .setPayload(film.toJSON()).setHeader(new Header("Authorization", "Bearer " + jwt)));

        return "loans-page";
    }

    @GetMapping("/loan/film/return")
    public String showReturnFilmLoan() {
        return "return-film-loan";
    }

    @PostMapping("/loan/film/return")
    public String returnFilmLoan(HttpSession session, @RequestParam String id) {
        String jwt = (String) session.getAttribute("jwt");
        FilmDTO film = new FilmDTO();
        film.setId(id);
        Response res = requestService
                .sendRequest(requestService.createRequest().setMethod(Method.PUT).setPath("/loan/film")
                        .setPayload(film.toJSON()).setHeader(new Header("Authorization", "Bearer " + jwt)));

        return "user-profile-test";
    }

    @GetMapping("/loan/expire")
    public String returnFilmLoan(HttpSession session) {
        String jwt = (String) session.getAttribute("jwt");
        Response res = requestService
                .sendRequest(requestService.createRequest().setMethod(Method.GET).setPath("/loan/expire")
                        .setHeader(new Header("Authorization", "Bearer " + jwt)));

        try {
            List<LoanDTO> loans = responseService.parseLoans(res.getPayload());
            loans.forEach(System.out::println);
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        return "loan-expire";
    }

    /* FUNZIONI DA SPOSTARE IN MESSAGE CONTROLLER */

    @GetMapping("/message")
    public String showMessages(HttpSession session) {
        String jwt = (String) session.getAttribute("jwt");
        Response res = requestService
                .sendRequest(requestService.createRequest().setMethod(Method.GET).setPath("/message")
                        .setHeader(new Header("Authorization", "Bearer " + jwt)));
        try {
            if (res.getStatusCode().equals("200")) {
                List<MessageDTO> messages = responseService.parseMessages(res.getPayload());
                messages.forEach(System.out::println);
            }
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        return "send-message";
    }

    @GetMapping("/send-message")
    public String showSendMessage() {
        return "send-message";
    }

    @PostMapping("/message")
    public String sendMessage(HttpSession session, @RequestParam String user_id, @RequestParam String text) {
        String jwt = (String) session.getAttribute("jwt");
        MessageDTO message = new MessageDTO();
        message.setUserId(user_id);
        message.setText(text);
        Response res = requestService.sendRequest(requestService.createRequest()
                .setMethod(Method.POST)
                .setPath("/message")
                .setPayload(message.toJSON())
                .setHeader(new Header("Authorization", "Bearer " + jwt)));

        return "user-profile-test";
    }

}