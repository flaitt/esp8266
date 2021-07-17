// import Vue from 'vue'
import { createRouter, createWebHistory } from 'vue-router'
import Devices from './components/Devices'
import Login from './components/Login'
import SingUp from './components/SingUp'

export default createRouter({
    history: createWebHistory(),
    routes: [{
        name: 'login',
        path: '/',
        component: Login,
        props: true
    }, {
        name: 'devices',
        path: '/devices',
        component: Devices,
        props: true
    }, {
        path: '/singUp',
        component: SingUp,
        props: true
    }]
})