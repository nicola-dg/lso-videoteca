# Usa un'immagine leggera di OpenJDK
FROM openjdk:17-jdk-slim

# Crea una directory per l'applicazione
WORKDIR /app

# Copia solo il JAR già buildato
COPY target/client-0.0.1-SNAPSHOT.jar client-0.0.1-SNAPSHOT.jar

# Esponi la porta
EXPOSE 8080

# Avvia l'applicazione
CMD ["java", "-jar", "client-0.0.1-SNAPSHOT.jar"]
