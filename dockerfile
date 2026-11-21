FROM gcc:latest

WORKDIR /app

COPY . .
RUN g++ backend/inventory.cpp -o inventory -pthread
EXPOSE 8080

CMD ["./inventory"]