package com.lso.client.controller;

import com.lso.client.service.RequestService;
import com.lso.client.service.ResponseService.Response;
import com.lso.client.service.SocketClient;
import com.lso.client.types.Method;

import jakarta.servlet.http.HttpSession;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;

import java.io.IOException;

@Controller
@RequestMapping("/")
public class MovieController {

    private RequestService requestService;
        private SocketClient socket;
    
        public MovieController(RequestService requestService, SocketClient socket) {
            this.requestService = requestService;
            this.socket = socket;
        }

    @GetMapping("/home")
    public String getAllMovies(Model model, HttpSession session) {
        // try {
        //     Response response = userSocket.sendRequest("GET_ALL_MOVIES");
        //     model.addAttribute("movies", response);
        //     return "main";
        // } catch (IOException e) {
        //     model.addAttribute("error", "Errore di connessione");
        //     return "main";
        // }
        Response res = requestService.sendRequest(requestService.createRequest().setMethod(Method.GET).setPath("/film"));

        return "main";
    }

    @PostMapping("/add-movie")
    public String addMovie(
            @RequestParam String title,
            @RequestParam String genre,
            @RequestParam double price,
            HttpSession session) {
        SocketClient userSocket = (SocketClient) session.getAttribute("userSocket");
        if (userSocket == null)
            return "redirect:/login";

        try {
            userSocket.sendRequest("ADD_MOVIE;" + title + ";" + genre + ";" + price);
            return "redirect:/admin-dashboard";
        } catch (IOException e) {
            return "redirect:/admin-dashboard?error";
        }
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