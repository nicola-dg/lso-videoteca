package com.lso.client.model;

import jakarta.validation.constraints.NotBlank;
import lombok.AllArgsConstructor;
import lombok.NoArgsConstructor;
import lombok.Setter;


@AllArgsConstructor
@NoArgsConstructor
@Setter
public class RegistrationForm {

    @NotBlank
    private String username;
    @NotBlank
    private String email;
    @NotBlank
    private String nome;
    @NotBlank
    private String cognome;
    @NotBlank
    private String password;
    @NotBlank
    private String ripeti_password;

    public String getUsername() {
        return this.username;
    }

    public String getEmail() {
        return this.email;
    }

    public String getNome() {
        return this.nome;
    }

    public String getCognome() {
        return this.cognome;
    }

    public String getPassword() {
        return this.password;
    }

    public String getRipeti_password() {
        return this.ripeti_password;
    }
}
