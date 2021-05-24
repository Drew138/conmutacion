package main

import (
	"encoding/json"
	"log"
	"net/http"
	"time"

	"github.com/gorilla/mux"
)

type Data struct {
	Temp  int   `json:"temp"`
	Light int   `json:"light"`
	R     uint8 `json:"R"`
	G     uint8 `json:"G"`
	B     uint8 `json:"B"`
	Angle int   `json:"angle"`
}

var data Data

func getData(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(data) // read data variable and encode it in response writer as json
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

func setAngle(w http.ResponseWriter, r *http.Request) {
	receivedData := Data{}
	err := json.NewDecoder(r.Body).Decode(&receivedData)
	if err != nil {
		http.Error(w, err.Error(), http.StatusBadRequest)
	}
	data.Angle = receivedData.Angle
	json.NewEncoder(w).Encode(data)
}

func writeData(w http.ResponseWriter, r *http.Request) {
	receivedData := Data{}
	err := json.NewDecoder(r.Body).Decode(&receivedData) // decode json in request and write it in receivedData variable attributes
	if err != nil {
		http.Error(w, err.Error(), http.StatusBadRequest)
	}
	data.Temp = receivedData.Temp // change data attributes to those of receivedData
	// data.Count = receivedData.Count
	data.Light = receivedData.Light
	json.NewEncoder(w).Encode(data) // enconde json response with attributes in data
}

func main() {
	r := mux.NewRouter()
	r.HandleFunc("/read", getData).Methods("GET") // register function handlers
	r.HandleFunc("/write", writeData).Methods("POST")
	r.HandleFunc("/led", setLed).Methods("POST")
	r.HandleFunc("/angle", setAngle).Methods("POST")

	srv := &http.Server{
		Handler:      r,
		Addr:         ":8000",
		WriteTimeout: 15 * time.Second,
		ReadTimeout:  15 * time.Second,
	}

	log.Fatal(srv.ListenAndServe())
}

// https://circuits4you.com/2019/01/11/nodemcu-esp8266-arduino-json-parsing-example/
