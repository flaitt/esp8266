<template>
  <div class="login">
    <h1>Login</h1>
    <input
      type="text"
      placeholder="Usuário"
      v-model="userNameUsed"
    >
    <br>
    <input
      type="password"
      placeholder="Senha"
      v-model="senha"
    >
    <br>
    <div class="btn-login">
      <button @click="login">Login</button>
    </div>
    <p>
      Você não tem conta?
      <span>
        <!-- <router-link to="/">crie uma conta</router-link> -->
        <router-link to="/singUp">crie uma conta</router-link>
      </span>
    </p>
  </div>
</template>

<script>
export default {
  name: "login",
  data () {
    return {
      userNameUsed: '',
      senha: ''
    }
  },
   methods: {
      login: function() {
        this.axios.post(`authenticate?userName=${this.userNameUsed}&password=${this.senha}`)
        .then(() => {
            alert(`Bem Vindo, ${this.userNameUsed}`)
            var userName = this.userNameUsed;
            this.$router.replace({name: 'devices', params: {userName}});
        }).catch((err) => {
            alert('Não foi possível realizar o login. ' + err.message)
        });
        // firebase.auth().signInWithEmailAndPassword(this.userNameUsed, this.senha).then(
        //   (user) => {
        //     this.$router.replace('home')
        //     alert(`Bem Vindo, ${{userNameUsed}}`)
        //   },
        //   (err) => {
        //     alert('Não foi possível realizar o login. ' + err.message)
        //   }
        // );
      }
    }
  }
</script>

<style scoped>
.login {
  margin-top: 40px;
  border-style: solid;
  margin: 42px;
  padding: 33px;
  border-radius: 35px;
  /* border-color: #4DD0E1; */
}
input {
  margin: 10px 0;
  padding: 15px;
  border-radius: 10px;
  border-style: solid;
  border-color: #0cbcd3;
}
input:focus {
  outline: none;
}
button {
  font-family: "Roboto", sans-serif;
  text-transform: uppercase;
  outline: 0;
  background: #0cbcd3;
  border: 0;
  padding: 15px;
  color: #ffffff;
  font-size: 14px;
  -webkit-transition: all 0.3 ease;
  transition: all 0.3 ease;
  cursor: pointer;
  border-radius: 5px;
}
p {
  margin-top: 40px;
  font-size: 13px;
}
p a {
  text-decoration: underline;
  cursor: pointer;
}
</style>