package main

import (
	"encoding/json"
	"log"
	"net/http"
	"time"

	"github.com/gorilla/mux"
)

type Data struct {
	Temp  string `json:"temp"`
	Light string `json:"light"`
	R     uint8  `json:"R"`
	G     uint8  `json:"G"`
	B     uint8  `json:"B"`
	// Count int    `json:"count"`
}

var data Data

func getData(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(data)
}

func setLed(w http.ResponseWriter, r *http.Request) {
	receivedData := Data{}
	err := json.NewDecoder(r.Body).Decode(&receivedData)
	if err != nil {
		http.Error(w, err.Error(), http.StatusBadRequest)
	}
	data.R = receivedData.R
	data.G = receivedData.G
	data.B = receivedData.B
	json.NewEncoder(w).Encode(data)
}

func writeData(w http.ResponseWriter, r *http.Request) {
	receivedData := Data{}
	err := json.NewDecoder(r.Body).Decode(&receivedData)
	if err != nil {
		http.Error(w, err.Error(), http.StatusBadRequest)
	}
	data.Temp = receivedData.Temp
	// data.Count = receivedData.Count
	data.Light = receivedData.Light
	json.NewEncoder(w).Encode(data)
}

func main() {
	r := mux.NewRouter()
	r.HandleFunc("/read", getData).Methods("GET")
	r.HandleFunc("/write", writeData).Methods("POST")
	r.HandleFunc("/led", setLed).Methods("POST")

	srv := &http.Server{
		Handler:      r,
		Addr:         ":8000",
		WriteTimeout: 15 * time.Second,
		ReadTimeout:  15 * time.Second,
	}

	log.Fatal(srv.ListenAndServe())
}
