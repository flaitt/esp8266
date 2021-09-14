<template>
  <div>
    <img src="./../static/home-icon.png">
    <h2>Smartcommand</h2>
    <div class="sign-up">
      <p>Crie uma nova conta</p>
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
      <div class="spinner-border text-info" v-if="isLoading"></div>
      <button @click="signUp" v-else>Registrar</button>
      <span>
        ou retorne ao
        <router-link to="/"> login.</router-link>
      </span>
    </div>
  </div>
</template>

<script>
// import firebase from 'firebase';
export default {
  name: 'signUp',
  data () {
    return {
      userNameUsed: '',
      senha: '',
      isLoading: false
    };
  },
  methods: {
    signUp () {
        this.isLoading = true;
        this.axios.post(`createNewUser?userName=${this.userNameUsed}&password=${this.senha}`)
        .then(() => {
            this.isLoading = false;
            alert(`Bem Vindo, ${this.userNameUsed}`)
            var userName = this.userNameUsed;
            this.$router.replace({name: 'devices', params: {userName}});
        }).catch((err) => {
            this.isLoading = false;
            alert('Não foi possível realizar a criação da conta. ' + err.message)
        });
    //   firebase.auth().createUserWithEmailAndPassword(this.userNameUsed, this.senha).then(
    //     (user) => {
    //       this.$router.replace('home'),
    //         alert('Sua conta foi cadastrata com sucesso!')
    //     },
    //     (err) => {
    //       alert('Aconteceu algo inesperado. ' + err.message)
    //     }
    //   );
    },
  },
};
</script>

 <style scoped>
.sign-up {
  margin-top: 40px;
  border-style: solid;
  margin: 42px;
  padding: 33px;
  border-radius: 35px;
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
span {
  display: block;
  margin-top: 20px;
  font-size: 11px;
}
</style>