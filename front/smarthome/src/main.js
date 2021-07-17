import { createApp } from 'vue'
import App from './App.vue'
import "bootstrap/dist/css/bootstrap.min.css"
import axios from 'axios'
// import mqtt from 'mqtt'
// import websocket from 'websocket'
// import Stomp from 'stompjs'
import Router from './router'
// import { w3cwebsocket as W3CWebSocket } from "websocket"

// import express from 'express'
// import retryAxios from 'retry-axios'

// axios.defaults.baseURL='http://192.168.0.127:37777'

// retryAxios(axios, {
//     retries: 3, // number of retries
//     retryDelay: (retryCount) => {
//       console.log(`retry attempt: ${retryCount}`);
//       return retryCount * 2000; // time interval between retries
//     }
// })

// var c = new WebS
const app = createApp(App)
app.use(Router)
app.config.globalProperties.userName='';
app.config.globalProperties.axios=axios.create({
    baseURL: 'https://smarthomedosfluxo.herokuapp.com',
    // baseURL: 'http://192.168.0.127:37777',
    timeout: 10000
  });
app.config.globalProperties.axiosDevice=axios.create({
    baseURL: 'http://192.168.4.1:8080',
    timeout: 3000
  });
// const wes = new WebSocket('ws://127.0.0.1:15674/ws');
// app.config.globalProperties.wsClient = Stomp.over(new WebSocket('ws://127.0.0.1:15674/ws'));
// app.config.globalProperties.mqttClient=mqtt
app.mount('#app')