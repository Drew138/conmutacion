package main

import (
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"time"

	"github.com/gorilla/mux"
)

type Data struct {
	Temp string `json:"text"`
	Led  bool   `json:"led"`
	hi   bool   `json:"hi"`
}

var data Data

func getData(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")

	json.NewEncoder(w).Encode(data)
}

func writeData(w http.ResponseWriter, r *http.Request) {
	receivedData := Data{}
	err := json.NewDecoder(r.Body).Decode(&receivedData)
	if err != nil {
		http.Error(w, err.Error(), http.StatusBadRequest)
	}
	data.Text = receivedData.Text
	fmt.Fprint(w, "Data written correctly")
}

func main() {
	r := mux.NewRouter()
	r.HandleFunc("/read", getData).Methods("GET")
	r.HandleFunc("/write", writeData).Methods("POST")

	srv := &http.Server{
		Handler:      r,
		Addr:         ":8000",
		WriteTimeout: 15 * time.Second,
		ReadTimeout:  15 * time.Second,
	}

	log.Fatal(srv.ListenAndServe())
}
