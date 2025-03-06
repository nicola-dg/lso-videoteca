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
                .sendRequest(requestService.createRequest().setMethod(Method.GET).setPath("/film").setHeader(new Header("Authorization", "Bearer " + jwt)));
        try {
            List<FilmDTO> films = responseService.parseFilms(res.getPayload());
            films.forEach(System.out::println);
         } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
         }

        return "main";
    }

    @PostMapping("/add-movie")
    public String addMovie(
            @RequestParam String title,
            @RequestParam String genre,
            @RequestParam double price,
            HttpSession session) {

            FilmDTO film = new FilmDTO();
            film.setTitle(title);
            film.setGenre(genre);
            film.setPrice(price);
            Response res = requestService.sendRequest(requestService.createRequest().setMethod(Method.POST).setPath("/film").setPayload(film.toJSON()));
            return "redirect:/admin-dashboard";
       
    }

    @GetMapping("/loans")
    public String getLoans(Model model, HttpSession session) {
        SocketClient userSocket = (SocketClient) session.getAttribute("userSocket");
        if (userSocket == null)
            return "redirect:/login";

        try {
            String response = userSocket.sendRequest("GET_LOANS");
            model.addAttribute("loans", response);
            return "loansPage";
        } catch (IOException e) {
            model.addAttribute("error", "Errore di connessione");
            return "loansPage";
        }
    }

    @GetMapping("/user/{userId}/movies")
    public String getUserMovies(@PathVariable String userId, Model model, HttpSession session) {
        SocketClient userSocket = (SocketClient) session.getAttribute("userSocket");
        if (userSocket == null)
            return "redirect:/login";

        try {
            String response = userSocket.sendRequest("GET_USER_MOVIES;" + userId);
            model.addAttribute("userMovies", response);
            return "user-profile";
        } catch (IOException e) {
            model.addAttribute("error", "Errore di connessione");
            return "main";
        }
    }
}