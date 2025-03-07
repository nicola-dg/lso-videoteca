package com.lso.client.DTO;

import com.fasterxml.jackson.annotation.JsonProperty;

public class FilmDTO {

    @JsonProperty("id")
    private String id;

    @JsonProperty("title")
    private String title;

    @JsonProperty("genre")
    private String genre;

    @JsonProperty("total_copies")
    private String totalCopies;

    @JsonProperty("available_copies")
    private String availableCopies;

    @JsonProperty("loan_count")
    private String loanCount;

    @JsonProperty("price")
    private Double price;

    // Costruttore vuoto
    public FilmDTO() {
    }

    // Getter e Setter
    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getGenre() {
        return genre;
    }

    public void setGenre(String genre) {
        this.genre = genre;
    }

    public String getTotalCopies() {
        return totalCopies;
    }

    public void setTotalCopies(String totalCopies) {
        this.totalCopies = totalCopies;
    }

    public String getAvailableCopies() {
        return availableCopies;
    }

    public void setAvailableCopies(String availableCopies) {
        this.availableCopies = availableCopies;
    }

    public String getLoanCount() {
        return loanCount;
    }

    public void setLoanCount(String loanCount) {
        this.loanCount = loanCount;
    }

    public Double getPrice() {
        return this.price;
    }

    public void setPrice(Double price) {
        this.price = price;
    }

    // Metodo toString per debug e logging
    @Override
    public String toString() {
        return "Film{" +
                "id=" + id +
                ", title='" + title + '\'' +
                ", genre='" + genre + '\'' +
                ", totalCopies=" + totalCopies +
                ", availableCopies=" + availableCopies +
                ", price=" + price.toString() +
                ", loanCount=" + loanCount +
                '}';
    }

    public String toJSON() {
        StringBuilder json = new StringBuilder("{");
        boolean first = true;

        if (id != null) {
            json.append("\\\"id\\\":").append(escapeString(id));
            first = false;
        }
        if (title != null) {
            if (!first)
                json.append(",");
            json.append("\\\"title\\\":").append(escapeString(title));
            first = false;
        }
        if (genre != null) {
            if (!first)
                json.append(",");
            json.append("\\\"genre\\\":").append(escapeString(genre));
            first = false;
        }
        if (totalCopies != null) {
            if (!first)
                json.append(",");
            json.append("\\\"total_copies\\\":").append(escapeString(totalCopies));
            first = false;
        }
        if (availableCopies != null) {
            if (!first)
                json.append(",");
            json.append("\\\"avaliable_copies\\\":").append(escapeString(availableCopies));
            first = false;
        }
        if (loanCount != null) {
            if (!first)
                json.append(",");
            json.append("\\\"loan_count\\\":").append(escapeString(loanCount));
        }
        if (price != null) {
            if (!first)
                json.append(",");
            json.append("\\\"price\\\":").append(escapeString(price.toString()));
        }

        json.append("}");
        return json.toString();
    }

    private String escapeString(String s) {
        return "\\\"" + s + "\\\"";
    }
}
