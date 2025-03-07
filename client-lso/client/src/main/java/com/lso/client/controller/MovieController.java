package com.lso.client.controller;

import com.lso.client.DTO.FilmDTO;
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
            List<FilmDTO> films = responseService.parseFilms(res.getPayload());
            films.forEach(System.out::println);
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
        return "redirect:/user-profile-test";

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

        return "redirect:/user-profile-test";
    }

    // @GetMapping("/loans") //L'UTENTE VEDE I NOLEGGI ATTIVI
    // public String getLoans(Model model, HttpSession session) {
    // SocketClient userSocket = (SocketClient) session.getAttribute("userSocket");
    // if (userSocket == null)
    // return "redirect:/login";

    // try {
    // String response = userSocket.sendRequest("GET_LOANS");
    // model.addAttribute("loans", response);
    // return "loansPage";
    // } catch (IOException e) {
    // model.addAttribute("error", "Errore di connessione");
    // return "loansPage";
    // }
    // }

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

        return "loansPage";
    }

    @GetMapping("/loan/film/return")
    public String showReturnFilmToLoan() {
        return "return-film-loan";
    }
    
    @PostMapping("/loan/film/return")
    public String returnFilmLoan(HttpSession session, @RequestParam String id) {
        System.out.println("loan film con PUT attivata");
        String jwt = (String) session.getAttribute("jwt");
        FilmDTO film = new FilmDTO();
        film.setId(id);
        Response res = requestService
                .sendRequest(requestService.createRequest().setMethod(Method.PUT).setPath("/loan/film")
                        .setPayload(film.toJSON()).setHeader(new Header("Authorization", "Bearer " + jwt)));

        return "user-profile-test";
    }

    // @GetMapping("/user/{userId}/movies")
    // public String getUserMovies(@PathVariable String userId, Model model,
    // HttpSession session) {
    // SocketClient userSocket = (SocketClient) session.getAttribute("userSocket");
    // if (userSocket == null)
    // return "redirect:/login";

    // try {
    // String response = userSocket.sendRequest("GET_USER_MOVIES;" + userId);
    // model.addAttribute("userMovies", response);
    // return "user-profile";
    // } catch (IOException e) {
    // model.addAttribute("error", "Errore di connessione");
    // return "main";
    // }
    // }
}