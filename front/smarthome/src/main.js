import { createApp } from 'vue'
import App from './App.vue'
import "bootstrap/dist/css/bootstrap.min.css"
import axios from 'axios'

axios.defaults.baseURL='http://localhost:37777'

const app = createApp(App)
app.config.globalProperties.axios=axios
app.mount('#app')