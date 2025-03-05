package com.lso.client.DTO;

public class UserDTO {
    String username;
    String password;
    String email;
    String name;
    String surname;
    String role;

    Integer max_loans;

    public String getUsername() {
        return this.username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getPassword() {
        return this.password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public String getEmail() {
        return this.email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public String getName() {
        return this.name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getSurname() {
        return this.surname;
    }

    public void setSurname(String surname) {
        this.surname = surname;
    }

    public Integer getMax_loans() {
        return this.max_loans;
    }

    public void setMax_loans(Integer max_loans) {
        this.max_loans = max_loans;
    }

    public String getRole() {
        return this.role;
    }

    public void setRole(String role) {
        this.role = role;
    }

    public String toJSON() {
        StringBuilder json = new StringBuilder("{");
        boolean first = true;

        if (username != null) {
            json.append("\\\"username\\\":").append(escapeString(username));
            first = false;
        }
        if (password != null) {
            if (!first)
                json.append(",");
            json.append("\\\"password\\\":").append(escapeString(password));
            first = false;
        }
        if (email != null) {
            if (!first)
                json.append(",");
            json.append("\\\"email\\\":").append(escapeString(email));
            first = false;
        }
        if (name != null) {
            if (!first)
                json.append(",");
            json.append("\\\"name\\\":").append(escapeString(name));
            first = false;
        }
        if (surname != null) {
            if (!first)
                json.append(",");
            json.append("\\\"surname\\\":").append(escapeString(surname));
            first = false;
        }
        if (role != null) {
            if (!first)
                json.append(",");
            json.append("\\\"role\\\":").append(escapeString(role));
        }
        if (max_loans != null) {
            if (!first)
                json.append(",");
            json.append("\\\"max_loans\\\":").append(escapeString(max_loans.toString()));
        }

        json.append("}");
        return json.toString();
    }

    private String escapeString(String s) {
        return "\\\"" + s + "\\\"";
    }
}
