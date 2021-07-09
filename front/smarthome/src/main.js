import { createApp } from 'vue'
import App from './App.vue'
import "bootstrap/dist/css/bootstrap.min.css"
import axios from 'axios'
import mqtt from 'mqtt'
// import retryAxios from 'retry-axios'

// axios.defaults.baseURL='http://192.168.0.127:37777'

// retryAxios(axios, {
//     retries: 3, // number of retries
//     retryDelay: (retryCount) => {
//       console.log(`retry attempt: ${retryCount}`);
//       return retryCount * 2000; // time interval between retries
//     }
// })

const app = createApp(App)
app.config.globalProperties.axios=axios.create({
    baseURL: 'http://192.168.0.127:37777',
    timeout: 5000
  });
app.config.globalProperties.axiosDevice=axios.create({
    baseURL: 'http://192.168.4.1:8080',
    timeout: 3000
  });
app.config.globalProperties.mqttClient=mqtt
app.mount('#app')