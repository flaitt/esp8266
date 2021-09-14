import { createApp } from 'vue'
import App from './App.vue'
import "bootstrap/dist/css/bootstrap.min.css"
import axios from 'axios'
import Router from './router'

const app = createApp(App)
app.use(Router)
app.config.globalProperties.userName='';
app.config.globalProperties.axios=axios.create({
    baseURL: 'https://smarthomedosfluxo.herokuapp.com',
    timeout: 10000
  });
app.config.globalProperties.axiosDevice=axios.create({
    baseURL: 'http://192.168.4.1:8080',
    timeout: 3000
  });
app.mount('#app')