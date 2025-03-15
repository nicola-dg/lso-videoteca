FROM ubuntu:22.04

# Crea una directory per l'app C
WORKDIR /app

# Copia solo la cartella bin contenente gli eseguibili
COPY bin /app/bin

# Installa le librerie necessarie
RUN apt-get update && apt-get install -y libjansson-dev libjwt-dev libjwt0 libpq-dev && ldconfig
 

# Imposta la variabile d'ambiente per i percorsi delle librerie
ENV LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu:/usr/local/lib:$LD_LIBRARY_PATH

# Imposta la porta su cui gira l'app
EXPOSE 3000

# Avvia il programma compilato
CMD ["./bin/main.o"]
